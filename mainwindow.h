void create_window();
void restart_program()
{
	gtk_widget_destroy(window);
	readconf();
	create_window();
}

void on_submenu_item2_selected(GtkMenuItem *menuitem, gpointer userdata) 
{
	dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title(GTK_WINDOW(dialog), "Help");
		gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 200);
		theme = gtk_icon_theme_get_default();
		info = gtk_icon_theme_lookup_icon(theme, "menulibre", 48, 0);
		if (info != NULL)
		{
			icon = gtk_icon_info_load_icon(info, NULL);
			gtk_window_set_icon(GTK_WINDOW(dialog), icon);
			g_object_unref(icon);
			g_object_unref(info);
		}
		gtk_container_set_border_width(GTK_CONTAINER(dialog), 10);
		GtkWidget *label = gtk_label_new(" - SGLauncher is a quick launcher that can run programs, desktop actions, run on terminal or search on web\n - You can run the first item on the list by pressing enter, or press down to select other item \n - You can press CTRL+T to run on terminal and CTRL+B to Search on Web\n - You can customize the program in the Settings or editing the config file on ~/.config/sglauncher.conf");
	gtk_container_add(GTK_CONTAINER(dialog), label);
	gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
	gtk_widget_show_all(dialog);
}

void on_submenu_item3_selected(GtkMenuItem *menuitem, gpointer userdata) 
{
	dialog = gtk_about_dialog_new();
		theme = gtk_icon_theme_get_default();
		info = gtk_icon_theme_lookup_icon(theme, "menulibre", 48, 0);
		if (info != NULL)
		{
			icon = gtk_icon_info_load_icon(info, NULL);
			gtk_window_set_icon(GTK_WINDOW(dialog), icon);
			g_object_unref(icon);
			g_object_unref(info);
		}
	gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "SGLauncher");
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "Copyright © 2024 ItzSelenux for Simple GTK Desktop Environment");
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "Simple GTK Launcher");
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), pver);
	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "https://itzselenux.github.io/sglauncher");
	gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(dialog), "Project WebSite");
	gtk_about_dialog_set_license_type(GTK_ABOUT_DIALOG(dialog),GTK_LICENSE_GPL_3_0);
	gtk_about_dialog_set_logo_icon_name(GTK_ABOUT_DIALOG(dialog),"menulibre");
	gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

void create_window()
{
	char local_app_dir[1024] = "";
	sprintf(local_app_dir, "%s/.local/share/applications", home_dir);
	app_dirs[2] = local_app_dir;

	listbox2 = gtk_list_box_new();
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "SGLauncher");
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	gtk_widget_set_size_request(window, 333, 333);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	theme = gtk_icon_theme_get_default();
	info = gtk_icon_theme_lookup_icon(theme, "menulibre", 48, 0);
	if (info != NULL) 
	{
		GdkPixbuf *icon = gtk_icon_info_load_icon(info, NULL);
		gtk_window_set_icon(GTK_WINDOW(window), icon);
		g_object_unref(icon);
		g_object_unref(info);
	}

	headerbar = gtk_header_bar_new();
	gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(headerbar), TRUE);


	entry = gtk_entry_new();

	GtkWidget *button = gtk_menu_button_new();
	image = gtk_image_new_from_icon_name("menulibre", GTK_ICON_SIZE_BUTTON);
	gtk_container_add(GTK_CONTAINER(button), image);

	gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), button);
	wtitle = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(wtitle), "<b>SGLauncher</b>");
	gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), wtitle);
	gtk_widget_set_hexpand(GTK_WIDGET(entry), FALSE);
	gtk_widget_set_vexpand(GTK_WIDGET(entry), FALSE);

	submenu = gtk_menu_new();

	submenu_item1 = gtk_menu_item_new_with_label("Settings");
	submenu_item2 = gtk_menu_item_new_with_label("Help");
	submenu_item3 = gtk_menu_item_new_with_label("About");

	gtk_menu_shell_append(GTK_MENU_SHELL(submenu), submenu_item1);
	gtk_menu_shell_append(GTK_MENU_SHELL(submenu), submenu_item2);
	gtk_menu_shell_append(GTK_MENU_SHELL(submenu), submenu_item3);

	gtk_widget_show_all(submenu);

	gtk_menu_button_set_popup(GTK_MENU_BUTTON(button), submenu);


	if (nocsd == 0)
	{
		gtk_header_bar_pack_end(GTK_HEADER_BAR(headerbar), entry);
		gtk_window_set_titlebar(GTK_WINDOW(window), headerbar);
	}

	scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
		GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	listbox = gtk_list_box_new();
	gtk_list_box_set_selection_mode(GTK_LIST_BOX(listbox), GTK_SELECTION_SINGLE);
	gtk_container_add(GTK_CONTAINER(scrolled_window), listbox);
	gtk_widget_grab_focus(entry);

	cmd_row = gtk_list_box_row_new();
	GtkWidget *cmd_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_container_add(GTK_CONTAINER(cmd_row), cmd_box);

	GtkWidget *cmdicon = gtk_image_new_from_icon_name("terminal-tango", GTK_ICON_SIZE_BUTTON);
	gtk_box_pack_start(GTK_BOX(cmd_box), cmdicon, FALSE, FALSE, 0);

	GtkWidget *cmdrun = gtk_label_new("Run in Terminal");
	gtk_box_pack_start(GTK_BOX(cmd_box), cmdrun, FALSE, FALSE, 0);

	web_row = gtk_list_box_row_new();
	web_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_container_add(GTK_CONTAINER(web_row), web_box);

	GtkWidget *webicon = gtk_image_new_from_icon_name("edit-find", GTK_ICON_SIZE_BUTTON);
	gtk_box_pack_start(GTK_BOX(web_box), webicon, FALSE, FALSE, 0);

	GtkWidget *webrun = gtk_label_new("Search on Web");
	gtk_box_pack_start(GTK_BOX(web_box), webrun, FALSE, FALSE, 0);
	
	if (showcmd == 1)
	{
		gtk_list_box_insert(GTK_LIST_BOX(listbox2), cmd_row, -1);
	}
	
	if (showweb == 1)
	{
		gtk_list_box_insert(GTK_LIST_BOX(listbox2), web_row, -1);
	}
	gtk_widget_set_size_request(web_row, -1, 32);
	gtk_widget_set_size_request(cmd_row, -1, 32);


	g_signal_connect(window, "key-release-event", G_CALLBACK(on_key_release), row);
	g_signal_connect(submenu_item1, "activate", G_CALLBACK(showcfg), NULL);
	g_signal_connect(submenu_item2, "activate", G_CALLBACK(on_submenu_item2_selected), NULL);
	g_signal_connect(submenu_item3, "activate", G_CALLBACK(on_submenu_item3_selected), NULL);
	g_signal_connect(listbox2, "row-activated", G_CALLBACK(on_run_command), entry);
	g_signal_connect(listbox, "row-activated", G_CALLBACK(on_item_activated), NULL);
	g_signal_connect(entry, "changed", G_CALLBACK(filter_listbox), listbox);
	g_signal_connect(window, "button-press-event", G_CALLBACK(on_button_press), submenu);

	// Load apps into the list box
	grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(window), grid);
	mathtext = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	GtkWidget *math = gtk_label_new("Math Answer: ");
	manswer = gtk_label_new("");
	gtk_label_set_selectable(GTK_LABEL(manswer), TRUE);
	pr = gtk_label_new("Press Enter to Run first item, or down to select another item");

	if (showcalc == 1)
	{
		gtk_box_pack_start(GTK_BOX(mathtext), math, TRUE, TRUE, 0);
		gtk_box_pack_start(GTK_BOX(mathtext), manswer, TRUE, TRUE, 0);
	}

	gtk_widget_set_vexpand(scrolled_window, TRUE);
	gtk_widget_set_hexpand(scrolled_window, TRUE);

	if (order == 1)
	{
		gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 4, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), listbox2, 0, 3, 1, 1);
	}
	else if (order == 0)
	{
		gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 3, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), listbox2, 0, 4, 1, 1);
	}
	else if (order == 2)
	{
		gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 3, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), listbox2, 1, 3, 1, 1);
	}
	else if (order == 3)
	{
		gtk_grid_attach(GTK_GRID(grid), scrolled_window, 1, 3, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), listbox2, 0, 3, 1, 1);
	}
	gtk_widget_set_size_request(listbox2, -1, -1);
	gtk_widget_set_size_request(scrolled_window, -1, 256);
	gtk_grid_attach(GTK_GRID(grid), entry, 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), pr, 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), mathtext, 0, 2, 1, 1);

	GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_container_add(GTK_CONTAINER(scrolled_window), GTK_WIDGET(listbox));

	if (sgcfg)
	{
		gtk_widget_activate(submenu_item1);
	}
	else
	{
		gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
		load_apps(GTK_LIST_BOX(listbox));
		gtk_widget_show_all(window);
		gtk_widget_hide(mathtext);
		gtk_widget_hide(listbox2);
		gtk_widget_hide(pr);
		gtk_main();
	}
}