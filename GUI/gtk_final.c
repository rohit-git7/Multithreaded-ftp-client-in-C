#include"header.h"
#include"print_buff.h"
#include"list_client.h"
#include"passive_connect.h"
#include"list_content.h"
#include"list_content_view.h"
#include"thread_get.h"
#include"put_content.h"
#include"put_unique.h"
#include"get_content.h"

void no_connection(Appstate *);
void already_running(Appstate *);
void serv_cd_dir(Appstate *, char *);	
void cli_cd_dir(Appstate *, char *);	
void cli_rmfile(Appstate *, char *);	
void serv_rmfile(Appstate *, char *);	
void down_serv(Appstate *,char *);
void rename_serv_func(GtkWidget *,Appstate *);
void rename_cli(Appstate *,char *,char *);
void rename_serv(Appstate *,char *,char *);
gboolean on_popup_focus_out (GtkWidget *,GdkEventFocus *,gpointer data);
void new_func(Appstate *);
void new_func_cli(Appstate *);

void move_up_dir_cli(GtkWidget *widget, Appstate *app_state)
{
	char user_input[MAXSZ];

	bzero(user_input,MAXSZ);

	sprintf(user_input,"!cd ..");
	gtk_entry_set_text(GTK_ENTRY(app_state->command),user_input); 
	cli_cd_dir(app_state,user_input);	
	gtk_entry_set_text(GTK_ENTRY(app_state->command),"Command");
	
	return;
}


void move_up_dir(GtkWidget *widget, Appstate *app_state)
{
	char user_input[MAXSZ];

	bzero(user_input,MAXSZ);

	sprintf(user_input,"cd ..");
	gtk_entry_set_text(GTK_ENTRY(app_state->command),user_input); 
	serv_cd_dir(app_state,user_input);	
	gtk_entry_set_text(GTK_ENTRY(app_state->command),"Command");
	
	return;
}

/* Upload file to server */
void upload_cli(Appstate *app_state,char *user_input)
{
	if(app_state->status == 0)//Connection not established
	{
		no_connection(app_state);//Call popup window function
		return;
	}

	if(app_state->running == 1)//Command already running
	{
		already_running(app_state);//Call popup window function
		return;
	}

	char *address = (char *)malloc(MAXSZ);
	
	/* Get IP Address of server from IP Address entry */
	address = (char *)gtk_entry_get_text(GTK_ENTRY(app_state->entry));
	char *argv = address;	

	clock_t start;
	clock_t end;
	
	double cpu_time;
	
	int ret_value;
	
	/* Set 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
	app_state->running = 1;
	gtk_entry_set_text(GTK_ENTRY(app_state->command),user_input); 

	start = clock();//Start clock
	put_unique(argv,user_input,app_state);//Call function to put files
	end = clock();//Stop clock
	cpu_time = ((double)(end - start))/CLOCKS_PER_SEC;//Calculate CPU time
	sprintf(buff,"Time taken %lf\n\n",cpu_time);
	print_buff(app_state);

	/* Reset 'running' variable */
	ret_value = list_content_view(argv,"ls -l",app_state);
	if(ret_value == 0)
		new_func(app_state);		
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;
	gtk_entry_set_text(GTK_ENTRY(app_state->command),"Command"); 

	return;
}

/* Upload from list store */
void view_popup_menu_upload_cli(GtkWidget *menuitem, Appstate *app_state)
{
	GtkTreeView *treeview = GTK_TREE_VIEW(app_state->view1);
	GtkTreeSelection *selection;
	GtkTreeModel *model;
	GtkTreeIter iter;
	
	char user_input[MAXSZ];
	char *perm = (char *)malloc(MAXSZ);
	char *filename = (char *)malloc(MAXSZ);

	bzero(user_input,MAXSZ);

	selection = gtk_tree_view_get_selection(treeview);
	if(gtk_tree_selection_get_selected(selection,&model,&iter))
	{	
		gtk_tree_model_get(model,&iter,COL_NAME,&filename,-1);
		gtk_tree_model_get(model,&iter,COL_PERM,&perm,-1);
		
		if(perm[0] != '-')
		{
			GtkWidget *popup_window;
			GtkWidget *table;
			GtkWidget *label;
			GtkWidget *horiz_align;
	
			/* Create window */
			popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
			gtk_window_set_title (GTK_WINDOW (popup_window), "Error");
			gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
			gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
			gtk_window_set_decorated (GTK_WINDOW (popup_window), TRUE);
			gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);
			gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
			gtk_widget_set_size_request (popup_window, 350, 60);
			gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
			gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);

			gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
			g_signal_connect (G_OBJECT (popup_window),
        		       			    "focus-out-event",
              			 		    G_CALLBACK (on_popup_focus_out),
                	   				NULL);
	
			table = gtk_table_new (2,2,FALSE);
			gtk_container_add(GTK_CONTAINER(popup_window),table);
			label = gtk_label_new("\t\tCannot upload. Not a regular file!");

			horiz_align = gtk_alignment_new(0,0,0,0);
	    
			gtk_container_add(GTK_CONTAINER(horiz_align),label);
			gtk_table_attach(GTK_TABLE(table),horiz_align,1,2,0,1,GTK_FILL,GTK_FILL,0,0);
	
			gtk_widget_show_all (popup_window);
			gtk_widget_grab_focus (popup_window);
			
		}	
		
		else
		{
			sprintf(user_input,"uniqput %s",filename);
			gtk_entry_set_text(GTK_ENTRY(app_state->command),user_input); 
			upload_cli(app_state,user_input);
			gtk_entry_set_text(GTK_ENTRY(app_state->command),"Command"); 
		}
	}
}

/* Remove from list store */
void view_popup_menu_remove_cli(GtkWidget *menuitem, Appstate *app_state)
{
	GtkTreeView *treeview = GTK_TREE_VIEW(app_state->view1);
	GtkTreeSelection *selection;
	GtkTreeModel *model;
	GtkTreeIter iter;
	
	char *filename = (char *)malloc(MAXSZ);
	char *perm = (char *)malloc(MAXSZ);
	
	char user_input[MAXSZ];

	bzero(user_input,MAXSZ);

	selection = gtk_tree_view_get_selection(treeview);
	if(gtk_tree_selection_get_selected(selection,&model,&iter))
	{	
		gtk_tree_model_get(model,&iter,COL_NAME,&filename,-1);
		gtk_tree_model_get(model,&iter,COL_PERM,&perm,-1);
		
		if(perm[0] != '-')
		{
			GtkWidget *popup_window;
			GtkWidget *table;
			GtkWidget *label;
			GtkWidget *horiz_align;
	
			/* Create window */
			popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
			gtk_window_set_title (GTK_WINDOW (popup_window), "Error");
			gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
			gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
			gtk_window_set_decorated (GTK_WINDOW (popup_window), TRUE);
			gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);
			gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
			gtk_widget_set_size_request (popup_window, 350, 60);
			gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
			gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);

			gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
			g_signal_connect (G_OBJECT (popup_window),
        		       			    "focus-out-event",
              			 		    G_CALLBACK (on_popup_focus_out),
                	   				NULL);
	
			table = gtk_table_new (2,2,FALSE);
			gtk_container_add(GTK_CONTAINER(popup_window),table);
			label = gtk_label_new("\t  Cannot remove. Not a regular file!");

			horiz_align = gtk_alignment_new(0,0,0,0);
	    
			gtk_container_add(GTK_CONTAINER(horiz_align),label);
			gtk_table_attach(GTK_TABLE(table),horiz_align,1,2,0,1,GTK_FILL,GTK_FILL,0,0);
	
			gtk_widget_show_all (popup_window);
			gtk_widget_grab_focus (popup_window);
			
		}	
		else		
 		{
			sprintf(user_input,"!rm %s",filename);
			cli_rmfile(app_state,user_input);	
		}
	}
}
 
/* Change directory on client from list store */
void view_popup_menu_change_dir_cli(GtkWidget *menuitem, Appstate *app_state)
{
	GtkTreeView *treeview = GTK_TREE_VIEW(app_state->view1);
	GtkTreeSelection *selection;
	GtkTreeModel *model;
	GtkTreeIter iter;

	char user_input[MAXSZ];

	
	char *filename = (char *)malloc(MAXSZ);
	char *perm = (char *)malloc(MAXSZ);
	
	
	selection = gtk_tree_view_get_selection(treeview);
	if(gtk_tree_selection_get_selected(selection,&model,&iter))
	{	
		gtk_tree_model_get(model,&iter,COL_NAME,&filename,-1);
		gtk_tree_model_get(model,&iter,COL_PERM,&perm,-1);
	}
	else
		return;	
   
	if(perm[0] != 'd')//Invalid entry
	{
		GtkWidget *popup_window;
		GtkWidget *table;
		GtkWidget *label;
		GtkWidget *horiz_align;
	
		popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title (GTK_WINDOW (popup_window), "Error");
		gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
		gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
		gtk_window_set_decorated (GTK_WINDOW (popup_window), TRUE);
		gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);	
		gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
		gtk_widget_set_size_request (popup_window, 340, 60);
		gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
		gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);
		
		gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
		g_signal_connect (G_OBJECT (popup_window),
	              			    "focus-out-event",
		              		 G_CALLBACK (on_popup_focus_out),							NULL);
		table = gtk_table_new (2,2,FALSE);
		gtk_container_add(GTK_CONTAINER(popup_window),table);
		label = gtk_label_new("Cannot change directory. Not a directory!");

		horiz_align = gtk_alignment_new(0,0,0,0);
    	
		gtk_container_add(GTK_CONTAINER(horiz_align),label);
		gtk_table_attach(GTK_TABLE(table),horiz_align,1,2,0,1,GTK_FILL,GTK_FILL,0,0);
	
		gtk_widget_show_all (popup_window);
		gtk_widget_grab_focus (popup_window);
	}
	else
	{
		sprintf(user_input,"!cd %s",filename);
				
		gtk_entry_set_text(GTK_ENTRY(app_state->command),user_input); 
		cli_cd_dir(app_state,user_input);
		gtk_entry_set_text(GTK_ENTRY(app_state->command),"Command"); 
	}


}

/* Rename from list store */
void view_popup_menu_rename_cli(GtkWidget *menuitem, Appstate *app_state)
{
	GtkTreeView *treeview = GTK_TREE_VIEW(app_state->view1);
	GtkTreeSelection *selection;
	GtkTreeModel *model;
	GtkTreeIter iter;
	GtkWidget *dialog;
	GtkWidget *entry;
	GtkWidget *entry1;
	GtkWidget *content_area;

	char user_input[MAXSZ];

	
	char *filename = (char *)malloc(MAXSZ);
	selection = gtk_tree_view_get_selection(treeview);
	if(gtk_tree_selection_get_selected(selection,&model,&iter))
	{	
		gtk_tree_model_get(model,&iter,COL_NAME,&filename,-1);
	}
	else
		return;

	dialog = gtk_dialog_new_with_buttons("Rename File(Client)",GTK_WINDOW(app_state->window),GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT,NULL,NULL,NULL,NULL);
	gtk_window_set_default_size(GTK_WINDOW(dialog),350,130);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Ok", 0);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Cancel", 1);

	content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	entry = gtk_entry_new(); 
	entry1 = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(entry1),filename); 
	gtk_entry_set_text(GTK_ENTRY(entry),"New name"); 
	gtk_entry_set_editable(GTK_ENTRY(entry1),FALSE);
	gtk_container_add(GTK_CONTAINER(content_area), entry1);
	gtk_container_add(GTK_CONTAINER(content_area), entry);

	gtk_widget_show_all(dialog);
	gint result = gtk_dialog_run(GTK_DIALOG(dialog));

	char *old_name = (char *)malloc(MAXSZ); // variable for entered text
	char *new_name = (char *)malloc(MAXSZ); // variable for entered text
   
	switch(result)
	{
		case 0:
			old_name = (char *)gtk_entry_get_text(GTK_ENTRY(entry1));
			new_name = (char *)gtk_entry_get_text(GTK_ENTRY(entry));

			if(strlen(old_name) == 0 || old_name == NULL || strlen(new_name) == 0 || new_name == NULL)//Invalid entry
			{
				GtkWidget *popup_window;
				GtkWidget *table;
				GtkWidget *label;
				GtkWidget *horiz_align;
	
				popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
				gtk_window_set_title (GTK_WINDOW (popup_window), "Input Error");
				gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
				gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
				gtk_window_set_decorated (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_widget_set_size_request (popup_window, 250, 60);
				gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
				gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);
		
				gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
				g_signal_connect (G_OBJECT (popup_window),
		               			    "focus-out-event",
		              		 G_CALLBACK (on_popup_focus_out),							NULL);
				table = gtk_table_new (2,2,FALSE);
				gtk_container_add(GTK_CONTAINER(popup_window),table);
				label = gtk_label_new("\t\tInvalid filename!");

				horiz_align = gtk_alignment_new(0,0,0,0);
    	
				gtk_container_add(GTK_CONTAINER(horiz_align),label);
				gtk_table_attach(GTK_TABLE(table),horiz_align,1,2,0,1,GTK_FILL,GTK_FILL,0,0);
	
				gtk_widget_show_all (popup_window);
				gtk_widget_grab_focus (popup_window);
			}
			else
			{
				sprintf(user_input,"!rename %s %s",old_name,new_name);
				
				gtk_entry_set_text(GTK_ENTRY(app_state->command),user_input); 
				rename_cli(app_state,old_name,new_name);
				gtk_entry_set_text(GTK_ENTRY(app_state->command),"Command"); 
			}

        		break;

		case 1:
			break;
		default:
			break;
	}

	gtk_widget_destroy(dialog);
	return;
}



/* Rename file on server from list store */
void rename_serv_func_list(GtkWidget *menuitem,Appstate *app_state)
{
	GtkTreeView *treeview = GTK_TREE_VIEW(app_state->view);
	GtkTreeSelection *selection;
	GtkTreeModel *model;
	GtkTreeIter iter;
	
	GtkWidget *dialog;
	GtkWidget *entry;
	GtkWidget *entry1;
	GtkWidget *content_area;

	char user_input[MAXSZ];
	char *filename = (char *)malloc(MAXSZ);

	selection = gtk_tree_view_get_selection(treeview);
	if(gtk_tree_selection_get_selected(selection,&model,&iter))
	{	
		gtk_tree_model_get(model,&iter,COL_NAME,&filename,-1);
	}
	else
		return;
	
	dialog = gtk_dialog_new_with_buttons("Rename File(Server)",GTK_WINDOW(app_state->window),GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT,NULL,NULL,NULL,NULL);
	gtk_window_set_default_size(GTK_WINDOW(dialog),350,130);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Ok", 0);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Cancel", 1);

	content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	entry = gtk_entry_new(); 
	entry1 = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(entry1),filename); 
	gtk_entry_set_text(GTK_ENTRY(entry),"New name"); 
	gtk_entry_set_editable(GTK_ENTRY(entry1),FALSE);
	gtk_container_add(GTK_CONTAINER(content_area), entry1);
	gtk_container_add(GTK_CONTAINER(content_area), entry);

	gtk_widget_show_all(dialog);
	gint result = gtk_dialog_run(GTK_DIALOG(dialog));

	char *old_name = (char *)malloc(MAXSZ); // variable for entered text
	char *new_name = (char *)malloc(MAXSZ); // variable for entered text
   
	switch(result)
	{
		case 0:
			old_name = (char *)gtk_entry_get_text(GTK_ENTRY(entry1));
			new_name = (char *)gtk_entry_get_text(GTK_ENTRY(entry));

			if(strlen(old_name) == 0 || old_name == NULL || strlen(new_name) == 0 || new_name == NULL)//Invalid entry
			{
				GtkWidget *popup_window;
				GtkWidget *table;
				GtkWidget *label;
				GtkWidget *horiz_align;
	
				popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
				gtk_window_set_title (GTK_WINDOW (popup_window), "Input Error");
				gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
				gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
				gtk_window_set_decorated (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_widget_set_size_request (popup_window, 250, 60);
				gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
				gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);
		
				gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
				g_signal_connect (G_OBJECT (popup_window),
		               			    "focus-out-event",
		              		 G_CALLBACK (on_popup_focus_out),							NULL);
				table = gtk_table_new (2,2,FALSE);
				gtk_container_add(GTK_CONTAINER(popup_window),table);
				label = gtk_label_new("\t\tInvalid filename!");

				horiz_align = gtk_alignment_new(0,0,0,0);
    	
				gtk_container_add(GTK_CONTAINER(horiz_align),label);
				gtk_table_attach(GTK_TABLE(table),horiz_align,1,2,0,1,GTK_FILL,GTK_FILL,0,0);
	
				gtk_widget_show_all (popup_window);
				gtk_widget_grab_focus (popup_window);
			}
			else
			{
				sprintf(user_input,"rename %s %s",old_name,new_name);
				
				gtk_entry_set_text(GTK_ENTRY(app_state->command),user_input); 
				rename_serv(app_state,old_name,new_name);
				gtk_entry_set_text(GTK_ENTRY(app_state->command),"Command"); 
			}

        		break;

		case 1:
			break;
		default:
			break;
	}

	gtk_widget_destroy(dialog);
	return;
}

/* Download file from list store */
void view_popup_menu_download(GtkWidget *menuitem, Appstate *app_state)
{
	GtkTreeView *treeview = GTK_TREE_VIEW(app_state->view);
	GtkTreeSelection *selection;
	GtkTreeModel *model;
	GtkTreeIter iter;
	
	char user_input[MAXSZ];	
	
	char *filename = (char *)malloc(MAXSZ);
	char *perm = (char *)malloc(MAXSZ);
	
	bzero(user_input,MAXSZ);
	
	selection = gtk_tree_view_get_selection(treeview);
	if(gtk_tree_selection_get_selected(selection,&model,&iter))
	{	
		gtk_tree_model_get(model,&iter,COL_NAME,&filename,-1);
		gtk_tree_model_get(model,&iter,COL_PERM,&perm,-1);
		
		if(perm[0] != '-')
		{
			GtkWidget *popup_window;
			GtkWidget *table;
			GtkWidget *label;
			GtkWidget *horiz_align;
	
			/* Create window */
			popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
			gtk_window_set_title (GTK_WINDOW (popup_window), "Error");
			gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
			gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
			gtk_window_set_decorated (GTK_WINDOW (popup_window), TRUE);
			gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);
			gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
			gtk_widget_set_size_request (popup_window, 350, 60);
			gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
			gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);

			gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
			g_signal_connect (G_OBJECT (popup_window),
        		       			    "focus-out-event",
              			 		    G_CALLBACK (on_popup_focus_out),
                	   				NULL);
	
			table = gtk_table_new (2,2,FALSE);
			gtk_container_add(GTK_CONTAINER(popup_window),table);
			label = gtk_label_new("\tCannot download. Not a regular file!");

			horiz_align = gtk_alignment_new(0,0,0,0);
	    
			gtk_container_add(GTK_CONTAINER(horiz_align),label);
			gtk_table_attach(GTK_TABLE(table),horiz_align,1,2,0,1,GTK_FILL,GTK_FILL,0,0);
	
			gtk_widget_show_all (popup_window);
			gtk_widget_grab_focus (popup_window);
			
		}	
		
		else
		{
			sprintf(user_input,"get %s",filename);
			gtk_entry_set_text(GTK_ENTRY(app_state->command),user_input); 
			down_serv(app_state,user_input);
		    	gtk_entry_set_text(GTK_ENTRY(app_state->command),"Command"); 
		}
	}
}
 
/* Change directory on server from list store */ 
void view_popup_menu_change_dir(GtkWidget *menuitem, Appstate *app_state)
{
	GtkTreeView *treeview = GTK_TREE_VIEW(app_state->view);
	GtkTreeSelection *selection;
	GtkTreeModel *model;
	GtkTreeIter iter;

	char user_input[MAXSZ];	
	
	char *filename = (char *)malloc(MAXSZ);
	char *perm = (char *)malloc(MAXSZ);
	
	bzero(user_input,MAXSZ);

	selection = gtk_tree_view_get_selection(treeview);
	if(gtk_tree_selection_get_selected(selection,&model,&iter))
	{	
		gtk_tree_model_get(model,&iter,COL_NAME,&filename,-1);
		gtk_tree_model_get(model,&iter,COL_PERM,&perm,-1);
		
		if(perm[0] != 'd')
		{
			GtkWidget *popup_window;
			GtkWidget *table;
			GtkWidget *label;
			GtkWidget *horiz_align;
	
			/* Create window */
			popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
			gtk_window_set_title (GTK_WINDOW (popup_window), "Error");
			gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
			gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
			gtk_window_set_decorated (GTK_WINDOW (popup_window), TRUE);
			gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);
			gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
			gtk_widget_set_size_request (popup_window, 340, 60);
			gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
			gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);

			gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
			g_signal_connect (G_OBJECT (popup_window),
        		       			    "focus-out-event",
              			 		    G_CALLBACK (on_popup_focus_out),
                	   				NULL);
	
			table = gtk_table_new (2,2,FALSE);
			gtk_container_add(GTK_CONTAINER(popup_window),table);
			label = gtk_label_new("Cannot change directory. Not a directory!");

			horiz_align = gtk_alignment_new(0,0,0,0);
	    
			gtk_container_add(GTK_CONTAINER(horiz_align),label);
			gtk_table_attach(GTK_TABLE(table),horiz_align,1,2,0,1,GTK_FILL,GTK_FILL,0,0);
	
			gtk_widget_show_all (popup_window);
			gtk_widget_grab_focus (popup_window);
			
		}	
		else		
 		{
			sprintf(user_input,"cd %s",filename);
			gtk_entry_set_text(GTK_ENTRY(app_state->command),user_input); 
			serv_cd_dir(app_state,user_input);	
		    	gtk_entry_set_text(GTK_ENTRY(app_state->command),"Command");
		}
	}

}

/* Remove file on server from list store */ 
void view_popup_menu_remove(GtkWidget *menuitem, Appstate *app_state)
{
	GtkTreeView *treeview = GTK_TREE_VIEW(app_state->view);
	GtkTreeSelection *selection;
	GtkTreeModel *model;
	GtkTreeIter iter;

	char user_input[MAXSZ];	
	
	char *filename = (char *)malloc(MAXSZ);
	char *perm = (char *)malloc(MAXSZ);
	
	bzero(user_input,MAXSZ);

	selection = gtk_tree_view_get_selection(treeview);
	if(gtk_tree_selection_get_selected(selection,&model,&iter))
	{	
		gtk_tree_model_get(model,&iter,COL_NAME,&filename,-1);
		gtk_tree_model_get(model,&iter,COL_PERM,&perm,-1);
		
		if(perm[0] != '-')
		{
			GtkWidget *popup_window;
			GtkWidget *table;
			GtkWidget *label;
			GtkWidget *horiz_align;
	
			/* Create window */
			popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
			gtk_window_set_title (GTK_WINDOW (popup_window), "Error");
			gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
			gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
			gtk_window_set_decorated (GTK_WINDOW (popup_window), TRUE);
			gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);
			gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
			gtk_widget_set_size_request (popup_window, 350, 60);
			gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
			gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);

			gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
			g_signal_connect (G_OBJECT (popup_window),
        		       			    "focus-out-event",
              			 		    G_CALLBACK (on_popup_focus_out),
                	   				NULL);
	
			table = gtk_table_new (2,2,FALSE);
			gtk_container_add(GTK_CONTAINER(popup_window),table);
			label = gtk_label_new("\t  Cannot remove. Not a regular file!");

			horiz_align = gtk_alignment_new(0,0,0,0);
	    
			gtk_container_add(GTK_CONTAINER(horiz_align),label);
			gtk_table_attach(GTK_TABLE(table),horiz_align,1,2,0,1,GTK_FILL,GTK_FILL,0,0);
	
			gtk_widget_show_all (popup_window);
			gtk_widget_grab_focus (popup_window);
			
		}	
		else		
 		{
			sprintf(user_input,"rm %s",filename);
			serv_rmfile(app_state,user_input);	
		}
	}
}
 
/* Fill list store on client side */
void new_func_cli(Appstate *app_state)
{
	GtkTreeIter    iter;
	
	struct stat file_buff;
	fstat(app_state->temp_file_descriptor_cli,&file_buff);	

	GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(app_state->view1));
	if(file_buff.st_size > 0)
	{
		gtk_list_store_clear(GTK_LIST_STORE(model));
		lseek(app_state->temp_file_descriptor_cli,app_state->seek_cli,0);
		while(1)
		{
			char data[MAXSZ];
			char ch[2];
			char date[20];
			char temp_val1[20];
			char temp_val2[20];
			char temp_val3[20];
			char temp_val4[20];
			char temp_val5[400];
			char temp_val6[20];
			char temp_val7[20];
			char temp_val8[20];
			char temp_val9[100];
	
			bzero(data,MAXSZ);
	
			int bytes;
			int j = 0;
	
			while((bytes = read(app_state->temp_file_descriptor_cli,ch,1)) > 0)
			{
				if(ch[0] == '\n')
					break;
				data[j++] = ch[0];
			}
			data[j] = '\0';
	
			if(bytes == 0)
				break;
			sscanf(data,"%s %s %s %s %s %s %s %s %s",temp_val1,temp_val2,temp_val3,temp_val4,temp_val5,temp_val6,temp_val7,temp_val8,temp_val9);
			strcpy(date,temp_val6);
			strcat(date," ");
			strcat(date,temp_val7);
			strcat(date," ");
			strcat(date,temp_val8);
			/* Append a row and fill in some data */
			gtk_list_store_append (GTK_LIST_STORE(model), &iter);
			gtk_list_store_set (GTK_LIST_STORE(model), &iter,
	        		              COL_PERM, temp_val1,
	       			               COL_NAME, temp_val9,
	        		              COL_SIZE, atoi(temp_val5),
	        		              COL_MOD, date,
        	        		      -1);
		}
		app_state->seek_cli = file_buff.st_size;
	}
	gtk_tree_view_set_model (GTK_TREE_VIEW(app_state->view1), model);
}

/* Fill list store on server side */
void new_func(Appstate *app_state)
{
	GtkTreeIter    iter;
	
	struct stat file_buff;
	fstat(app_state->temp_file_descriptor,&file_buff);	

	GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(app_state->view));
	if(file_buff.st_size > 0)
	{
		gtk_list_store_clear(GTK_LIST_STORE(model));
		lseek(app_state->temp_file_descriptor,app_state->seek,0);
		while(1)
		{
			char data[MAXSZ];
			char ch[2];
			char date[20];
			char temp_val1[20];
			char temp_val2[20];
			char temp_val3[20];
			char temp_val4[20];
			char temp_val5[100];
			char temp_val6[20];
			char temp_val7[20];
			char temp_val8[20];
			char temp_val9[100];
	
			bzero(data,MAXSZ);
	
			int bytes;
			int j = 0;
	
			while((bytes = read(app_state->temp_file_descriptor,ch,1)) > 0)
			{
				if(ch[0] == '\n')
					break;
				
				data[j++] = ch[0];
			}
			data[j] = '\0';
	
			if(bytes == 0)
				break;

			sscanf(data,"%s %s %s %s %s %s %s %s %s",temp_val1,temp_val2,temp_val3,temp_val4,temp_val5,temp_val6,temp_val7,temp_val8,temp_val9);
			strcpy(date,temp_val6);
			strcat(date," ");
			strcat(date,temp_val7);
			strcat(date," ");
			strcat(date,temp_val8);
			/* Append a row and fill in some data */
			gtk_list_store_append (GTK_LIST_STORE(model), &iter);
			gtk_list_store_set (GTK_LIST_STORE(model), &iter,
	        		              COL_PERM, temp_val1,
	       			               COL_NAME, temp_val9,
	        		              COL_SIZE, atoi(temp_val5),
	        		              COL_MOD, date,
        	        		      -1);
		}
		app_state->seek = file_buff.st_size;
	}
	gtk_tree_view_set_model (GTK_TREE_VIEW(app_state->view), model);
}

/* Pop menu on client side in list store */
void view_popup_menu_cli(GtkWidget *treeview, GdkEventButton *event, Appstate *app_state)
{
	GtkWidget *menu, *down, *delete, *rename, *change_dir;
 
	GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(app_state->view1));
	GtkTreeIter iter;

	if(gtk_tree_model_get_iter_first(model,&iter) == TRUE)
	{
		menu = gtk_menu_new();
 
		down = gtk_menu_item_new_with_label("Upload");
		delete = gtk_menu_item_new_with_label("Remove file");
		rename = gtk_menu_item_new_with_label("Rename");
		change_dir = gtk_menu_item_new_with_label("Move to Directory");
	 
		g_signal_connect(down, "activate",(GCallback) view_popup_menu_upload_cli, app_state);
		g_signal_connect(rename, "activate",(GCallback)view_popup_menu_rename_cli, app_state);
		g_signal_connect(delete, "activate",(GCallback) view_popup_menu_remove_cli, app_state);
		g_signal_connect(change_dir, "activate",(GCallback) view_popup_menu_change_dir_cli, app_state);
	 
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), down);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), delete);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), rename);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), change_dir);
	 
		gtk_widget_show_all(menu);
 
	 	gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL,(event != NULL) ? event->button : 0,gdk_event_get_time((GdkEvent*)event));
	}
}
 
/* Action on pressing the button */ 
gboolean view_onButtonPressed_cli(GtkWidget *treeview, GdkEventButton *event,Appstate *app_state)
{
	/* single click with the right mouse button? */
	if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3)
	{
		if (1)
		{
			GtkTreeSelection *selection;
			selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
 
			if (gtk_tree_selection_count_selected_rows(selection)  <= 1)
			{
				GtkTreePath *path;
 
				/* Get tree path for row that was clicked */
				if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(treeview),(gint) event->x,(gint) event->y,&path, NULL, NULL, NULL))
				{
					gtk_tree_selection_unselect_all(selection);
					gtk_tree_selection_select_path(selection, path);
					gtk_tree_path_free(path);
				}
			}
		} /* end of optional bit */
 
	view_popup_menu_cli(treeview, event, app_state);
 
	return TRUE; /* we handled this */
	}
 
	return FALSE; /* we did not handle this */
}
 
 
gboolean view_onPopupMenu_cli(GtkWidget *treeview, Appstate *app_state)
{	
	view_popup_menu_cli(treeview, NULL, app_state);
 
	return TRUE; /* we handled this */
} 
 
/* Pop menu on server side in list store */
void view_popup_menu (GtkWidget *treeview, GdkEventButton *event, Appstate *app_state)
{
	GtkWidget *menu, *down, *delete, *rename, *change_dir;
 
	GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(app_state->view));
	GtkTreeIter iter;

	if(gtk_tree_model_get_iter_first(model,&iter) == TRUE)
	{
		menu = gtk_menu_new();
 
		down = gtk_menu_item_new_with_label("Download");
		delete = gtk_menu_item_new_with_label("Remove file");
		rename = gtk_menu_item_new_with_label("Rename");
		change_dir = gtk_menu_item_new_with_label("Move to Directory");
	 
		g_signal_connect(down, "activate",(GCallback) view_popup_menu_download, app_state);
		g_signal_connect(rename, "activate",(GCallback)rename_serv_func_list, app_state);
		g_signal_connect(delete, "activate",(GCallback) view_popup_menu_remove, app_state);
		g_signal_connect(change_dir, "activate",(GCallback) view_popup_menu_change_dir, app_state);
	 
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), down);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), delete);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), rename);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), change_dir);
	 
		gtk_widget_show_all(menu);
 
	 	gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL,(event != NULL) ? event->button : 0,gdk_event_get_time((GdkEvent*)event));
	}
}
 
/* Action on pressing button */
gboolean view_onButtonPressed (GtkWidget *treeview, GdkEventButton *event,Appstate *app_state)
{
	/* single click with the right mouse button? */
	if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3)
	{
		if (1)
		{
			GtkTreeSelection *selection;
			selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
 
			if (gtk_tree_selection_count_selected_rows(selection)  <= 1)
			{
				GtkTreePath *path;
 
				/* Get tree path for row that was clicked */
				if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(treeview),(gint) event->x,(gint) event->y,&path, NULL, NULL, NULL))
				{
					gtk_tree_selection_unselect_all(selection);
					gtk_tree_selection_select_path(selection, path);
					gtk_tree_path_free(path);
				}
			}
		} /* end of optional bit */
 
	view_popup_menu(treeview, event, app_state);
 
	return TRUE; /* we handled this */
	}
 
	return FALSE; /* we did not handle this */
}
 
 
gboolean view_onPopupMenu (GtkWidget *treeview, Appstate *app_state)
{	
	view_popup_menu(treeview, NULL, app_state);
 
	return TRUE; /* we handled this */
} 
 
static GtkTreeModel *create_and_fill_model (void)
{
	GtkListStore  *store;
	
	store = gtk_list_store_new (NUM_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_UINT, G_TYPE_STRING);

	return GTK_TREE_MODEL (store);
}

/* Client side */
static GtkWidget *create_view_and_model1 (Appstate *app_state)
{
	GtkCellRenderer     *renderer;
	GtkTreeModel        *model;
	GtkWidget           *view;
 
	view = gtk_tree_view_new ();
 
	g_signal_connect(view, "button-press-event", (GCallback) view_onButtonPressed_cli, app_state);
	g_signal_connect(view, "popup-menu", (GCallback) view_onPopupMenu_cli, app_state);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
        	                                       -1,      
        	                                       "Permissions",  
        	                                       renderer,
        	                                       "text", COL_PERM,
        	                                       NULL);
 
 
	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
        	                                       -1,      
        	                                       "Filename",  
        	                                       renderer,
        	                                       "text", COL_NAME,
        	                                       NULL);
 	
	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                         	                      -1,      
                         	                      "Filesize",  
                         	                      renderer,
        	                                       "text", COL_SIZE,
			                               NULL);
 
	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
	                                               -1,      
	                                               "Last Modified",  
	                                               renderer,
	                                               "text", COL_MOD,
	                                               NULL);
	
 
	model = create_and_fill_model ();
 
	gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);
 
	/* The tree view has acquired its own reference to the
	*  model, so we can drop ours. That way the model will
	*  be freed automatically when the tree view is destroyed */
 
	g_object_unref (model);
 
	return view;
}
 
/* Server side */ 
static GtkWidget *create_view_and_model (Appstate *app_state)
{
	GtkCellRenderer     *renderer;
	GtkTreeModel        *model;
	GtkWidget           *view;
 
	view = gtk_tree_view_new ();
 
	g_signal_connect(view, "button-press-event", (GCallback) view_onButtonPressed, app_state);
	g_signal_connect(view, "popup-menu", (GCallback) view_onPopupMenu, app_state);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
        	                                       -1,      
        	                                       "Permissions",  
        	                                       renderer,
        	                                       "text", COL_PERM,
        	                                       NULL);
 
 
	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
        	                                       -1,      
        	                                       "Filename",  
        	                                       renderer,
        	                                       "text", COL_NAME,
        	                                       NULL);
 	
	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                         	                      -1,      
                         	                      "Filesize",  
                         	                      renderer,
        	                                       "text", COL_SIZE,
			                               NULL);
 
	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
	                                               -1,      
	                                               "Last Modified",  
	                                               renderer,
	                                               "text", COL_MOD,
	                                               NULL);
	
 
	model = create_and_fill_model ();
 
	gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);
 
	/* The tree view has acquired its own reference to the
	*  model, so we can drop ours. That way the model will
	*  be freed automatically when the tree view is destroyed */
 
	g_object_unref (model);
 
	return view;
}
 
/* Focus out popup window */
gboolean on_popup_focus_out (GtkWidget *widget,GdkEventFocus *event,gpointer data)
{
	gtk_widget_destroy (widget);
	return TRUE;
}

/* Popup window if a command is running and user tries to execute another command */
void already_running(Appstate *app_state)
{

	GtkWidget *popup_window;
	GtkWidget *table;
	GtkWidget *label;
	GtkWidget *horiz_align;
	
	/* Create window */
	popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (popup_window), "Error");
	gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
	gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
	gtk_window_set_decorated (GTK_WINDOW (popup_window), TRUE);
	gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);
	gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
	gtk_widget_set_size_request (popup_window, 390, 60);
	gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
	gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);

	gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
	g_signal_connect (G_OBJECT (popup_window),
               			    "focus-out-event",
              		 		    G_CALLBACK (on_popup_focus_out),
                   				NULL);
	
	table = gtk_table_new (2,2,FALSE);
	gtk_container_add(GTK_CONTAINER(popup_window),table);
	label = gtk_label_new("Another command running. Please wait for completion!");

	horiz_align = gtk_alignment_new(0,0,0,0);
    
	gtk_container_add(GTK_CONTAINER(horiz_align),label);
	gtk_table_attach(GTK_TABLE(table),horiz_align,1,2,0,1,GTK_FILL,GTK_FILL,0,0);
	
	gtk_widget_show_all (popup_window);
	gtk_widget_grab_focus (popup_window);
	return;
}

/* Pops up window if user tries to execute a command without setting up a connection with server */
void no_connection(Appstate *app_state)
{

	GtkWidget *popup_window;
	GtkWidget *table;
	GtkWidget *label;
	GtkWidget *horiz_align;
	

	popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (popup_window), "Connection error");
	gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
	gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
	gtk_window_set_decorated (GTK_WINDOW (popup_window), TRUE);
	gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);
	gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
	gtk_widget_set_size_request (popup_window, 350, 60);
	gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
	gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);

	gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
	g_signal_connect (G_OBJECT (popup_window),
               			    "focus-out-event",
              		 		    G_CALLBACK (on_popup_focus_out),
                   				NULL);
	
	table = gtk_table_new (2,2,FALSE);
	gtk_container_add(GTK_CONTAINER(popup_window),table);
	label = gtk_label_new("No connection. Please establish connection first!");

	horiz_align = gtk_alignment_new(0,0,0,0);
    
	gtk_container_add(GTK_CONTAINER(horiz_align),label);
	gtk_table_attach(GTK_TABLE(table),horiz_align,1,2,0,1,GTK_FILL,GTK_FILL,0,0);
	
	gtk_widget_show_all (popup_window);
	gtk_widget_grab_focus (popup_window);
	return;
}

/* Help button: Pops up window with details about using the program */
void help_func(GtkWidget *widget,Appstate *app_state)
{

	GtkWidget *popup_window;
	GtkWidget *table;
	GtkWidget *frame;
	GtkWidget *label;

	popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (popup_window), "Help");
	gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
	gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
	gtk_window_set_decorated (GTK_WINDOW (popup_window), FALSE);
	gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);
	gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
	gtk_widget_set_size_request (popup_window, 395, 320);
	gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
	gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);

	gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
	g_signal_connect (G_OBJECT (popup_window),
               			    "focus-out-event",
              		 		    G_CALLBACK (on_popup_focus_out),
                   				NULL);
	
	table = gtk_table_new (2,2,FALSE);
	gtk_container_add(GTK_CONTAINER(popup_window),table);

   	frame =  gtk_frame_new("\t\t\t\t\tHappy to help you!\t     \t\t\t\t\n");
	label = gtk_label_new("\tCreate directory: \tmkdir directory-name\n\tRemove directory: \trmdir directory-name\n\tDownload: \tget file-name\n\tUpload: \tput file-name\n\tRename: \trename file-name new-name\n\tChange directory: \tcd file-path\n\tWorking directory: \tpwd\n\tRemove file: \trm file-name\n\tList files: \tls -l\n\n\n\tNOTE: for client side,use '!' before the command.\n\tFor example: !pwd (To get working directory on\n\tclient side)\n");
	gtk_label_set_line_wrap(GTK_LABEL(label),TRUE);
	gtk_container_add(GTK_CONTAINER(frame),label);
	gtk_table_attach(GTK_TABLE(table),frame,1,2,0,1,GTK_FILL,GTK_FILL,0,0);


	gtk_widget_show_all (popup_window);
	gtk_widget_grab_focus (popup_window);
	return;
}

/* About us */
void about_us_func(GtkWidget *widget,Appstate *app_state)
{

	GtkWidget *popup_window;
	GtkWidget *table;
	GtkWidget *frame;
	GtkWidget *label;

	popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (popup_window), "About Us");
	gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
	gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
	gtk_window_set_decorated (GTK_WINDOW (popup_window), FALSE);
	gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);
	gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
	gtk_widget_set_size_request (popup_window, 464, 200);
	gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
	gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);

	gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
	g_signal_connect (G_OBJECT (popup_window),
               			    "focus-out-event",
              		 		    G_CALLBACK (on_popup_focus_out),
                   				NULL);
	
	table = gtk_table_new (2,2,FALSE);
	gtk_container_add(GTK_CONTAINER(popup_window),table);

   	frame =  gtk_frame_new("\t\t\t\t\t\t\t  About ST-FTP\t     \t\t\t\t\t\t");
	label = gtk_label_new("\nST-FTP is a multithreaded FTP client which splits the file, to be downloaded, in chunks and then joins them together.\n\n\nCompleted by:\nRohit Takhar\nUnder the guidance of Professor Khushil Saini.\n");
	gtk_label_set_line_wrap(GTK_LABEL(label),TRUE);
	gtk_container_add(GTK_CONTAINER(frame),label);
	gtk_table_attach(GTK_TABLE(table),frame,1,2,0,1,GTK_FILL,GTK_FILL,0,0);


	gtk_widget_show_all (popup_window);
	gtk_widget_grab_focus (popup_window);
	return;
}


/*Get home directory of user executing program */
char * find_home_dir(char *file)
{
	struct passwd *pw;
	char *sudo_uid = getenv("SUDO_UID");
	pw = getpwuid(atoi(sudo_uid));
	
	return pw->pw_dir;

}

/* Remove directory on client side */
void rmdir_cli(Appstate *app_state,char *user_input)
{

	if(app_state->status == 0)//Connection is not established
	{
		no_connection(app_state);//Call popup window function
		return;
	}
	
	if(app_state->running == 1)//Command is already executing
	{
		already_running(app_state);//Call popup window function
		return;
	}
	
	char cwd[MAXSZ];
	bzero(cwd,MAXSZ);
	/* Set 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
	app_state->running = 1;

	int dir_check = rmdir(user_input + 7);//Remove directory
	
	if(dir_check == -1)
		sprintf(buff,"Error: %s\n\n",strerror(errno));
	else
		sprintf(buff,"Directory successfully removed\n\n");
	
	print_buff(app_state);
	
	/* Reset 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;
	getcwd(cwd,MAXSZ);
	ls_l_dir_view(app_state,cwd);
	new_func_cli(app_state);

	return;
}

/* Remove directory on server */
void rmdir_serv(Appstate *app_state,char *user_input)
{
	if(app_state->status == 0)//Connection is not established
	{
		no_connection(app_state);//Call popup window function
		return;
	}
	
	if(app_state->running == 1)//Command is already executing
	{
		already_running(app_state);//Call popup window function
		return;
	}
	
	char *address = (char *)malloc(MAXSZ);
	
	address = (char *)gtk_entry_get_text(GTK_ENTRY(app_state->entry));
	char *argv = address;	
	/* Set 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
	app_state->running = 1;
	
	int no_of_bytes;
	int ret_value;

	char message_to_server[MAXSZ];
	char message_from_server[MAXSZ];

	bzero(message_from_server,MAXSZ);
	bzero(message_to_server,MAXSZ);

	sprintf(message_to_server,"RMD %s\r\n",user_input + 6);
	sprintf(buff,"Command: RMD %s\nResponse: ",user_input + 6);	
	print_buff(app_state);
	
	/* Send request to server to remove specified directory */
	send(app_state->sockfd,message_to_server,strlen(message_to_server),0);
	while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0 )
	{
		message_from_server[no_of_bytes] = '\0';
		sprintf(buff,"%s",message_from_server);
		print_buff(app_state);
				

		if(strstr(message_from_server,"250 ") > 0 || strstr(message_from_server,"530 ") > 0 || strstr(message_from_server,"500 ") > 0|| strstr(message_from_server,"501 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"502 ") > 0 || strstr(message_from_server,"550 ") > 0)
			break;	
	}
	sprintf(buff,"\n");
	print_buff(app_state);
	
	ret_value = list_content_view(argv,"ls -l",app_state);	
	if(ret_value == 0)
		new_func(app_state);

	/* Reset 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;

	return;
}

/* Create directory on client */
void mkdir_cli(Appstate *app_state,char *user_input)
{
	if(app_state->status == 0)//Connection is not established
	{
		no_connection(app_state);//Call popup window function
		return;
	}
	
	if(app_state->running == 1)//Command is already executing
	{
		already_running(app_state);//Call popup window function
		return;
	}
	
	char cwd[MAXSZ];
	bzero(cwd,MAXSZ);
	/* Set 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
	app_state->running = 1;

	int dir_check = mkdir(user_input + 7,0755);//Create directoty command
	
	if(dir_check == -1)
		sprintf(buff,"Error: %s\n\n",strerror(errno));
	else
		sprintf(buff,"Directory successfully created\n\n");
	
	print_buff(app_state);
	
	getcwd(cwd,MAXSZ);
	ls_l_dir_view(app_state,cwd);
	new_func_cli(app_state);
	/* Reset 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;
	
	return;
}

/* Rename file on client */
void rename_cli(Appstate *app_state,char *old_name, char *new_name)
{
	if(app_state->status == 0)//Connection not established
	{
		no_connection(app_state);//Call popup window function
		return;
	}
	
	if(app_state->running == 1)//Command aleady executing
	{
		already_running(app_state);//Call popup window function
		return;
	}
	
	char cwd[MAXSZ];

	getcwd(cwd,MAXSZ);

	if(access(new_name,F_OK) == 0)
	{
		sprintf(buff,"Error: File cannot be renamed. Filename already present. Try another name!\n\n");
		print_buff(app_state);
		return;
	}

	/* Set 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
	app_state->running = 1;

	if(rename(old_name,new_name) == 0)
	{
		sprintf(buff,"File renamed successfully!\n\n");
		print_buff(app_state);

	}
	else
	{
		sprintf(buff,"Error: %s\n\n",strerror(errno));
		print_buff(app_state);
		
	}

	ls_l_dir_view(app_state,cwd);	
	new_func_cli(app_state);
	/* Reset 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;
	
	return;
}

/* Rename file on server*/
void rename_serv(Appstate *app_state,char *old_name, char *new_name)
{
	if(app_state->status == 0)//Connection not established
	{
		no_connection(app_state);//Call popup window function
		return;
	}
	
	if(app_state->running == 1)//Command already running
	{
		already_running(app_state);//Call popup window function
		return;
	}
	
	/* Set 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
	app_state->running = 1;
	
	int no_of_bytes;
	int temp = 0;
	int ret_value;
	
	char message_to_server[MAXSZ];
	char message_from_server[MAXSZ];
	
	char *address= (char *)malloc(MAXSZ);
	
	address = (char *)gtk_entry_get_text(GTK_ENTRY(app_state->entry));

	bzero(message_from_server,MAXSZ);
	bzero(message_to_server,MAXSZ);

	sprintf(message_to_server,"RNFR %s\r\n",old_name);
	sprintf(buff,"Command: RNFR %s\nResponse: ",old_name);
	print_buff(app_state);
	
	/* Send command to server */	
	send(app_state->sockfd,message_to_server,strlen(message_to_server),0);
	while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0 )
	{
		message_from_server[no_of_bytes] = '\0';
		
		if(strncmp(message_from_server,"550",3) == 0)/* RNFR fails*/
		{
			temp = 1;	
		}

		sprintf(buff,"%s",message_from_server);
		print_buff(app_state);
				
		if(strstr(message_from_server,"350 ") > 0 || strstr(message_from_server,"450 ") > 0 || strstr(message_from_server,"530 ") > 0 || strstr(message_from_server,"500 ") > 0|| strstr(message_from_server,"501 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"502 ") > 0 || strstr(message_from_server,"550 ") > 0)
			break;	
	}
	sprintf(buff,"\n");
	print_buff(app_state);
	
			
	if(temp  == 1)
	{	
		/* Renaming failed. Reset 'running' variable */
		app_state->running = 0;
		gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	
		return;
	}
	sprintf(message_to_server,"RNTO %s\r\n",new_name);
	sprintf(buff,"Command: RNTO %s\nResponse: ",new_name);
	print_buff(app_state);
	

	send(app_state->sockfd,message_to_server,strlen(message_to_server),0);
	while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0 )
		
	{
		message_from_server[no_of_bytes] = '\0';
		sprintf(buff,"%s",message_from_server);
		print_buff(app_state);
				
				
		if(strstr(message_from_server,"550 ") > 0 || strstr(message_from_server,"553 ") > 0 ||strstr(message_from_server,"250 ") > 0 || strstr(message_from_server,"532 ") > 0 || strstr(message_from_server,"530 ") > 0 || strstr(message_from_server,"500 ") > 0|| strstr(message_from_server,"501 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"502 ") > 0 || strstr(message_from_server,"503 ") > 0)
			break;

	}
	sprintf(buff,"\n");
	print_buff(app_state);
	
	ret_value = list_content_view(address,"ls -l",app_state);	
	if(ret_value == 0)
		new_func(app_state);
	
	/* Reset 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;
	
	return;
}

/* Download file from server */
void down_serv(Appstate *app_state,char *user_input)
{
	if(app_state->status == 0)//Connection not established
	{
		no_connection(app_state);//Call popup window function
		return;
	}

	if(app_state->running == 1)//Command already running
	{
		already_running(app_state);//Call popup window function
		return;
	}

	char cwd[MAXSZ];
	bzero(cwd,MAXSZ);

	char *address = (char *)malloc(MAXSZ);
	
	/* Get IP Address of server from IP Address entry */
	address = (char *)gtk_entry_get_text(GTK_ENTRY(app_state->entry));
	char *argv = address;	

	char *home_dir= find_home_dir(argv);

	clock_t start;
	clock_t end;
	double cpu_time;
	
	/* Set 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
	app_state->running = 1;

	start = clock();//Start clock
	get_content(argv,user_input,app_state,home_dir);//Call function to get files
	end = clock();//Stop clock
	cpu_time = ((double)(end - start))/CLOCKS_PER_SEC;//Calculate CPU time
	sprintf(buff,"Time taken %lf\n\n",cpu_time);
	print_buff(app_state);

	/* Reset 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;
	getcwd(cwd,MAXSZ);
	ls_l_dir_view(app_state,cwd);
	new_func_cli(app_state);
	return;
}

/* Change directory on server */
void serv_cd_dir(Appstate *app_state,char *user_input)
{
	int no_of_bytes;
	int temp;
	int j;
	int count;
	int ret_value;

	if(app_state->status == 0)//Connection not established
	{
		no_connection(app_state);//Call popup window function
		return;
	}

	if(app_state->running == 1)//Command already running
	{
		already_running(app_state);//Call popup window function
		return;
	}

	/* Set 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
	app_state->running = 1;

	char message_to_server[MAXSZ];
	char message_from_server[MAXSZ];
	char serv_curr_dir[MAXSZ];
	
	char *address = (char *)malloc(MAXSZ);
	
	address = (char *)gtk_entry_get_text(GTK_ENTRY(app_state->entry));
	char *argv = address;	
	
	bzero(message_to_server,MAXSZ);
	bzero(message_from_server,MAXSZ);
	bzero(serv_curr_dir,MAXSZ);
	
	sprintf(message_to_server,"CWD %s\r\n",user_input + 3);
	sprintf(buff,"Command: CWD %s\nResponse: ",user_input+3);
	print_buff(app_state);
	
	/* Requesting to change directory */
	send(app_state->sockfd,message_to_server,strlen(message_to_server),0);	
		
	while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0)
	{
		message_from_server[no_of_bytes] = '\0';
		sprintf(buff,"%s",message_from_server);
		print_buff(app_state);
				
		if(strstr(message_from_server,"530 ") > 0 || strstr(message_from_server,"250 ") > 0 || strstr(message_from_server,"500 ") > 0 || strstr(message_from_server,"501 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"502 ") > 0 || strstr(message_from_server,"550 ") > 0)
			break;
	}
	
	bzero(message_to_server,MAXSZ);
	bzero(message_from_server,MAXSZ);
	bzero(buff,MAXSZ1);
	
	/* Getting working directory to display after changing directory */
	sprintf(message_to_server,"PWD\r\n");
	
	send(app_state->sockfd,message_to_server,strlen(message_to_server),0);
		
	while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0)
	{
		message_from_server[no_of_bytes] = '\0';
		while(gtk_events_pending())
			gtk_main_iteration();
				
		if(strstr(message_from_server,"257 ") > 0 || strstr(message_from_server,"500 ") > 0 || strstr(message_from_server,"501 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"502 ") > 0 || strstr(message_from_server,"550 ") > 0)
			break;
	}
	
	temp = 0;
	count = 0;
	j = 0;
	while(count != 2)
	{
		if(message_from_server[temp] == '\"')
		{
			count++;
			temp++;	
		}
	
		if(count == 2)
			break;
	
		if(count == 1)
			serv_curr_dir[j++] = message_from_server[temp++];
		else
			temp++;
		
		while(gtk_events_pending())
			gtk_main_iteration();
	}	
	serv_curr_dir[j] = '\0';
		
	gtk_entry_set_text(GTK_ENTRY(app_state->entry_dir),serv_curr_dir);
	sprintf(buff,"You are in directory: \"%s\"\n\n",serv_curr_dir);
	print_buff(app_state);
	
	ret_value = list_content_view(argv,"ls -l",app_state);	
	if(ret_value == 0)
		new_func(app_state);
	/* Reset 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;
	
	return;
}

/* Change directory on client side */
void cli_cd_dir(Appstate *app_state,char *user_input)
{
	
	if(app_state->status == 0)//Connection not established
	{
		no_connection(app_state);//Call popup window function
		return;
	}

	if(app_state->running == 1)//Command already running
	{
		already_running(app_state);//Call popup window function
		return;
	}

	char cwd[MAXSZ];	
	
	/* Set 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
	app_state->running = 1;
	
	if(chdir(user_input + 4) == 0)//Change directory
	{
		getcwd(cwd,MAXSZ);//Get current working directory after changing directory
		sprintf(buff,"Directory successfully changed\nYou are in directory: \"%s\"\n\n",cwd);
	}			
	else
	{
		sprintf(buff,"Error: %s\n\n",strerror(errno));
	}
	
	print_buff(app_state);
	
	/* Reset 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;
	ls_l_dir_view(app_state,cwd);
	new_func_cli(app_state);
	gtk_entry_set_text(GTK_ENTRY(app_state->entry_dir_cli),cwd);
		
	return;
}

/*Remove file on client side*/
void cli_rmfile(Appstate *app_state,char *user_input)
{

	if(app_state->status == 0)//Connection not established
	{
		no_connection(app_state);//Call popup window function
		return;
	}

	if(app_state->running == 1)//Command already running
	{
		already_running(app_state);//Call popup window function
		return;
	}
	
	char cwd[MAXSZ];
	char file_path[MAXSZ];
	
	bzero(cwd,MAXSZ);
	bzero(file_path,MAXSZ);

	getcwd(cwd,MAXSZ);//Get current working directory
	sprintf(file_path,"%s/%s",cwd,user_input + 4);

	/* Set 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
	app_state->running = 1;
	
	if(unlink(file_path) == 0)//Remove file
	{
		sprintf(buff,"File succesfully removed!\n\n");
	}			
	else
	{
		sprintf(buff,"Error: %s\n\n",strerror(errno));
	}
	
	print_buff(app_state);
	ls_l_dir_view(app_state,cwd);
	new_func_cli(app_state);
	/* Reset 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;
	
	return;

}

/* List files on client */
void ls_cli_func(GtkWidget *widget,Appstate *app_state)
{
	if(app_state->status == 0)//Connection not established
	{
		no_connection(app_state);//Call popup window function
		return;
	}

	if(app_state->running == 1)//Command already running
	{
		already_running(app_state);//Call popup window function
		return;
	}
	
	char working_dir[MAXSZ];

	/* Set 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
	app_state->running = 1;
	gtk_entry_set_text(GTK_ENTRY(app_state->command),"!ls -l"); 

	getcwd(working_dir,MAXSZ);	
	ls_l_dir(app_state,working_dir);//Call function to list files

	/* Reset 'running' variable */
	app_state->running = 0;
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	gtk_entry_set_text(GTK_ENTRY(app_state->command),"Command"); 
	
	return;

}

/* List files on server */
void ls_ser_func(GtkWidget *widget,Appstate *app_state)
{
	if(app_state->status == 0)//Connection not established
	{
		no_connection(app_state);
		return;
	}

	if(app_state->running == 1)//Command already running
	{
		already_running(app_state);
		return;
	}

	/* Set 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
	app_state->running = 1;

	char user_input[]="ls -l";
	char *address = (char *)malloc(MAXSZ);
	
	address = (char *)gtk_entry_get_text(GTK_ENTRY(app_state->entry));
	char *argv = address;	

	gtk_entry_set_text(GTK_ENTRY(app_state->command),user_input); 

	list_content(argv,user_input,app_state);	
	
	/* Reset 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;
	gtk_entry_set_text(GTK_ENTRY(app_state->command),"Command"); 
	
	return;

}

/* Remove file on server */
void serv_rmfile(Appstate *app_state,char *user_input)
{

	int no_of_bytes;
	int ret_value;

	if(app_state->status == 0)//Connection not established
	{
		no_connection(app_state);
		return;
	}

	if(app_state->running == 1)//Command already running
	{
		already_running(app_state);
		return;
	}

	char *address = (char *)malloc(MAXSZ);
	
	address = (char *)gtk_entry_get_text(GTK_ENTRY(app_state->entry));
	/* Set 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
	app_state->running = 1;

	char message_to_server[MAXSZ];
	char message_from_server[MAXSZ];
	
	bzero(message_to_server,MAXSZ);
	bzero(message_from_server,MAXSZ);
	
	sprintf(message_to_server,"DELE %s\r\n",user_input + 3);
	sprintf(buff,"Command: DELE %s\nResponse: ",user_input+3);	
	print_buff(app_state);
	
	/* Requesting server to delete specified file */
	send(app_state->sockfd,message_to_server,strlen(message_to_server),0);
	while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0 )
	{
		message_from_server[no_of_bytes] = '\0';
		sprintf(buff,"%s\n",message_from_server);
		print_buff(app_state);
				
		if(strstr(message_from_server,"250 ") > 0 || strstr(message_from_server,"450 ") > 0 || strstr(message_from_server,"530 ") > 0 || strstr(message_from_server,"500 ") > 0|| strstr(message_from_server,"501 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"502 ") > 0 || strstr(message_from_server,"550 ") > 0)
			break;
			
	}

	/* Reset 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;
	
	ret_value = list_content_view(address,"ls -l",app_state);	
	if(ret_value == 0)
		new_func(app_state);

	return;
}

/* This functions includes the case when commands are executed manually i.e without GUI */
void run_command(GtkWidget *widget, Appstate *app_state)
{
	if(app_state->status == 0)//Connectionnot established
	{
		no_connection(app_state);
		return;
	}

	if(app_state->running == 1)//Command already running
	{
		already_running(app_state);
		return;
	}

	char *user_input = (char *)malloc(MAXSZ);
	char *address = (char *)malloc(MAXSZ);
	
	address = (char *)gtk_entry_get_text(GTK_ENTRY(app_state->entry));
	char *argv = address;	
	int count;
	int temp;
	int j = 0;	

	clock_t start,end;
	double cpu_time;

	char message_from_server[MAXSZ];/* message from server*/
	char message_to_server[MAXSZ];/* message to server */
	char dir[MAXSZ];/* directory name */
	char working_dir[MAXSZ];
	char old_name[MAXSZ];
	char new_name[MAXSZ];
	char file_name[MAXSZ];
	char cwd[MAXSZ];
	char serv_curr_dir[MAXSZ];

	char *home_dir= find_home_dir(argv);//Get home directory of user

	int no_of_bytes;
	int ret_value;	

	/* Initialise strings */
	bzero(user_input,MAXSZ);
	bzero(message_to_server,MAXSZ);
	bzero(message_from_server,MAXSZ);
	bzero(dir,MAXSZ);
	bzero(working_dir,MAXSZ);
	bzero(file_name,MAXSZ);
	bzero(old_name,MAXSZ);
	bzero(new_name,MAXSZ);
	bzero(cwd,MAXSZ);
	bzero(serv_curr_dir,MAXSZ);

	/* Read user input */
	user_input = (char *)gtk_entry_get_text(GTK_ENTRY(app_state->command));
 
	/* User wants to exit */
	if(strcmp(user_input,"exit") == 0 || strcmp(user_input,"quit") == 0 || strcmp(user_input,"bye") == 0)
	{
		app_state->running = 1;
		gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
		sprintf(message_to_server,"QUIT\r\n");
		sprintf(buff,"Command: QUIT\nResponse: ");	
		print_buff(app_state);
		
		/* Send message to server */
		send(app_state->sockfd,message_to_server,strlen(message_to_server),0);
		while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0)
		{
			message_from_server[no_of_bytes] = '\0';
			sprintf(buff,"%s",message_from_server);
			print_buff(app_state);

			if(strstr(message_from_server,"221 ") > 0 || strstr(message_from_server,"500 ") > 0)	
				break;
		}
		sprintf(buff,"\n");
		print_buff(app_state);
		close(app_state->sockfd);
		app_state->status = 0;
		app_state->running = 0;
		gtk_button_set_label(GTK_BUTTON(app_state->button),"Connect");
		gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
		return;
	}

	/* Change directory on client side */
	if(strncmp(user_input,"!cd ",4) == 0 || strcmp(user_input,"!cd") == 0)
	{
		
		app_state->running = 1;
		gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
		if(chdir(user_input + 4) == 0)
		{
			sprintf(buff,"Directory successfully changed\n\n");
		}			
		else
		{
			sprintf(buff,"Error: %s\n",strerror(errno));
		}
	
		print_buff(app_state);
		getcwd(cwd,MAXSZ);
		ls_l_dir_view(app_state,cwd);
		new_func_cli(app_state);
		gtk_entry_set_text(GTK_ENTRY(app_state->entry_dir_cli),cwd);
	
	}

	/* Current working directory on client side */
	if(strncmp(user_input,"!pwd ",5) == 0 || strcmp(user_input,"!pwd") == 0)
	{
		app_state->running = 1;
		gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
		getcwd(working_dir,MAXSZ);	
		sprintf(buff,"You are in directory: \"%s\"\n\n",working_dir);
		print_buff(app_state);
	
	}	

	/* List files with details in current working directory on client side */
	if(strncmp(user_input,"!ls -l ",7) == 0 || strcmp(user_input,"!ls -l") == 0)
	{
		app_state->running = 1;
		gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
		getcwd(working_dir,MAXSZ);	
		ls_l_dir(app_state,working_dir);
		app_state->running = 0;
		gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
		return;
	}
	
	/* List files in current working directory on client side */	
	if(strncmp(user_input,"!ls ",4) == 0 || strcmp(user_input,"!ls") == 0)
	{
		app_state->running = 1;
		gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
		getcwd(working_dir,MAXSZ);	
		ls_dir(app_state,working_dir);
	}

	/* Create directory on client side */
	if(strncmp(user_input,"!mkdir ",7) == 0)
	{
		mkdir_cli(app_state,user_input);
	}		

	/* Remove directory on client side */
	if(strncmp(user_input,"!rmdir ",7) == 0)
	{
		rmdir_cli(app_state,user_input);
	}		

	/* Rename file on client */	
	if(strncmp(user_input,"!rename ",8) == 0)
	{
		/* parse user input to get old file name and new file name */
		count = sscanf(user_input,"%s %s %s",message_to_server,old_name,new_name);
	
		if(count != 3)
		{		
			sprintf(buff,"Error: rename expects two arguments\n");
			print_buff(app_state);
			app_state->running = 0;
			gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
			return;
		}			
		rename_cli(app_state,old_name,new_name);
	}

	if(strncmp(user_input,"!rm ",4) == 0)
	{
		getcwd(working_dir,MAXSZ);//Get current working directory
		sprintf(file_name,"%s/%s",working_dir,user_input + 4);

	
		if(unlink(file_name) == 0)//Remove file
		{
			sprintf(buff,"File succesfully removed!\n\n");
		}			
		else
		{
			sprintf(buff,"Error: %s\n\n",strerror(errno));
		}
	
		print_buff(app_state);
		ls_l_dir_view(app_state,working_dir);
		new_func_cli(app_state);

	}
	
	/* Change directory on server side */
	if(strncmp(user_input,"cd ",3) == 0)
	{
		char dir1[MAXSZ];
		char dir2[MAXSZ];
		char dir3[MAXSZ];
	
		bzero(dir1,MAXSZ);
		bzero(dir2,MAXSZ);
		bzero(dir3,MAXSZ);
	
		app_state->running = 1;
		gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
		sprintf(dir,"CWD %s\r\n",user_input + 3);
		sprintf(buff,"Command: CWD %s\nResponse: ",user_input+3);
		send(app_state->sockfd,dir,strlen(dir),0);
	
		print_buff(app_state);
		
		while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0)
		{
			message_from_server[no_of_bytes] = '\0';
			sprintf(buff,"%s",message_from_server);
			print_buff(app_state);
				
			if(strstr(message_from_server,"530 ") > 0 || strstr(message_from_server,"250 ") > 0 || strstr(message_from_server,"500 ") > 0 || strstr(message_from_server,"501 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"502 ") > 0 || strstr(message_from_server,"550 ") > 0)
				break;
		}
		sprintf(buff,"\n");
		print_buff(app_state);
		ret_value = list_content_view(argv,"ls -l",app_state);	
		if(ret_value == 0)
			new_func(app_state);
		
		/* Getting working directory to display after changing directory */
		sprintf(message_to_server,"PWD\r\n");
		
		send(app_state->sockfd,message_to_server,strlen(message_to_server),0);
		
		while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0)
		{
			message_from_server[no_of_bytes] = '\0';
			if(strstr(message_from_server,"257 ") > 0 || strstr(message_from_server,"500 ") > 0 || strstr(message_from_server,"501 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"502 ") > 0 || strstr(message_from_server,"550 ") > 0)
				break;
		}
		
		temp = 0;
		count = 0;
		j = 0;
		while(count != 2)
		{
			if(message_from_server[temp] == '\"')
			{
				count++;
				temp++;	
			}
	
			if(count == 2)
				break;
	
			if(count == 1)
				serv_curr_dir[j++] = message_from_server[temp++];
			else
				temp++;
		
			while(gtk_events_pending())
				gtk_main_iteration();
		}	
		serv_curr_dir[j] = '\0';
		
		gtk_entry_set_text(GTK_ENTRY(app_state->entry_dir),serv_curr_dir);
	
	}
	
	/* List files on server side */
	if(strncmp(user_input,"ls ",3)== 0 || strcmp(user_input,"ls")== 0)
	{
		app_state->running = 1;
		gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
		list_content(argv,user_input,app_state);	
	}
	
	/* Current working directory on server side */
	if(strcmp(user_input,"pwd")== 0)
	{
		app_state->running = 1;
		gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
		sprintf(message_to_server,"PWD\r\n");
		sprintf(buff,"Command: PWD\nResponse: ");
		print_buff(app_state);
	
		send(app_state->sockfd,message_to_server,strlen(message_to_server),0);
		
		while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0)
		{
			message_from_server[no_of_bytes] = '\0';
			sprintf(buff,"%s",message_from_server);
			print_buff(app_state);
				
			if(strstr(message_from_server,"257 ") > 0 || strstr(message_from_server,"500 ") > 0 || strstr(message_from_server,"501 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"502 ") > 0 || strstr(message_from_server,"550 ") > 0)
				break;
		}
			sprintf(buff,"\n");
			print_buff(app_state);
	
	}

	/* Download file from server */
	if(strncmp(user_input,"get ",4) == 0)
	{
		app_state->running = 1;
		gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
		start = clock();
		get_content(argv,user_input,app_state,home_dir);
		end = clock();
		cpu_time = ((double)(end - start))/CLOCKS_PER_SEC;
		sprintf(buff,"Time taken %lf\n\n",cpu_time);
		print_buff(app_state);
		getcwd(cwd,MAXSZ);
		ls_l_dir_view(app_state,cwd);
		new_func_cli(app_state);
	
	}
		
	/* Upload file to server */
	if(strncmp(user_input,"put ",4)== 0)
	{
		app_state->running = 1;
		gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
		start = clock();
		put_content(argv,user_input,app_state);
		end = clock();
		cpu_time = ((double)(end - start))/CLOCKS_PER_SEC;
		sprintf(buff,"Time taken %lf\n\n",cpu_time);
		print_buff(app_state);
		ret_value = list_content_view(argv,"ls -l",app_state);	
		if(ret_value == 0)
			new_func(app_state);
	}

	/* Upload file uniquely to server */
	if(strncmp(user_input,"uniqput ",8)== 0)
	{
		app_state->running = 1;
		gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
		start = clock();
		put_unique(argv,user_input,app_state);
		end = clock();
		cpu_time = ((double)(end - start))/CLOCKS_PER_SEC;
		sprintf(buff,"Time taken %lf\n\n",cpu_time);
		print_buff(app_state);
		ret_value = list_content_view(argv,"ls -l",app_state);	
		if(ret_value == 0)
			new_func(app_state);
	}
		
	/* Rename file on server */	
	if(strncmp(user_input,"rename ",7) == 0)
	{
		/* parse user input to get old file name and new file name */
		count = sscanf(user_input,"%s %s %s",message_to_server,old_name,new_name);
	
		if(count != 3)
		{		
			sprintf(buff,"Error: rename expects two arguments\n");
			print_buff(app_state);
			app_state->running = 0;
			gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
			return;
		}			
		rename_serv(app_state,old_name,new_name);
		ret_value = list_content_view(argv,"ls -l",app_state);	
		if(ret_value == 0)
			new_func(app_state);
	}

	/* Creating diectory on server */
	if(strncmp(user_input,"mkdir ",6) == 0)
	{	
		app_state->running = 1;
		gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
		sprintf(message_to_server,"MKD %s\r\n",user_input + 6);
		sprintf(buff,"Command: MKD %s\nResponse: ",user_input+6);	
		print_buff(app_state);
	
		send(app_state->sockfd,message_to_server,strlen(message_to_server),0);
		while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0 )
		{
			message_from_server[no_of_bytes] = '\0';
			sprintf(buff,"%s",message_from_server);
			print_buff(app_state);
				

			if(strstr(message_from_server,"257 ") > 0 || strstr(message_from_server,"530 ") > 0 || strstr(message_from_server,"500 ") > 0|| strstr(message_from_server,"501 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"502 ") > 0 || strstr(message_from_server,"550 ") > 0)
				break;
			
		}	
		sprintf(buff,"\n");
		print_buff(app_state);
		ret_value = list_content_view(argv,"ls -l",app_state);	
		if(ret_value == 0)	
			new_func(app_state);
	}

	/* Removing directory on server */
	if(strncmp(user_input,"rmdir ",6) == 0)
	{	
		rmdir_serv(app_state,user_input);
		ret_value = list_content_view(argv,"ls -l",app_state);	
		if(ret_value == 0)	
			new_func(app_state);
	}
		
	/* Delete file on server */
	if(strncmp(user_input,"rm ",3) == 0)
	{	
		app_state->running = 1;
		gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
		sprintf(message_to_server,"DELE %s\r\n",user_input + 3);
		sprintf(buff,"Command: DELE %s\nResponse: ",user_input+3);	
		print_buff(app_state);
	

		send(app_state->sockfd,message_to_server,strlen(message_to_server),0);
		while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0 )
		{
			message_from_server[no_of_bytes] = '\0';
			sprintf(buff,"%s\n",message_from_server);
			print_buff(app_state);
				
			if(strstr(message_from_server,"250 ") > 0 || strstr(message_from_server,"450 ") > 0 || strstr(message_from_server,"530 ") > 0 || strstr(message_from_server,"500 ") > 0|| strstr(message_from_server,"501 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"502 ") > 0 || strstr(message_from_server,"550 ") > 0)
				break;
			
		}
		ret_value = list_content_view(argv,"ls -l",app_state);	
		if(ret_value == 0)
			new_func(app_state);
	
	}

	/* Reset 'running' variable */	
	app_state->running = 0;
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
		
	return;
}

/* Quit function */
void close_func(GtkWidget *widget, Appstate *app_state)
{
	int no_of_bytes;
	char message_to_server[MAXSZ];
	char message_from_server[MAXSZ];

	bzero(message_to_server,MAXSZ);
	bzero(message_from_server,MAXSZ);
	
	/* Command laready executing. Wait for completion */
	if(app_state->running == 1)//Pop up window
	{

		GtkWidget *popup_window;
		GtkWidget *table;
		GtkWidget *label;
		GtkWidget *horiz_align;
	

		popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title (GTK_WINDOW (popup_window), "Error");
		gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
		gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
		gtk_window_set_decorated (GTK_WINDOW (popup_window), TRUE);
		gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);
		gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
		gtk_widget_set_size_request (popup_window, 370, 60);
		gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
		gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);

		gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
		g_signal_connect (G_OBJECT (popup_window),
               				    "focus-out-event",
              		 		    G_CALLBACK (on_popup_focus_out),
                   				NULL);
	
		table = gtk_table_new (2,2,FALSE);
		gtk_container_add(GTK_CONTAINER(popup_window),table);
		label = gtk_label_new("Command executing. Please wait for completion!");

	horiz_align = gtk_alignment_new(0,0,0,0);
    
	gtk_container_add(GTK_CONTAINER(horiz_align),label);
	gtk_table_attach(GTK_TABLE(table),horiz_align,1,2,0,1,GTK_FILL,GTK_FILL,0,0);
	
	gtk_widget_show_all (popup_window);
	gtk_widget_grab_focus (popup_window);

	}
	else
	{
		if(app_state->status == 1)//Connection is established
		{
			/* Ask server to close connection */
			sprintf(message_to_server,"QUIT\r\n");
			sprintf(buff,"Command: QUIT\nResponse: ");	
			print_buff(app_state);
		
			/* Send message to server */
			send(app_state->sockfd,message_to_server,strlen(message_to_server),0);
			while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0)
			{
				message_from_server[no_of_bytes] = '\0';
				sprintf(buff,"%s",message_from_server);
				print_buff(app_state);

				if(strstr(message_from_server,"221 ") > 0 || strstr(message_from_server,"500 ") > 0)	
					break;
			}		
			sprintf(buff,"\n");
			print_buff(app_state);
			close(app_state->sockfd);
			usleep(500000);//wait 0.5 seconds
		}	
		
		gtk_main_quit();//Quit
	}
	return;
}

/*Validating IP Address*/
int validate_ip(char *ip)
{
	int value_1 = MIN_VALUE;
	int value_2 = MIN_VALUE;
	int value_3 = MIN_VALUE;
	int value_4 = MIN_VALUE;
	int count = INITIALISE;
	int i = INITIALISE;

	while(ip[i] != '\0')
	{
		if(ip[i] == '.')
			count++;
		i++;
	}
	
	if(count != 3 )
		return -1;
	else
	{
		sscanf(ip,"%d.%d.%d.%d",&value_1,&value_2,&value_3,&value_4);
		
		if(value_1 < MIN_IP || value_2 < MIN_IP || value_3 < MIN_IP || value_4 < MIN_IP || value_1 > MAX_IP || value_2 > MAX_IP || value_3 > MAX_IP || value_4 > MAX_IP)/* IP Addresses from 0.0.0.0 to 255.255.255.255*/
			return -1;
		else
			return 1;

	}

}




/* Establish connection */
int connect_func(GtkWidget *widget, Appstate *app_state)
{
	int no_of_bytes;/* number of bytes sent or received from server */
	int ret_value;

	char message_from_server[MAXSZ];/* message from server*/
	char message_to_server[MAXSZ];/* message from server*/
	char cwd[MAXSZ];
	
	bzero(message_from_server,MAXSZ);
	bzero(message_to_server,MAXSZ);
	bzero(cwd,MAXSZ);
	bzero(buff,MAXSZ1);
	
	getcwd(cwd,MAXSZ);

	if(app_state->running == 1)//Command running
	{
		close_func(widget,app_state);
		return 2;
	}
	
	if(app_state->status == 1)//Connection already established. Disconnect
	{
		sprintf(message_to_server,"QUIT\r\n");
		sprintf(buff,"Command: QUIT\nResponse: ");	
		sprintf(message_to_server,"QUIT\r\n");
		sprintf(buff,"Command: QUIT\nResponse: ");	
		print_buff(app_state);
		
		/* Send message to server */
		send(app_state->sockfd,message_to_server,strlen(message_to_server),0);
		while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0)
		{
			message_from_server[no_of_bytes] = '\0';
			sprintf(buff,"%s",message_from_server);
			print_buff(app_state);

			if(strstr(message_from_server,"221 ") > 0 || strstr(message_from_server,"500 ") > 0)	
				break;
		}		
		sprintf(buff,"\n");
		print_buff(app_state);
		close(app_state->sockfd);
		sprintf(buff,"Disconnected\n-----------------------------------------------------------------------------------------------------------------------------------------------------\n");
		print_buff(app_state);
		gtk_button_set_label(GTK_BUTTON(app_state->button),"Connect");
		app_state->status = 0;
		gtk_entry_set_editable(GTK_ENTRY(app_state->entry),TRUE);
		return 2;
	}
	

	char *password = (char*)malloc(MAXSZ);
	char *username = (char *)malloc(MAXSZ);
	char *address= (char *)malloc(MAXSZ);
	
	/* Get server ip address, username and password */
	address = (char *)gtk_entry_get_text(GTK_ENTRY(app_state->entry));
	username = (char *)gtk_entry_get_text(GTK_ENTRY(app_state->user));
	password = (char *)gtk_entry_get_text(GTK_ENTRY(app_state->password));
	
	char *argv = address;

	char serv_curr_dir[MAXSZ];
	char ip_address[30];

	bzero(serv_curr_dir,MAXSZ);	
	
	/*Temporary Variables*/
	int connect_value;
	int ip_valid;
	int temp = MIN_VALUE;
	int j = 0;
	int count = 0;
	int i = 0;

	GtkTextIter iter;

	struct sockaddr_in serverAddress;/* client will connect on this */
	struct hostent *host;

	if(isdigit(argv[0]))
	{
		ip_valid = validate_ip(argv);/* Validate ip-address entered by user */
	
		if(ip_valid == MIN_VALUE)/* Invalid ipaddress */
		{
			sprintf(buff,"Error: Invalid ip-address\n");
			buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app_state->text_view));
			gtk_text_buffer_set_text(buffer,buff,strlen(buff));
			gtk_text_buffer_get_end_iter(buffer,&iter);
			gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(app_state->text_view),&iter,0.0,FALSE,0,0);
			while(gtk_events_pending())
			gtk_main_iteration();
			return -1;
		}
	}
	else
	{
		host = gethostbyname(argv);
		if(host == NULL)
		{
			switch(h_errno)
			{
				case NO_ADDRESS:
					sprintf(buff,"The requested name is valid but doesn't have any IP Address\n");
					break;
				case NO_RECOVERY:
					sprintf(buff,"A non-recoverable name server error occured\n");
					break;
				case TRY_AGAIN:
					sprintf(buff,"A temporary error occurred on authoritative name server. Try again later.\n");
				case HOST_NOT_FOUND:
					sprintf(buff,"Unknown host.\n");
					break;
				default:
					sprintf(buff,"Unknown error.\n");

			}
		
			buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app_state->text_view));
			gtk_text_buffer_set_text(buffer,buff,strlen(buff));
			gtk_text_buffer_get_end_iter(buffer,&iter);
			gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(app_state->text_view),&iter,0.0,FALSE,0,0);
			while(gtk_events_pending())
				gtk_main_iteration();
		
			return -1;
		}
		while(host->h_addr_list[i]!= NULL)
		{
			sprintf(ip_address,"%u.%u.%u.%u",host->h_addr_list[i][0] & 0x000000FF, host->h_addr_list[i][1] & 0x000000FF,host->h_addr_list[i][2] & 0x000000FF,host->h_addr_list[i][3] & 0x000000FF);
			i++;
		}
		gtk_entry_set_text(GTK_ENTRY(app_state->entry),ip_address);
		
	}
	
	

	app_state->sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);/* Create socket */

	if(app_state->sockfd == -1)/* Error in socket creation */
	{
        	sprintf(buff,"Error: %s\n",strerror(errno)); 
		buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app_state->text_view));
		gtk_text_buffer_set_text(buffer,buff,strlen(buff));
		gtk_text_buffer_get_end_iter(buffer,&iter);
		gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(app_state->text_view),&iter,0.0,FALSE,0,0);
		while(gtk_events_pending())
			gtk_main_iteration();
		return -1;
	}

	bzero(buff,MAXSZ1);
	bzero(&serverAddress,sizeof(serverAddress));/* Initialise structure */

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(argv);
	serverAddress.sin_port = htons(PORT);

	/* Connect to server */
	connect_value = connect(app_state->sockfd,(struct sockaddr *)&serverAddress,sizeof(serverAddress));
	if(connect_value == -1)/* Connection Error */
	{
        	sprintf(buff,"Error: %s\n",strerror(errno)); 
		buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app_state->text_view));
		gtk_text_buffer_set_text(buffer,buff,strlen(buff));
		gtk_text_buffer_get_end_iter(buffer,&iter);
		gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(app_state->text_view),&iter,0.0,FALSE,0,0);
		while(gtk_events_pending())
			gtk_main_iteration();
		return -1;
	}

	/* set connection variable */
	
	sprintf(buff,"Connected to %s.\n",argv);
	
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app_state->text_view));
	gtk_text_buffer_set_text(buffer,buff,strlen(buff));
	gtk_text_buffer_get_end_iter(buffer,&iter);
	gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(app_state->text_view),&iter,0.0,FALSE,0,0);
	
	while(gtk_events_pending())
		gtk_main_iteration();
	
	/* Receive message from server "Server will send 220" */
	while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0 )
	{
		message_from_server[no_of_bytes] = '\0';
		sprintf(buff,"%s",message_from_server);
		print_buff(app_state);
	
		if(strstr(message_from_server,"220 ") > 0 || strstr(message_from_server,"421 ") > 0)	
			break;
	}
	sprintf(buff,"\n");
	print_buff(app_state);
	
	if(strstr(message_from_server,"421 ") > 0)	
		return -1;
	
	app_state->status = 1;
	gtk_button_set_label(GTK_BUTTON(app_state->button),"Disconnect");
	gtk_entry_set_editable(GTK_ENTRY(app_state->entry),FALSE);
	
	/* Send user details */
	sprintf(user,"USER %s\r\n",username);
	sprintf(buff,"Command: USER %s\nResponse: ",username);	
	print_buff(app_state);
	send(app_state->sockfd,user,strlen(user),0);/* Send username to server */
	

	/*
		Receive message from server after sending user name.
		Message with code 331 asks you to enter password corresponding to user.
		Message with code 230 means no password is required for the entered username(LOGIN successful).
	*/
	while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0)
	{	
		message_from_server[no_of_bytes] = '\0';
		if(strncmp(message_from_server,"331",3) == 0)
		{
			temp = 1;
		}
	
		if(strncmp(message_from_server,"230",3) == 0)
		{
			temp = 2;
		}
		
		if(strncmp(message_from_server,"530",3) == 0)
		{
			temp = 0;	
		}

		sprintf(buff,"%s",message_from_server);
		print_buff(app_state);
		bzero(buff,MAXSZ);

		if(strstr(message_from_server,"230 ") > 0 || strstr(message_from_server,"500 ") > 0 || strstr(message_from_server,"501 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"332 ") > 0 || strstr(message_from_server,"530 ")|| strstr(message_from_server,"331 ") > 0)
			break;
	}
	sprintf(buff,"\n");
	print_buff(app_state);
	
	
	if(temp == 1)
	{
	
		sprintf(pass,"PASS %s\r\n",password);
		sprintf(buff,"Command: PASS ******\nResponse: ");
		print_buff(app_state);
		
		send(app_state->sockfd,pass,strlen(pass),0);/* Send password to server */

		/* Receive message from server */
		while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0)
		{

			if(strncmp(message_from_server,"230",3) == 0)
			{
				temp = 2;
			}
			
			if(strncmp(message_from_server,"530",3) == 0)
			{
				temp = 0;	
			}
			if(strstr(message_from_server,"230 ") > 0)
				sprintf(buff,"%s",strstr(message_from_server,"230 "));
			if(strstr(message_from_server,"530 ") > 0)
				sprintf(buff,"%s",strstr(message_from_server,"530 "));
			print_buff(app_state);
			bzero(buff,MAXSZ);

			if(strstr(message_from_server,"230 ") > 0 || strstr(message_from_server,"500 ") > 0 || strstr(message_from_server,"501 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"332 ") > 0 || strstr(message_from_server,"530 ")|| strstr(message_from_server,"503 ") > 0 || strstr(message_from_server,"202 ") > 0)
				break;

			
		}
	}	

	sprintf(buff,"\n");
	print_buff(app_state);
	
	if(temp == 0)/* Invalid ipaddress */
	{
		close(app_state->sockfd);
		app_state->status = 0;
		gtk_button_set_label(GTK_BUTTON(app_state->button),"Connect");
		gtk_entry_set_editable(GTK_ENTRY(app_state->entry),TRUE);
		return 0;
	}
	

	/* System type(Server) */
	sprintf(message_to_server,"SYST\r\n");
	sprintf(buff,"Command: SYST\nResponse: ");
	print_buff(app_state);
	
	send(app_state->sockfd,message_to_server,strlen(message_to_server),0);

	while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0)
	{
		message_from_server[no_of_bytes] = '\0';
		sprintf(buff,"%s",message_from_server);
		print_buff(app_state);
		
		if(strstr(message_from_server,"215 ") > 0 || strstr(message_from_server,"500 ") > 0 || strstr(message_from_server,"501 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"502 ") > 0) 	
			break;

	}
	sprintf(buff,"\n");
	print_buff(app_state);
	ls_l_dir_view(app_state,".");	
	new_func_cli(app_state);
	ret_value = list_content_view(argv,"ls -l",app_state);	
	if(ret_value == 0)
		new_func(app_state);
	
	/* Getting working directory to display after changing directory */
	sprintf(message_to_server,"PWD\r\n");
	
	send(app_state->sockfd,message_to_server,strlen(message_to_server),0);
		
	while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0)
	{
		message_from_server[no_of_bytes] = '\0';
		while(gtk_events_pending())
			gtk_main_iteration();
		if(strstr(message_from_server,"257 ") > 0 || strstr(message_from_server,"500 ") > 0 || strstr(message_from_server,"501 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"502 ") > 0 || strstr(message_from_server,"550 ") > 0)
			break;
	}
	

	temp = 0;
	while(count != 2)
	{
		if(message_from_server[temp] == '\"')
		{
			count++;
			temp++;	
		}

		if(count == 2)
			break;

		if(count == 1)
			serv_curr_dir[j++] = message_from_server[temp++];
		else
			temp++;
		
		while(gtk_events_pending())
			gtk_main_iteration();
	}	
	serv_curr_dir[j] = '\0';
	
	gtk_entry_set_text(GTK_ENTRY(app_state->entry_dir),serv_curr_dir);
	gtk_entry_set_text(GTK_ENTRY(app_state->entry_dir_cli),cwd);

	return 1;
}


/* Icon */
GdkPixbuf *create_pixbuf(const gchar *filename)
{
	GdkPixbuf *pixbuf;
	GError *error = NULL;
	pixbuf = gdk_pixbuf_new_from_file(filename,&error);

	return pixbuf;
}

/* Upload file */
void func_upload(GtkWidget *widget,Appstate *app_state)
{
	char *user_input = malloc(MAXSZ);
	/* Get ip address of server */
	char *argv = (char *)gtk_entry_get_text(GTK_ENTRY(app_state->entry));
	/* Get name of selected file */
	char *entry = (char *)gtk_entry_get_text(GTK_ENTRY(app_state->fileselect));
	
	char cur_dir[MAXSZ];	
	
	clock_t start;
	clock_t end;

	double cpu_time;

	int i;
	int j;	
	int ret_value;
	
	/*Check whether file with entered name exists or not */
	if(access(entry,F_OK) != 0)
	{
		GtkWidget *popup_window;
		GtkWidget *table;
		GtkWidget *label;
		GtkWidget *horiz_align;
	

		popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title (GTK_WINDOW (popup_window), "Error");
		gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
		gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
		gtk_window_set_decorated (GTK_WINDOW (popup_window), TRUE);
		gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);
		gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
		gtk_widget_set_size_request (popup_window, 350, 60);
		gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
		gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);

		gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
		g_signal_connect (G_OBJECT (popup_window),
                			    "focus-out-event",
               		 		    G_CALLBACK (on_popup_focus_out),
                    				NULL);
	
		table = gtk_table_new (2,2,FALSE);
		gtk_container_add(GTK_CONTAINER(popup_window),table);
		label = gtk_label_new("\t     No such file present. Please try again!");

		horiz_align = gtk_alignment_new(0,0,0,0);
    
		gtk_container_add(GTK_CONTAINER(horiz_align),label);
		gtk_table_attach(GTK_TABLE(table),horiz_align,1,2,0,1,GTK_FILL,GTK_FILL,0,0);


		gtk_widget_show_all (popup_window);
		gtk_widget_grab_focus (popup_window);
	}
	else
	{
	
		if(app_state->status == 0)//No connection
		{
			no_connection(app_state);
			return;
		}
	
		if(app_state->running == 1)//Another command already running
		{
			already_running(app_state);
			return;
		}
		
		for(i = 0,j = 0;entry[i]!='\0';i++)//parse file path to get file name
		{
			if(entry[i] == '/')
				j = i;
			while(gtk_events_pending())
				gtk_main_iteration();

		}

		for(i=0;i < j; i++)//get file directory
		{
			cur_dir[i]=entry[i];
		}
		cur_dir[i]='\0';

		/* Uniquely upload file i.e if a file already exists on server with same name then create another copy. */
		sprintf(user_input,"uniqput %s",entry + j + 1);
		gtk_entry_set_text(GTK_ENTRY(app_state->command),user_input); 

		if(chdir(cur_dir) == -1)
			return;
		
		/* Set 'running' variable */
		app_state->running = 1;
		gtk_button_set_label(GTK_BUTTON(app_state->ok_file_button),"Uploading");
		gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
		/* Start clock */
		start = clock();
		
		put_unique(argv,user_input,app_state);//Function to upload file
		/* Stop clock */
		end = clock();

		gtk_button_set_label(GTK_BUTTON(app_state->ok_file_button),"Upload");
		/*Reset 'running' variable */
		app_state->running = 0;
		ret_value = list_content_view(argv,"ls -l",app_state);
		if(ret_value == 0)
			new_func(app_state);
		
		cpu_time = ((double)(end - start))/CLOCKS_PER_SEC;
		sprintf(buff,"Time taken %lf\n\n",cpu_time);
		print_buff(app_state);
		
	}
	gtk_entry_set_text(GTK_ENTRY(app_state->command),"Command"); 
	gtk_entry_set_text(GTK_ENTRY(app_state->fileselect),"Select file to upload"); 
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	
	return;
}

/* File chooser dialog */
void file_select(GtkWidget *widget, Appstate *app_state)
{
	GtkWidget *dialog;

	dialog = gtk_file_chooser_dialog_new ("Select File",
     				      GTK_WINDOW(app_state->window),
     				      GTK_FILE_CHOOSER_ACTION_OPEN,
     				      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
     				      GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
     				      NULL);
    	gtk_window_set_default_size(GTK_WINDOW(dialog),660,480);

	if(gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		char *filename;
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		gtk_entry_set_text(GTK_ENTRY(app_state->fileselect),filename); 
		
		g_free (filename);
	}
	gtk_widget_destroy (dialog);
	return;	
}

/* Create directory on sever */
void mkdir_serv(Appstate *app_state,char *user_input)
{
	if(app_state->status == 0)//No connection
	{
		no_connection(app_state);
		return;
	}
	
	if(app_state->running == 1)//Another command already running
	{
		already_running(app_state);
		return;
	}
	
	char *address = (char *)malloc(MAXSZ);
	
	address = (char *)gtk_entry_get_text(GTK_ENTRY(app_state->entry));
	char *argv = address;	
	/* Set 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
	app_state->running = 1;
	
	int no_of_bytes;
	int ret_value;

	char message_to_server[MAXSZ];
	char message_from_server[MAXSZ];

	bzero(message_from_server,MAXSZ);
	bzero(message_to_server,MAXSZ);

	sprintf(message_to_server,"MKD %s\r\n",user_input + 6);
	sprintf(buff,"Command: MKD %s\nResponse: ",user_input+6);	
	print_buff(app_state);
	
	/* Request server to create directory */
	send(app_state->sockfd,message_to_server,strlen(message_to_server),0);
	while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0 )
	{
		message_from_server[no_of_bytes] = '\0';
		if(strncmp(message_from_server,"550",3) == 0)/* MKD fails*/
		{
			sprintf(buff,"Error: Creating directory failed.\n\n");
		}
		else
			sprintf(buff,"%s\n",message_from_server);
		print_buff(app_state);
				

		if(strstr(message_from_server,"257 ") > 0 || strstr(message_from_server,"530 ") > 0 || strstr(message_from_server,"500 ") > 0|| strstr(message_from_server,"501 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"502 ") > 0 || strstr(message_from_server,"550 ") > 0)
			break;
			
	}
	
	ret_value = list_content_view(argv,"ls -l",app_state);	
	if(ret_value == 0)
		new_func(app_state);
	/* Reset 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;
	return;

}

/* Create directory on client side. Display dialog */
void crdir_cli_func(GtkWidget *widget,Appstate *app_state)
{
	GtkWidget *dialog;
	GtkWidget *entry;
	GtkWidget *content_area;

	char user_input[MAXSZ];

	dialog = gtk_dialog_new_with_buttons("Create Directory(Client)",GTK_WINDOW(app_state->window),GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT,NULL,NULL,NULL,NULL);
	gtk_window_set_default_size(GTK_WINDOW(dialog),350,100);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Ok", 0);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Cancel", 1);

	content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(entry),"Enter name"); 
    
	gtk_container_add(GTK_CONTAINER(content_area), entry);

	gtk_widget_show_all(dialog);
	gint result = gtk_dialog_run(GTK_DIALOG(dialog));

	char *entry_line; // variable for entered text

	switch(result)
	{
		case 0:
	        	entry_line = (char *)gtk_entry_get_text(GTK_ENTRY(entry));
	
        		if(strlen(entry_line) == 0 || entry_line == NULL || strcmp(entry_line,"Enter name") == 0)//Invalid name
        		{
				GtkWidget *popup_window;
				GtkWidget *table;
				GtkWidget *label;
				GtkWidget *horiz_align;
			
				popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
				gtk_window_set_title (GTK_WINDOW (popup_window), "Input Error");
				gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
				gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
				gtk_window_set_decorated (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_widget_set_size_request (popup_window, 250, 60);
				gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
				gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);
		
				gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
				g_signal_connect (G_OBJECT (popup_window),
        		       			    "focus-out-event",
        		      		 		    G_CALLBACK (on_popup_focus_out),
        			           				NULL);
		
				table = gtk_table_new (2,2,FALSE);
				gtk_container_add(GTK_CONTAINER(popup_window),table);
				label = gtk_label_new("\t\tInvalid filename!");
		
				horiz_align = gtk_alignment_new(0,0,0,0);
    	
				gtk_container_add(GTK_CONTAINER(horiz_align),label);
				gtk_table_attach(GTK_TABLE(table),horiz_align,1,2,0,1,GTK_FILL,GTK_FILL,0,0);
			
				gtk_widget_show_all (popup_window);
				gtk_widget_grab_focus (popup_window);
			}
			else
			{
				bzero(user_input,MAXSZ);
				sprintf(user_input,"!mkdir %s",entry_line);
				gtk_entry_set_text(GTK_ENTRY(app_state->command),user_input); 
			
				mkdir_cli(app_state,user_input);
    				gtk_entry_set_text(GTK_ENTRY(app_state->command),"Command"); 
			}

        		break;

		case 1:
			break;
		default:
			break;
	}

	gtk_widget_destroy(dialog);
	return;
}

/* Remove directory on client side. Display dialog */
void rmdir_cli_func(GtkWidget *widget,Appstate *app_state)
{
	GtkWidget *dialog;
	GtkWidget *entry;
	GtkWidget *content_area;

	char user_input[MAXSZ];

	dialog = gtk_dialog_new_with_buttons("Remove Directory(Client)",GTK_WINDOW(app_state->window),GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT,NULL,NULL,NULL,NULL);
	gtk_window_set_default_size(GTK_WINDOW(dialog),350,100); 

	gtk_dialog_add_button(GTK_DIALOG(dialog), "Ok", 0);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Cancel", 1);

	content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(entry),"Enter name"); 
	gtk_container_add(GTK_CONTAINER(content_area), entry);

	gtk_widget_show_all(dialog);
	gint result = gtk_dialog_run(GTK_DIALOG(dialog));

	char *entry_line; // variable for entered text

	switch(result)
	{
		case 0:
			entry_line = (char *)gtk_entry_get_text(GTK_ENTRY(entry));

			if(strlen(entry_line) == 0 || entry_line == NULL ||strcmp(entry_line,"Enter name") == 0)//Invalid entry
        		{
				GtkWidget *popup_window;
				GtkWidget *table;
				GtkWidget *label;
				GtkWidget *horiz_align;
	
				popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
				gtk_window_set_title (GTK_WINDOW (popup_window), "Input Error");
				gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
				gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
				gtk_window_set_decorated (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_widget_set_size_request (popup_window, 250, 60);
				gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
				gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);

				gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
				g_signal_connect (G_OBJECT (popup_window),
        		       			    "focus-out-event",
              				 		    G_CALLBACK (on_popup_focus_out),
                   						NULL);
	
				table = gtk_table_new (2,2,FALSE);
				gtk_container_add(GTK_CONTAINER(popup_window),table);
				label = gtk_label_new("\t\tInvalid filename!");

				horiz_align = gtk_alignment_new(0,0,0,0);
    
				gtk_container_add(GTK_CONTAINER(horiz_align),label);
				gtk_table_attach(GTK_TABLE(table),horiz_align,1,2,0,1,GTK_FILL,GTK_FILL,0,0);
	
				gtk_widget_show_all (popup_window);
				gtk_widget_grab_focus (popup_window);
			}
			else
			{
				bzero(user_input,MAXSZ);
				sprintf(user_input,"!rmdir %s",entry_line);
				gtk_entry_set_text(GTK_ENTRY(app_state->command),user_input); 
				rmdir_cli(app_state,user_input);
				gtk_entry_set_text(GTK_ENTRY(app_state->command),"Command"); 
			}

			break;
		case 1:
			break;
		default:
			break;
	}
	gtk_widget_destroy(dialog);
	return;
}

/* Rename file on server. Display dialog */
void rename_serv_func(GtkWidget *widget,Appstate *app_state)
{
	GtkWidget *dialog;
	GtkWidget *entry;
	GtkWidget *entry1;
	GtkWidget *content_area;

	char user_input[MAXSZ];

	dialog = gtk_dialog_new_with_buttons("Rename File(Server)",GTK_WINDOW(app_state->window),GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT,NULL,NULL,NULL,NULL);
	gtk_window_set_default_size(GTK_WINDOW(dialog),350,130);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Ok", 0);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Cancel", 1);

	content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	entry = gtk_entry_new(); 
	entry1 = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(entry1),"Name of file"); 
	gtk_entry_set_text(GTK_ENTRY(entry),"New name"); 
	gtk_container_add(GTK_CONTAINER(content_area), entry1);
	gtk_container_add(GTK_CONTAINER(content_area), entry);

	gtk_widget_show_all(dialog);
	gint result = gtk_dialog_run(GTK_DIALOG(dialog));

	char *old_name = (char *)malloc(MAXSZ); // variable for entered text
	char *new_name = (char *)malloc(MAXSZ); // variable for entered text
   
	switch(result)
	{
		case 0:
			old_name = (char *)gtk_entry_get_text(GTK_ENTRY(entry1));
			new_name = (char *)gtk_entry_get_text(GTK_ENTRY(entry));

			if(strlen(old_name) == 0 || old_name == NULL || strlen(new_name) == 0 || new_name == NULL || strcmp(old_name,"Name of file") == 0)//Invalid entry
			{
				GtkWidget *popup_window;
				GtkWidget *table;
				GtkWidget *label;
				GtkWidget *horiz_align;
	
				popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
				gtk_window_set_title (GTK_WINDOW (popup_window), "Input Error");
				gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
				gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
				gtk_window_set_decorated (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_widget_set_size_request (popup_window, 250, 60);
				gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
				gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);
		
				gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
				g_signal_connect (G_OBJECT (popup_window),
		               			    "focus-out-event",
		              		 		    G_CALLBACK (on_popup_focus_out),
								NULL);
				table = gtk_table_new (2,2,FALSE);
				gtk_container_add(GTK_CONTAINER(popup_window),table);
				label = gtk_label_new("\t\tInvalid filename!");

				horiz_align = gtk_alignment_new(0,0,0,0);
    	
				gtk_container_add(GTK_CONTAINER(horiz_align),label);
				gtk_table_attach(GTK_TABLE(table),horiz_align,1,2,0,1,GTK_FILL,GTK_FILL,0,0);
	
				gtk_widget_show_all (popup_window);
				gtk_widget_grab_focus (popup_window);
			}
			else
			{
				sprintf(user_input,"rename %s %s",old_name,new_name);
				
				gtk_entry_set_text(GTK_ENTRY(app_state->command),user_input); 
				rename_serv(app_state,old_name,new_name);
				gtk_entry_set_text(GTK_ENTRY(app_state->command),"Command"); 
			}

        		break;

		case 1:
			break;
		default:
			break;
	}

	gtk_widget_destroy(dialog);
	return;
}

/* Create directory on server. Display dialog */
void crdir_serv_func(GtkWidget *widget,Appstate *app_state)
{
	GtkWidget *dialog;
	GtkWidget *entry;
	GtkWidget *content_area;
	char user_input[MAXSZ];

	dialog = gtk_dialog_new_with_buttons("Create Directory(Server)",GTK_WINDOW(app_state->window),GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT,NULL,NULL,NULL,NULL);
	gtk_window_set_default_size(GTK_WINDOW(dialog),350,100);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Ok", 0);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Cancel", 1);

	content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(entry),"Enter name"); 
	gtk_container_add(GTK_CONTAINER(content_area), entry);

	gtk_widget_show_all(dialog);
	gint result = gtk_dialog_run(GTK_DIALOG(dialog));

	char *entry_line; // variable for entered text

	switch(result)
	{
		case 0:
        		entry_line = (char *)gtk_entry_get_text(GTK_ENTRY(entry));

        		if(strlen(entry_line) == 0 || entry_line == NULL || strcmp(entry_line,"Enter name") == 0)//Invalid entry
        		{
				GtkWidget *popup_window;
				GtkWidget *table;
				GtkWidget *label;
				GtkWidget *horiz_align;
	

				popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
				gtk_window_set_title (GTK_WINDOW (popup_window), "Input Error");
				gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
				gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
				gtk_window_set_decorated (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_widget_set_size_request (popup_window, 250, 60);
				gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
				gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);

				gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
				g_signal_connect (G_OBJECT (popup_window),
               					    "focus-out-event",
              			 		    G_CALLBACK (on_popup_focus_out),
                   					NULL);
	
				table = gtk_table_new (2,2,FALSE);
				gtk_container_add(GTK_CONTAINER(popup_window),table);
				label = gtk_label_new("\t\tInvalid filename!");

				horiz_align = gtk_alignment_new(0,0,0,0);
    
				gtk_container_add(GTK_CONTAINER(horiz_align),label);
				gtk_table_attach(GTK_TABLE(table),horiz_align,1,2,0,1,GTK_FILL,GTK_FILL,0,0);
	
				gtk_widget_show_all (popup_window);
				gtk_widget_grab_focus (popup_window);
    		
			}
			else
			{
				bzero(user_input,MAXSZ);
				sprintf(user_input,"mkdir %s",entry_line);
				gtk_entry_set_text(GTK_ENTRY(app_state->command),user_input); 
				mkdir_serv(app_state,user_input);
    				gtk_entry_set_text(GTK_ENTRY(app_state->command),"Command"); 
			}

        		break;

		case 1:
			break;
		default:
			break;
	}

	gtk_widget_destroy(dialog);
	return;
}

/* Remove directory on server. Display dialog */
void rmdir_serv_func(GtkWidget *widget,Appstate *app_state)
{
	GtkWidget *dialog;
	GtkWidget *entry;
	GtkWidget *content_area;

	char user_input[MAXSZ];

	dialog = gtk_dialog_new_with_buttons("Remove Directory(Server)",GTK_WINDOW(app_state->window),GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT,NULL,NULL,NULL,NULL);
	gtk_window_set_default_size(GTK_WINDOW(dialog),350,100);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Ok", 0);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Cancel", 1);

	content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(entry),"Enter name"); 
	gtk_container_add(GTK_CONTAINER(content_area), entry);

	gtk_widget_show_all(dialog);
	gint result = gtk_dialog_run(GTK_DIALOG(dialog));

	char *entry_line; // variable for entered text

	switch(result)
	{
		case 0:
		        entry_line = (char *)gtk_entry_get_text(GTK_ENTRY(entry));

    			if(strlen(entry_line) == 0 || entry_line == NULL || strcmp(entry_line,"Enter name") == 0)//Invalid entry
        		{
				GtkWidget *popup_window;
				GtkWidget *table;
				GtkWidget *label;
				GtkWidget *horiz_align;
	

				popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
				gtk_window_set_title (GTK_WINDOW (popup_window), "Input Error");
				gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
				gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
				gtk_window_set_decorated (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_widget_set_size_request (popup_window, 250, 60);
				gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
				gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);

				gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
				g_signal_connect (G_OBJECT (popup_window),
               				    "focus-out-event",
              		 			    G_CALLBACK (on_popup_focus_out),
                   					NULL);
				table = gtk_table_new (2,2,FALSE);
				gtk_container_add(GTK_CONTAINER(popup_window),table);
				label = gtk_label_new("\t\tInvalid filename!");

				horiz_align = gtk_alignment_new(0,0,0,0);
    
				gtk_container_add(GTK_CONTAINER(horiz_align),label);
				gtk_table_attach(GTK_TABLE(table),horiz_align,1,2,0,1,GTK_FILL,GTK_FILL,0,0);
	
				gtk_widget_show_all (popup_window);
				gtk_widget_grab_focus (popup_window);
    		
			}
			else
			{
				bzero(user_input,MAXSZ);
				sprintf(user_input,"rmdir %s",entry_line);
				gtk_entry_set_text(GTK_ENTRY(app_state->command),user_input); 
				rmdir_serv(app_state,user_input);
    				gtk_entry_set_text(GTK_ENTRY(app_state->command),"Command"); 
			}

        		break;

		case 1:
			break;
		default:
			break;
		}

	gtk_widget_destroy(dialog);
	return;
}
	
/* Rename file on client. Display dialog */
void rename_cli_func(GtkWidget *widget,Appstate *app_state)
{
	GtkWidget *dialog;
	GtkWidget *entry;
	GtkWidget *entry1;
	GtkWidget *content_area;

	char user_input[MAXSZ];
	char cwd[MAXSZ];
	char file_name[MAXSZ];
	
	bzero(cwd,MAXSZ);
	bzero(file_name,MAXSZ);

	getcwd(cwd,MAXSZ);//Get current working directory

	dialog = gtk_dialog_new_with_buttons("Rename File(Client)",GTK_WINDOW(app_state->window),GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT,NULL,NULL,NULL,NULL);
	gtk_window_set_default_size(GTK_WINDOW(dialog),350,130);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Ok", 0);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Cancel", 1);

	content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	entry = gtk_entry_new(); 
	entry1 = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(entry1),"Name of file"); 
	gtk_entry_set_text(GTK_ENTRY(entry),"New name"); 
	gtk_container_add(GTK_CONTAINER(content_area), entry1);
	gtk_container_add(GTK_CONTAINER(content_area), entry);

	gtk_widget_show_all(dialog);
	gint result = gtk_dialog_run(GTK_DIALOG(dialog));

	char *old_name = (char *)malloc(MAXSZ); // variable for entered text for file name
	char *new_name = (char *)malloc(MAXSZ); // variable for entered text for new name
   
	switch(result)
	{
		case 0:
			old_name = (char *)gtk_entry_get_text(GTK_ENTRY(entry1));
			new_name = (char *)gtk_entry_get_text(GTK_ENTRY(entry));
			sprintf(file_name,"%s/%s",cwd,new_name);

			if(strlen(old_name) == 0 || old_name == NULL || strlen(new_name) == 0 || new_name == NULL || strcmp(old_name,"Name of file") == 0)//Invalid entry
			{
				GtkWidget *popup_window;
				GtkWidget *table;
				GtkWidget *label;
				GtkWidget *horiz_align;
			
				popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
				gtk_window_set_title (GTK_WINDOW (popup_window), "Input Error");
				gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
				gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
				gtk_window_set_decorated (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_widget_set_size_request (popup_window, 250, 60);
				gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
				gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);

				gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
				g_signal_connect (G_OBJECT (popup_window),
		               			    "focus-out-event",
        		      		 		    G_CALLBACK (on_popup_focus_out),
                		   				NULL);
				table = gtk_table_new (2,2,FALSE);
				gtk_container_add(GTK_CONTAINER(popup_window),table);
				label = gtk_label_new("\t\tInvalid filename!");

				horiz_align = gtk_alignment_new(0,0,0,0);
    	
				gtk_container_add(GTK_CONTAINER(horiz_align),label);
				gtk_table_attach(GTK_TABLE(table),horiz_align,1,2,0,1,GTK_FILL,GTK_FILL,0,0);
	
				gtk_widget_show_all (popup_window);
				gtk_widget_grab_focus (popup_window);
			}
			else if(access(file_name,F_OK) == 0)//File with the entered name already present. Try different name.
			{
				GtkWidget *popup_window;
				GtkWidget *table;
				GtkWidget *label;
				GtkWidget *horiz_align;
	
				popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
				gtk_window_set_title (GTK_WINDOW (popup_window), "Input Error");
				gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
				gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
				gtk_window_set_decorated (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_widget_set_size_request (popup_window, 250, 60);
				gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
				gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);
		
				gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
				g_signal_connect (G_OBJECT (popup_window),
        		       			    "focus-out-event",
        		      		 		    G_CALLBACK (on_popup_focus_out),
                		   				NULL);
				table = gtk_table_new (2,2,FALSE);
				gtk_container_add(GTK_CONTAINER(popup_window),table);
				label = gtk_label_new("\t\tFilename already present!");

				horiz_align = gtk_alignment_new(0,0,0,0);
    
				gtk_container_add(GTK_CONTAINER(horiz_align),label);
				gtk_table_attach(GTK_TABLE(table),horiz_align,1,2,0,1,GTK_FILL,GTK_FILL,0,0);
	
				gtk_widget_show_all (popup_window);
				gtk_widget_grab_focus (popup_window);
			}
			else
			{
				sprintf(user_input,"!rename %s %s",old_name,new_name);
				
				gtk_entry_set_text(GTK_ENTRY(app_state->command),user_input); 
				rename_cli(app_state,old_name,new_name);
				gtk_entry_set_text(GTK_ENTRY(app_state->command),"Command");
			}
	
		        break;

		case 1:
			break;
		default:
			break;
	}

	gtk_widget_destroy(dialog);	
	return;
}

/* Download file. Display dialog */
void down_serv_func(GtkWidget *widget,Appstate *app_state)
{
	GtkWidget *dialog;
	GtkWidget *entry;
	GtkWidget *content_area;

	char user_input[MAXSZ];

	dialog = gtk_dialog_new_with_buttons("Download file",GTK_WINDOW(app_state->window),GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT,NULL,NULL,NULL,NULL);
	gtk_window_set_default_size(GTK_WINDOW(dialog),350,100);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Ok", 0);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Cancel", 1);

	content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(entry),"Enter name"); 
	gtk_container_add(GTK_CONTAINER(content_area), entry);

	gtk_widget_show_all(dialog);
	gint result = gtk_dialog_run(GTK_DIALOG(dialog));

	char *entry_line; // variable for entered text

	switch(result)
	{
		case 0:
		        entry_line = (char *)gtk_entry_get_text(GTK_ENTRY(entry));

			if(strlen(entry_line) == 0 || entry_line == NULL || strcmp(entry_line,"Enter name") == 0)//Invalid name
			{
				GtkWidget *popup_window;
				GtkWidget *table;
				GtkWidget *label;
				GtkWidget *horiz_align;
		
				popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
				gtk_window_set_title (GTK_WINDOW (popup_window), "Input Error");
				gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
				gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
				gtk_window_set_decorated (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_widget_set_size_request (popup_window, 250, 60);
				gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
				gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);

				gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
				g_signal_connect (G_OBJECT (popup_window),
		               			    "focus-out-event",
        		      		 		    G_CALLBACK (on_popup_focus_out),
        		           				NULL);
				table = gtk_table_new (2,2,FALSE);
				gtk_container_add(GTK_CONTAINER(popup_window),table);
				label = gtk_label_new("\t\tInvalid filename!");
			
				horiz_align = gtk_alignment_new(0,0,0,0);
    		
				gtk_container_add(GTK_CONTAINER(horiz_align),label);
				gtk_table_attach(GTK_TABLE(table),horiz_align,1,2,0,1,GTK_FILL,GTK_FILL,0,0);
		
				gtk_widget_show_all (popup_window);
				gtk_widget_grab_focus (popup_window);
    		
			}
			else
			{
				bzero(user_input,MAXSZ);
				sprintf(user_input,"get %s",entry_line);
				gtk_entry_set_text(GTK_ENTRY(app_state->command),user_input); 
				down_serv(app_state,user_input);
		    		gtk_entry_set_text(GTK_ENTRY(app_state->command),"Command"); 
			}

        		break;
		case 1:
			break;
		default:
			break;
	}

	gtk_widget_destroy(dialog);//Destroy widget
	return;
}

/* Change directory on client. Display dialog*/
void cddir_cli_func(GtkWidget *widget,Appstate *app_state)
{
	GtkWidget *dialog;
	GtkWidget *entry;
	GtkWidget *content_area;

	char user_input[MAXSZ];

	dialog = gtk_dialog_new_with_buttons("Change Directory(Client)",GTK_WINDOW(app_state->window),GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT,NULL,NULL,NULL,NULL);
	gtk_window_set_default_size(GTK_WINDOW(dialog),350,100);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Ok", 0);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Cancel", 1);

	content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(entry),"Enter directory path"); 
    
	gtk_container_add(GTK_CONTAINER(content_area), entry);

	gtk_widget_show_all(dialog);
	gint result = gtk_dialog_run(GTK_DIALOG(dialog));

	char *entry_line; // variable for entered text

	switch(result)
	{	
		case 0:
			entry_line = (char *)gtk_entry_get_text(GTK_ENTRY(entry));

			if(strlen(entry_line) == 0 || entry_line == NULL || strcmp(entry_line,"Enter directory path") == 0)//Invalid name
			{
				GtkWidget *popup_window;
				GtkWidget *table;
				GtkWidget *label;
				GtkWidget *horiz_align;
			
				popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
				gtk_window_set_title (GTK_WINDOW (popup_window), "Input Error");
				gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
				gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
				gtk_window_set_decorated (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_widget_set_size_request (popup_window, 250, 60);
				gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
				gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);
		
				gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
				g_signal_connect (G_OBJECT (popup_window),
		               			    "focus-out-event",
		              		 		    G_CALLBACK (on_popup_focus_out),
        		           				NULL);
	
				table = gtk_table_new (2,2,FALSE);
				gtk_container_add(GTK_CONTAINER(popup_window),table);
				label = gtk_label_new("\t\t\tInvalid path!");

				horiz_align = gtk_alignment_new(0,0,0,0);
    
				gtk_container_add(GTK_CONTAINER(horiz_align),label);
				gtk_table_attach(GTK_TABLE(table),horiz_align,1,2,0,1,GTK_FILL,GTK_FILL,0,0);
	
				gtk_widget_show_all (popup_window);
				gtk_widget_grab_focus (popup_window);
    		
			}
			else
			{
				bzero(user_input,MAXSZ);
				sprintf(user_input,"!cd %s",entry_line);
				gtk_entry_set_text(GTK_ENTRY(app_state->command),user_input); 
				cli_cd_dir(app_state,user_input);
		    		gtk_entry_set_text(GTK_ENTRY(app_state->command),"Command");
			}

        		break;
		case 1:
			break;
		default:
			break;
	}

	gtk_widget_destroy(dialog);//Destroy widget
	return;
}

/* Change directory on server. Display dialog */
void cddir_serv_func(GtkWidget *widget,Appstate *app_state)
{
	GtkWidget *dialog;
	GtkWidget *entry;
	GtkWidget *content_area;

	char user_input[MAXSZ];

	dialog = gtk_dialog_new_with_buttons("Change Directory(Server)",GTK_WINDOW(app_state->window),GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT,NULL,NULL,NULL,NULL);
	gtk_window_set_default_size(GTK_WINDOW(dialog),350,100);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Ok", 0);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Cancel", 1);

	content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(entry),"Enter directory path"); 
    
	gtk_container_add(GTK_CONTAINER(content_area), entry);

	gtk_widget_show_all(dialog);
	gint result = gtk_dialog_run(GTK_DIALOG(dialog));

	char *entry_line; // variable for entered text

	switch(result)
	{
		case 0:
			entry_line = (char *)gtk_entry_get_text(GTK_ENTRY(entry));

			if(strlen(entry_line) == 0 || entry_line == NULL || strcmp(entry_line,"Enter directory path") == 0)//Invalid name
			{
				GtkWidget *popup_window;
				GtkWidget *table;
				GtkWidget *label;
				GtkWidget *horiz_align;
			
				popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
				gtk_window_set_title (GTK_WINDOW (popup_window), "Input Error");
				gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
				gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
				gtk_window_set_decorated (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_widget_set_size_request (popup_window, 250, 60);
				gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
				gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);
		
				gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
				g_signal_connect (G_OBJECT (popup_window),
               					    "focus-out-event",
              				 		    G_CALLBACK (on_popup_focus_out),
                   						NULL);
	
				table = gtk_table_new (2,2,FALSE);
				gtk_container_add(GTK_CONTAINER(popup_window),table);
				label = gtk_label_new("\t\t\tInvalid path!");
		
				horiz_align = gtk_alignment_new(0,0,0,0);
    	
				gtk_container_add(GTK_CONTAINER(horiz_align),label);
				gtk_table_attach(GTK_TABLE(table),horiz_align,1,2,0,1,GTK_FILL,GTK_FILL,0,0);
			
				gtk_widget_show_all (popup_window);
				gtk_widget_grab_focus (popup_window);
    		
			}
			else
			{
				bzero(user_input,MAXSZ);
				sprintf(user_input,"cd %s",entry_line);
				gtk_entry_set_text(GTK_ENTRY(app_state->command),user_input); 
				serv_cd_dir(app_state,user_input);
    				gtk_entry_set_text(GTK_ENTRY(app_state->command),"Command"); 
			}

        		break;

		case 1:
			break;
		default:
			break;
	}

	gtk_widget_destroy(dialog);
	return;
}

/* Present working directory client */
void pwd_cli_func(GtkWidget *widget,Appstate *app_state)
{
	if(app_state->status == 0)
	{
		no_connection(app_state);
		return;
	}

	if(app_state->running == 1)
	{
		already_running(app_state);
		return;
	}

	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
	app_state->running = 1;
	
	gtk_entry_set_text(GTK_ENTRY(app_state->command),"!pwd"); 
	
	char working_dir[MAXSZ];
	getcwd(working_dir,MAXSZ);	
	sprintf(buff,"You are in directory: \"%s\"\n\n",working_dir);
	print_buff(app_state);
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;
	gtk_entry_set_text(GTK_ENTRY(app_state->command),"Command"); 
	return;
}

/* Present working directory client */
void pwd_serv_func(GtkWidget *widget,Appstate *app_state)
{
	int no_of_bytes;
	if(app_state->status == 0)//No conection
	{
		no_connection(app_state);
		return;
	}

	if(app_state->running == 1)//Another command already running
	{
		already_running(app_state);
		return;
	}

	/* Set 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
	app_state->running = 1;
	gtk_entry_set_text(GTK_ENTRY(app_state->command),"pwd"); 

	char message_to_server[MAXSZ];
	char message_from_server[MAXSZ];

	bzero(message_to_server,MAXSZ);
	bzero(message_from_server,MAXSZ);
		
	sprintf(message_to_server,"PWD\r\n");
	sprintf(buff,"Command: PWD\nResponse: ");
	print_buff(app_state);
	
	/* Send request to server */
	send(app_state->sockfd,message_to_server,strlen(message_to_server),0);
		
	while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0)
	{
		message_from_server[no_of_bytes] = '\0';
		sprintf(buff,"%s",message_from_server);
		print_buff(app_state);
				
		if(strstr(message_from_server,"257 ") > 0 || strstr(message_from_server,"500 ") > 0 || strstr(message_from_server,"501 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"502 ") > 0 || strstr(message_from_server,"550 ") > 0)
			break;
	}
	sprintf(buff,"\n");
	print_buff(app_state);
	
	/* Reset 'running' variable */
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;
	gtk_entry_set_text(GTK_ENTRY(app_state->command),"Command"); 
	return;
}

/* Remove file on client. Display dialog */
void rmfile_cli_func(GtkWidget *widget,Appstate *app_state)
{
	GtkWidget *dialog;
	GtkWidget *entry;
	GtkWidget *content_area;

	char user_input[MAXSZ];

	dialog = gtk_dialog_new_with_buttons("Remove File(Client)",GTK_WINDOW(app_state->window),GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT,NULL,NULL,NULL,NULL);
	gtk_window_set_default_size(GTK_WINDOW(dialog),350,100);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Ok", 0);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Cancel", 1);

	content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(entry),"Enter filename"); 
    
	gtk_container_add(GTK_CONTAINER(content_area), entry);

	gtk_widget_show_all(dialog);
	gint result = gtk_dialog_run(GTK_DIALOG(dialog));

	char *entry_line; // variable for entered text

	switch(result)
	{
		case 0:
			entry_line = (char *)gtk_entry_get_text(GTK_ENTRY(entry));

			if(strlen(entry_line) == 0 || entry_line == NULL || strcmp(entry_line,"Enter filename") == 0)//Invalid name
			{
				GtkWidget *popup_window;
				GtkWidget *table;
				GtkWidget *label;
				GtkWidget *horiz_align;
			
				popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
				gtk_window_set_title (GTK_WINDOW (popup_window), "Input Error");
				gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
				gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
				gtk_window_set_decorated (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_widget_set_size_request (popup_window, 250, 60);
				gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
				gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);
		
				g_signal_connect (G_OBJECT (popup_window),
		               			    "focus-out-event",
        		      		 		    G_CALLBACK (on_popup_focus_out),
        		           				NULL);
		
				table = gtk_table_new (2,2,FALSE);
				gtk_container_add(GTK_CONTAINER(popup_window),table);
				label = gtk_label_new("\t\tInvalid filename!");
		
				horiz_align = gtk_alignment_new(0,0,0,0);
    
				gtk_container_add(GTK_CONTAINER(horiz_align),label);
				gtk_table_attach(GTK_TABLE(table),horiz_align,1,2,0,1,GTK_FILL,GTK_FILL,0,0);
			
				gtk_widget_show_all (popup_window);
				gtk_widget_grab_focus (popup_window);
    		
			}
			else
			{
				bzero(user_input,MAXSZ);
				sprintf(user_input,"!rm %s",entry_line);
				gtk_entry_set_text(GTK_ENTRY(app_state->command),user_input); 
				cli_rmfile(app_state,user_input);
    				gtk_entry_set_text(GTK_ENTRY(app_state->command),"Command");
			}
	
        		break;

		case 1:
			break;
		default:
			break;
	}

	gtk_widget_destroy(dialog);
	return;
}

/* Remove file on server. Display dialog */
void rmfile_serv_func(GtkWidget *widget,Appstate *app_state)
{
	GtkWidget *dialog;
	GtkWidget *entry;
	GtkWidget *content_area;

	char user_input[MAXSZ];

	dialog = gtk_dialog_new_with_buttons("Remove file(Server)",GTK_WINDOW(app_state->window),GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT,NULL,NULL,NULL,NULL);
	gtk_window_set_default_size(GTK_WINDOW(dialog),350,100);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Ok", 0);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Cancel", 1);

	content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(entry),"Enter filename"); 
    
	gtk_container_add(GTK_CONTAINER(content_area), entry);

	gtk_widget_show_all(dialog);
	gint result = gtk_dialog_run(GTK_DIALOG(dialog));

	char *entry_line; // variable for entered text

	switch(result)
	{
		case 0:
			entry_line = (char *)gtk_entry_get_text(GTK_ENTRY(entry));

			if(strlen(entry_line) == 0 || entry_line == NULL || strcmp(entry_line,"Enter filename") == 0)//Invalid name
			{
				GtkWidget *popup_window;
				GtkWidget *table;
				GtkWidget *label;
				GtkWidget *horiz_align;
			
				popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
				gtk_window_set_title (GTK_WINDOW (popup_window), "Input Error");
				gtk_container_set_border_width (GTK_CONTAINER (popup_window), 10);
				gtk_window_set_resizable (GTK_WINDOW (popup_window), FALSE);
				gtk_window_set_decorated (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_taskbar_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_window_set_skip_pager_hint (GTK_WINDOW (popup_window), TRUE);
				gtk_widget_set_size_request (popup_window, 250, 60);
				gtk_window_set_transient_for (GTK_WINDOW (popup_window), GTK_WINDOW (app_state->window));
				gtk_window_set_position (GTK_WINDOW (popup_window), GTK_WIN_POS_CENTER);
		
				gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
				g_signal_connect (G_OBJECT (popup_window),
        		       			    "focus-out-event",
        		      		 		    G_CALLBACK (on_popup_focus_out),
        		           				NULL);
	
				table = gtk_table_new (2,2,FALSE);
				gtk_container_add(GTK_CONTAINER(popup_window),table);
				label = gtk_label_new("\t\tInvalid filename!");
		
				horiz_align = gtk_alignment_new(0,0,0,0);
    		
				gtk_container_add(GTK_CONTAINER(horiz_align),label);
				gtk_table_attach(GTK_TABLE(table),horiz_align,1,2,0,1,GTK_FILL,GTK_FILL,0,0);
		
				gtk_widget_show_all (popup_window);
				gtk_widget_grab_focus (popup_window);
	    		
			}
			else
			{
				bzero(user_input,MAXSZ);
				sprintf(user_input,"rm %s",entry_line);
				gtk_entry_set_text(GTK_ENTRY(app_state->command),user_input); 
				serv_rmfile(app_state,user_input);
    				gtk_entry_set_text(GTK_ENTRY(app_state->command),"Command"); 
			}

        		break;

		case 1:
			break;
		default:
			break;
	}

	gtk_widget_destroy(dialog);
	return;
}

/* Main function */
int main( int argc,char *argv[] )
{

	Appstate app_state;
	app_state.status = 0;
	app_state.running = 0;
	app_state.seek = 0;
	app_state.seek_cli = 0;

	app_state.temp_file_descriptor = mkstemp(temporary_file);
	app_state.temp_file_descriptor_cli = mkstemp(temporary_file_cli);
	
	GtkWidget *table;
	
	/* IP Address */	
	GtkWidget *label;
	GtkWidget *horiz_align;

	/* Client */	
	GtkWidget *label1;
	GtkWidget *horiz_align1;

	/* Command/Response */	
	GtkWidget *label2;
	GtkWidget *horiz_align2;

	/* Server */	
	GtkWidget *label3;
	GtkWidget *horiz_align3;

	/* Username */	
	GtkWidget *label4;
	GtkWidget *horiz_align4;

	/* Password */	
	GtkWidget *label5;
	GtkWidget *horiz_align5;

	GtkWidget *close_button;
	GtkWidget *up;
	GtkWidget *move_up_cli;
	GtkWidget *select_button;
	GtkWidget *menubar, *client,*server, *help, *clientmenu, *servermenu, *helpmenu;//Menu bar
	GtkWidget *cr_dir_cli,*up_cli,*rename_cli,*rm_dir_cli,*cd_dir_cli,*pwd_cli,*rm_file_cli,*ls_l_cli;//Client menu
	GtkWidget *cr_dir_ser, *down_ser, *rename_ser,*rm_dir_ser,*cd_dir_ser,*pwd_ser,*rm_file_ser,*ls_l_ser;//Server menu
	GtkWidget *contents, *about;//Help menu
	GtkWidget *quit, *quitmenu;//Quit menu
	
	GtkWidget *scrolled_window;
	GtkWidget *scrolled_window1;
	GtkWidget *scrolled_window2;

	GtkAccelGroup *group;

	gtk_init (&argc, &argv);
	GdkColor color;
	gdk_color_parse("white smoke",&color);
	
	/* create a new window */
	app_state.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request( GTK_WIDGET (app_state.window), 660, 500);
	gtk_window_set_resizable(GTK_WINDOW(app_state.window), TRUE);
	gtk_window_set_title(GTK_WINDOW(app_state.window), "ST-FTP");
	gtk_widget_modify_bg(app_state.window,GTK_STATE_NORMAL,&color);
	gtk_window_set_position (GTK_WINDOW (app_state.window), GTK_WIN_POS_CENTER);
	gtk_window_set_icon(GTK_WINDOW(app_state.window),create_pixbuf("logo.png"));
	gtk_signal_connect(GTK_OBJECT (app_state.window), "delete_event",(GtkSignalFunc) gtk_exit, NULL);

	gtk_container_set_border_width (GTK_CONTAINER (app_state.window),10);

	table = gtk_table_new (12,4,FALSE);
	gtk_table_set_col_spacings(GTK_TABLE(table),0);
	gtk_container_add(GTK_CONTAINER(app_state.window),table);
	
	label = gtk_label_new("\nIP Address:");

	horiz_align = gtk_alignment_new(0,0,0,0);
    
	gtk_container_add(GTK_CONTAINER(horiz_align),label);
	gtk_table_attach(GTK_TABLE(table),horiz_align,0,1,1,2,GTK_FILL,0,1,1);
	label4 = gtk_label_new("\nUsername");

	horiz_align4 = gtk_alignment_new(0,0,0,0);
    
	gtk_container_add(GTK_CONTAINER(horiz_align4),label4);
	gtk_table_attach(GTK_TABLE(table),horiz_align4,1,2,1,2,GTK_FILL,0,1,1);
	label5 = gtk_label_new("\nPassword:");

	horiz_align5 = gtk_alignment_new(0,0,0,0);
    
	gtk_container_add(GTK_CONTAINER(horiz_align5),label5);
	gtk_table_attach(GTK_TABLE(table),horiz_align5,2,3,1,2,GTK_FILL,0,1,1);
	
	label1 = gtk_label_new("\nClient:");

	horiz_align1 = gtk_alignment_new(0,0,0,0);
    
	gtk_container_add(GTK_CONTAINER(horiz_align1),label1);
	gtk_table_attach(GTK_TABLE(table),horiz_align1,0,2,4,5,GTK_FILL,0,2,1);
	
	label3 = gtk_label_new("\nServer:");

	horiz_align3 = gtk_alignment_new(0,0,0,0);
    
	gtk_container_add(GTK_CONTAINER(horiz_align3),label3);
	gtk_table_attach(GTK_TABLE(table),horiz_align3,2,4,4,5,GTK_FILL,0,2,1);

	label2 = gtk_label_new("\nCommand/Response:");

	horiz_align2 = gtk_alignment_new(0,0,0,0);
    
	gtk_container_add(GTK_CONTAINER(horiz_align2),label2);
	gtk_table_attach(GTK_TABLE(table),horiz_align2,0,4,8,9,GTK_FILL,0,4,1);
	
	/* Menu bar */
	group = gtk_accel_group_new();	
	menubar = gtk_menu_bar_new ();
	client = gtk_menu_item_new_with_label ("Client");
	server = gtk_menu_item_new_with_label ("Server");
	help = gtk_menu_item_new_with_label ("Help");
	close_button = gtk_menu_item_new_with_label ("Quit");

	clientmenu = gtk_menu_new ();
	servermenu = gtk_menu_new ();
	helpmenu = gtk_menu_new ();
	quitmenu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (client), clientmenu);
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (server), servermenu);
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (help), helpmenu);
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (close_button), quitmenu);
	
	gtk_menu_shell_append (GTK_MENU_SHELL (menubar), client);
	gtk_menu_shell_append (GTK_MENU_SHELL (menubar), server);
	gtk_menu_shell_append (GTK_MENU_SHELL (menubar), help);
	gtk_menu_shell_append (GTK_MENU_SHELL (menubar), close_button);
	
	/* Create the Client menu content. */
	cr_dir_cli = gtk_menu_item_new_with_label("Create Directory");
	rm_dir_cli = gtk_menu_item_new_with_label("Remove Directory");
	up_cli = gtk_menu_item_new_with_label("Upload");
	rename_cli = gtk_menu_item_new_with_label("Rename");
	cd_dir_cli = gtk_menu_item_new_with_label("Change Directory");
	pwd_cli = gtk_menu_item_new_with_label("Working Directory");
	rm_file_cli = gtk_menu_item_new_with_label("Remove File");
	ls_l_cli = gtk_menu_item_new_with_label("List Files");
	gtk_menu_shell_append (GTK_MENU_SHELL (clientmenu), cr_dir_cli);
	gtk_menu_shell_append (GTK_MENU_SHELL (clientmenu), rm_dir_cli);
	gtk_menu_shell_append (GTK_MENU_SHELL (clientmenu), up_cli);
	gtk_menu_shell_append (GTK_MENU_SHELL (clientmenu), rename_cli);
	gtk_menu_shell_append (GTK_MENU_SHELL (clientmenu), cd_dir_cli);
	gtk_menu_shell_append (GTK_MENU_SHELL (clientmenu), pwd_cli);
	gtk_menu_shell_append (GTK_MENU_SHELL (clientmenu), rm_file_cli);
	gtk_menu_shell_append (GTK_MENU_SHELL (clientmenu), ls_l_cli);
	gtk_signal_connect(GTK_OBJECT(ls_l_cli),"activate",GTK_SIGNAL_FUNC(ls_cli_func),&app_state);
	gtk_signal_connect(GTK_OBJECT(cr_dir_cli),"activate",GTK_SIGNAL_FUNC(crdir_cli_func),&app_state);
	gtk_signal_connect(GTK_OBJECT(rm_dir_cli),"activate",GTK_SIGNAL_FUNC(rmdir_cli_func),&app_state);
	gtk_signal_connect(GTK_OBJECT(up_cli),"activate",GTK_SIGNAL_FUNC(file_select),&app_state);
	gtk_signal_connect(GTK_OBJECT(rename_cli),"activate",GTK_SIGNAL_FUNC(rename_cli_func),&app_state);
	gtk_signal_connect(GTK_OBJECT(cd_dir_cli),"activate",GTK_SIGNAL_FUNC(cddir_cli_func),&app_state);
	gtk_signal_connect(GTK_OBJECT(pwd_cli),"activate",GTK_SIGNAL_FUNC(pwd_cli_func),&app_state);
	gtk_signal_connect(GTK_OBJECT(rm_file_cli),"activate",GTK_SIGNAL_FUNC(rmfile_cli_func),&app_state);

	/* Create the Server menu content. */
	cr_dir_ser = gtk_menu_item_new_with_label("Create Directory");
	rm_dir_ser = gtk_menu_item_new_with_label("Remove Directory");
	down_ser = gtk_menu_item_new_with_label("Download");
	rename_ser = gtk_menu_item_new_with_label("Rename");
	cd_dir_ser = gtk_menu_item_new_with_label("Change Directory");
	pwd_ser = gtk_menu_item_new_with_label("Working Directory");
	rm_file_ser = gtk_menu_item_new_with_label("Remove File");
	ls_l_ser = gtk_menu_item_new_with_label("List Files");
	gtk_menu_shell_append (GTK_MENU_SHELL (servermenu), cr_dir_ser);
	gtk_menu_shell_append (GTK_MENU_SHELL (servermenu), rm_dir_ser);
	gtk_menu_shell_append (GTK_MENU_SHELL (servermenu), down_ser);
	gtk_menu_shell_append (GTK_MENU_SHELL (servermenu), rename_ser);
	gtk_menu_shell_append (GTK_MENU_SHELL (servermenu), cd_dir_ser);
	gtk_menu_shell_append (GTK_MENU_SHELL (servermenu), pwd_ser);
	gtk_menu_shell_append (GTK_MENU_SHELL (servermenu), rm_file_ser);
	gtk_menu_shell_append (GTK_MENU_SHELL (servermenu), ls_l_ser);
	gtk_signal_connect(GTK_OBJECT(ls_l_ser),"activate",GTK_SIGNAL_FUNC(ls_ser_func),&app_state);
	gtk_signal_connect(GTK_OBJECT(cr_dir_ser),"activate",GTK_SIGNAL_FUNC(crdir_serv_func),&app_state);
	gtk_signal_connect(GTK_OBJECT(rm_dir_ser),"activate",GTK_SIGNAL_FUNC(rmdir_serv_func),&app_state);
	gtk_signal_connect(GTK_OBJECT(down_ser),"activate",GTK_SIGNAL_FUNC(down_serv_func),&app_state);
	gtk_signal_connect(GTK_OBJECT(rename_ser),"activate",GTK_SIGNAL_FUNC(rename_serv_func),&app_state);
	gtk_signal_connect(GTK_OBJECT(cd_dir_ser),"activate",GTK_SIGNAL_FUNC(cddir_serv_func),&app_state);
	gtk_signal_connect(GTK_OBJECT(pwd_ser),"activate",GTK_SIGNAL_FUNC(pwd_serv_func),&app_state);
	gtk_signal_connect(GTK_OBJECT(rm_file_ser),"activate",GTK_SIGNAL_FUNC(rmfile_serv_func),&app_state);

	/* Create the Help menu content. */
	contents = gtk_image_menu_item_new_from_stock (GTK_STOCK_HELP, group);
	about = gtk_image_menu_item_new_from_stock (GTK_STOCK_ABOUT, group);
	gtk_menu_shell_append (GTK_MENU_SHELL (helpmenu), contents);
	gtk_menu_shell_append (GTK_MENU_SHELL (helpmenu), about);
	gtk_signal_connect(GTK_OBJECT(about),"activate",GTK_SIGNAL_FUNC(about_us_func),&app_state);
	gtk_signal_connect(GTK_OBJECT(contents),"activate",GTK_SIGNAL_FUNC(help_func),&app_state);
	
	/* Create the Quit menu content. */
	quit = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, group);
	gtk_menu_shell_append (GTK_MENU_SHELL (quitmenu), quit);
	gtk_signal_connect(GTK_OBJECT(quit),"activate",GTK_SIGNAL_FUNC(close_func),&app_state);

	gtk_table_attach(GTK_TABLE(table),menubar,0,4,0,1,GTK_FILL|GTK_EXPAND,0,4,1);
	
	/* Directory on server side */
	app_state.entry_dir = gtk_entry_new_with_max_length(1024);
	gtk_entry_set_text (GTK_ENTRY (app_state.entry_dir),"Working directory");
	gtk_entry_select_region (GTK_ENTRY (app_state.entry_dir),0, GTK_ENTRY(app_state.entry_dir)->text_length);

//	gtk_entry_set_editable(GTK_ENTRY(app_state.entry_dir),FALSE);
	gtk_table_attach(GTK_TABLE(table),app_state.entry_dir,2,3,5,6,GTK_FILL,0,1,1);
	/* Directory on client side */
	app_state.entry_dir_cli = gtk_entry_new_with_max_length(1024);
	gtk_entry_set_text (GTK_ENTRY (app_state.entry_dir_cli),"Working directory");
	gtk_entry_select_region (GTK_ENTRY (app_state.entry_dir_cli),0, GTK_ENTRY(app_state.entry_dir_cli)->text_length);
//	gtk_entry_set_editable(GTK_ENTRY(app_state.entry_dir_cli),FALSE);

	gtk_table_attach(GTK_TABLE(table),app_state.entry_dir_cli,0,1,5,6,GTK_FILL,0,1,1);

	/* IP Address */
	app_state.entry = gtk_entry_new_with_max_length(200);
	gtk_entry_set_text (GTK_ENTRY (app_state.entry),"IP Address");
	gtk_entry_select_region (GTK_ENTRY (app_state.entry),0, GTK_ENTRY(app_state.entry)->text_length);
	gtk_table_attach(GTK_TABLE(table),app_state.entry,0,1,2,3,GTK_FILL,0,1,1);
    
	/* Username */
	app_state.user = gtk_entry_new_with_max_length(100);
	gtk_entry_set_text (GTK_ENTRY (app_state.user),"Username");
	gtk_entry_select_region (GTK_ENTRY (app_state.user),0, GTK_ENTRY(app_state.user)->text_length);
	gtk_table_attach(GTK_TABLE(table),app_state.user,1,2,2,3,GTK_FILL,0,1,1);

	/* Password */
	app_state.password = gtk_entry_new_with_max_length(100);
	gtk_entry_set_text (GTK_ENTRY (app_state.password),"Password");
	gtk_entry_select_region (GTK_ENTRY (app_state.password),0, GTK_ENTRY(app_state.password)->text_length);
	gtk_entry_set_visibility(GTK_ENTRY(app_state.password),FALSE);
	gtk_table_attach(GTK_TABLE(table),app_state.password,2,3,2,3,GTK_FILL|GTK_EXPAND,0,1,1);

	/* Command */
	app_state.command = gtk_entry_new_with_max_length(1024);
	gtk_entry_set_text (GTK_ENTRY (app_state.command),"Command");
	gtk_entry_select_region (GTK_ENTRY (app_state.command),0, GTK_ENTRY(app_state.command)->text_length);
	gtk_table_attach(GTK_TABLE(table),app_state.command,0,3,3,4,GTK_FILL,0,3,1);
    
	/* Select file */
	app_state.fileselect = gtk_entry_new_with_max_length(4096);
	gtk_entry_set_text (GTK_ENTRY (app_state.fileselect),"Select file to upload");
	gtk_entry_select_region (GTK_ENTRY (app_state.fileselect),0, GTK_ENTRY(app_state.fileselect)->text_length);
	gtk_table_attach(GTK_TABLE(table),app_state.fileselect,0,3,10,11,GTK_FILL,0,3,1);
    
	/* Text view (Command and Response) */
	app_state.text_view = gtk_text_view_new_with_buffer(buffer);                                    
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(app_state.text_view),buffer);
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app_state.text_view));
	gtk_text_buffer_set_text(buffer,"Welcome to ST-FTP!",-1);

	scrolled_window = gtk_scrolled_window_new(NULL, NULL);              
	gtk_container_add(GTK_CONTAINER(scrolled_window), app_state.text_view);       
	gtk_table_attach(GTK_TABLE(table),scrolled_window,0,4,9,10,GTK_FILL|GTK_EXPAND,GTK_FILL|GTK_EXPAND,4,1);
  
	/* List store for server files*/
	app_state.view = create_view_and_model (&app_state);
	scrolled_window1 = gtk_scrolled_window_new(NULL, NULL);              
	gtk_container_add(GTK_CONTAINER(scrolled_window1),app_state.view);       
	gtk_table_attach(GTK_TABLE(table),scrolled_window1,2,4,6,7,GTK_FILL|GTK_EXPAND,GTK_FILL|GTK_EXPAND,2,1);
  

	/* List store for server files*/
	app_state.view1 = create_view_and_model1 (&app_state);
	scrolled_window2 = gtk_scrolled_window_new(NULL, NULL);              
	gtk_container_add(GTK_CONTAINER(scrolled_window2),app_state.view1);       
	gtk_table_attach(GTK_TABLE(table),scrolled_window2,0,2,6,7,GTK_FILL|GTK_EXPAND,GTK_FILL|GTK_EXPAND,2,1);
  
	/* Connect button */
	app_state.button = gtk_button_new_with_label("Connect");
	gtk_signal_connect(GTK_OBJECT(app_state.button), "clicked",GTK_SIGNAL_FUNC(connect_func),&app_state);
	gtk_widget_set_size_request(app_state.button,BUTTON_WIDTH,BUTTON_HEIGHT);
	gtk_table_attach(GTK_TABLE(table),app_state.button,3,4,2,3,0,0,1,1);
	
	/* Move up button */
	up = gtk_button_new_with_label("Move Up");
	gtk_signal_connect(GTK_OBJECT(up), "clicked",GTK_SIGNAL_FUNC(move_up_dir),&app_state);
	gtk_widget_set_size_request(up,BUTTON_WIDTH,BUTTON_HEIGHT);
	gtk_table_attach(GTK_TABLE(table),up,3,4,5,6,0,0,1,1);

	/* Move up button client */
	move_up_cli = gtk_button_new_with_label("Move Up");
	gtk_signal_connect(GTK_OBJECT(move_up_cli), "clicked",GTK_SIGNAL_FUNC(move_up_dir_cli),&app_state);
	gtk_widget_set_size_request(move_up_cli,BUTTON_WIDTH,BUTTON_HEIGHT);
	gtk_table_attach(GTK_TABLE(table),move_up_cli,1,2,5,6,0,0,1,1);


	/* Run button */
	app_state.ok_button = gtk_button_new_with_label("Run");
	gtk_signal_connect(GTK_OBJECT(app_state.ok_button), "clicked",GTK_SIGNAL_FUNC(run_command),&app_state);
	gtk_widget_set_size_request(app_state.ok_button,BUTTON_WIDTH,BUTTON_HEIGHT);
	gtk_table_attach(GTK_TABLE(table),app_state.ok_button,3,4,3,4,0,0,1,1);

	/* Select button */
	select_button = gtk_button_new_with_label("Select");
	gtk_signal_connect(GTK_OBJECT(select_button), "clicked",GTK_SIGNAL_FUNC(file_select),&app_state);
	gtk_widget_set_size_request(select_button,BUTTON_WIDTH,BUTTON_HEIGHT);
	gtk_table_attach(GTK_TABLE(table),select_button,3,4,10,11,0,0,1,1);
	
	/* Upload button */
	app_state.ok_file_button = gtk_button_new_with_label("Upload");
	gtk_signal_connect(GTK_OBJECT(app_state.ok_file_button), "clicked",GTK_SIGNAL_FUNC(func_upload),&app_state);
	gtk_widget_set_size_request(app_state.ok_file_button,BUTTON_WIDTH,BUTTON_HEIGHT);
	gtk_table_attach(GTK_TABLE(table),app_state.ok_file_button,3,4,11,12,GTK_EXPAND,0,1,1);
	
	gtk_window_add_accel_group (GTK_WINDOW (app_state.window), group);

	gtk_widget_show_all(app_state.window);

	gtk_main();
	return 0;
}	
