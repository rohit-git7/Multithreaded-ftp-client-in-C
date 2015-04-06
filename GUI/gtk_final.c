#include"header.h"
#include"print_buff.h"
#include"list_client.h"
#include"passive_connect.h"
#include"list_content.h"
#include"thread_get.h"
#include"put_content.h"
#include"put_unique.h"
#include"get_content.h"

gboolean on_popup_focus_out (GtkWidget *widget,GdkEventFocus *event,gpointer data)
{
	gtk_widget_destroy (widget);
	return TRUE;
}

void already_running(Appstate *app_state)
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

}

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

}

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

}


/*Get home directory of user executing program */
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

}


/*Get home directory of user executing program */
char * find_home_dir(char *file)
{
	struct passwd *pw;
	char *sudo_uid = getenv("SUDO_UID");
	pw = getpwuid(atoi(sudo_uid));
	
	return pw->pw_dir;

}

void rmdir_cli(Appstate *app_state,char *user_input)
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
	int dir_check = rmdir(user_input + 7);
	if(dir_check == -1)
		sprintf(buff,"Error: %s\n",strerror(errno));
	else
		sprintf(buff,"Directory successfully removed\n");
	
	print_buff(app_state);
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;
	return;
}

void rmdir_serv(Appstate *app_state,char *user_input)
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
	
	int no_of_bytes;
	char message_to_server[MAXSZ];
	char message_from_server[MAXSZ];

	bzero(message_from_server,MAXSZ);
	bzero(message_to_server,MAXSZ);

	sprintf(message_to_server,"RMD %s\r\n",user_input + 6);
	sprintf(buff,"Command: RMD %s\nResponse: ",user_input + 6);	
	print_buff(app_state);
	
	send(app_state->sockfd,message_to_server,strlen(message_to_server),0);
	while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0 )
	{
		message_from_server[no_of_bytes] = '\0';
		if(strncmp(message_from_server,"550",3) == 0)/* RMD fails*/
		{
			sprintf(buff,"Error: Removing directory failed.\n\n");
		}
		else
			sprintf(buff,"%s\n",message_from_server);
		print_buff(app_state);
				

		if(strstr(message_from_server,"250 ") > 0 || strstr(message_from_server,"530 ") > 0 || strstr(message_from_server,"500 ") > 0|| strstr(message_from_server,"501 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"502 ") > 0 || strstr(message_from_server,"550 ") > 0)
			break;	
	}
	
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");

	app_state->running = 0;
	return;

}

void mkdir_cli(Appstate *app_state,char *user_input)
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
	int dir_check = mkdir(user_input + 7,0755);
	if(dir_check == -1)
		sprintf(buff,"Error: %s\n",strerror(errno));
	else
		sprintf(buff,"Directory successfully created\n");
	
	print_buff(app_state);
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;
	return;
}

void rename_cli(Appstate *app_state,char *old_name, char *new_name)
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
	
	
	int old_fd;
	int new_fd;
	int no_of_bytes;

	int total;
	int p;

	char data[MAXSZ];
	char cwd[MAXSZ];
	char file_name[MAXSZ];

	bzero(cwd,MAXSZ);
	bzero(data,MAXSZ);
	bzero(file_name,MAXSZ);

	old_fd = open(old_name,O_RDONLY);
	if(old_fd == -1)	
	{
		sprintf(buff,"Error: Could not rename file!\n\n");
		print_buff(app_state);
		return;
	}	

	new_fd = open(new_name,O_WRONLY|O_CREAT|O_TRUNC,0644);
	
	if(new_fd == -1)	
	{
		sprintf(buff,"Error: Could not rename file!\n\n");
		print_buff(app_state);
		close(old_fd);
		return;
	}	

	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
	app_state->running = 1;

	while((no_of_bytes = read(old_fd,data,MAXSZ)) > 0)
	{	
		total = 0;
		while(total < no_of_bytes)
		{
			p = write(new_fd,data + total,no_of_bytes - total);
			total += p;
		}
	}

	getcwd(cwd,MAXSZ);
	sprintf(file_name,"%s/%s",cwd,old_name);
	if(unlink(file_name) != 0)
	{
		sprintf(buff,"Error: Could not rename file!\n\n");
		print_buff(app_state);
		close(old_fd);
		sprintf(file_name,"%s/%s",cwd,new_name);
		unlink(file_name);
		close(new_fd);
	}
	else
	{
		sprintf(buff,"File renamed successfully!\n\n");
		print_buff(app_state);
		close(old_fd);
		close(new_fd);
	}

	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;
	return;
}


void rename_serv(Appstate *app_state,char *old_name, char *new_name)
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
	
	int no_of_bytes;
	int temp = 0;
	char message_to_server[MAXSZ];
	char message_from_server[MAXSZ];

	bzero(message_from_server,MAXSZ);
	bzero(message_to_server,MAXSZ);

	sprintf(message_to_server,"RNFR %s\r\n",old_name);
	sprintf(buff,"Command: RNFR %s\nResponse: ",old_name);
	print_buff(app_state);
	
		
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
		if(strncmp(message_from_server,"550",3) == 0)/* RNTO fails*/
		{
			sprintf(buff,"Error: Renaming file failed.");
		}
		else
			sprintf(buff,"%s",message_from_server);
		print_buff(app_state);
				
				
		if(strstr(message_from_server,"553 ") > 0 ||strstr(message_from_server,"250 ") > 0 || strstr(message_from_server,"532 ") > 0 || strstr(message_from_server,"530 ") > 0 || strstr(message_from_server,"500 ") > 0|| strstr(message_from_server,"501 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"502 ") > 0 || strstr(message_from_server,"503 ") > 0)
			break;

	}
	sprintf(buff,"\n");
	print_buff(app_state);
	
	
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;
	return;
}

void down_serv(Appstate *app_state,char *user_input)
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

	char *address = (char *)malloc(MAXSZ);
	
	address = (char *)gtk_entry_get_text(GTK_ENTRY(app_state->entry));
	char *argv = address;	


	char *home_dir= find_home_dir(argv);

	clock_t start;
	clock_t end;
	double cpu_time;
	
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
	app_state->running = 1;

	start = clock();
	get_content(argv,user_input,app_state,home_dir);
	end = clock();
	cpu_time = ((double)(end - start))/CLOCKS_PER_SEC;
	sprintf(buff,"Time taken %lf\n\n",cpu_time);
	print_buff(app_state);

	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;

	return;
}

void serv_cd_dir(Appstate *app_state,char *user_input)
{
	int no_of_bytes;
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

	char message_to_server[MAXSZ];
	char message_from_server[MAXSZ];
	bzero(message_to_server,MAXSZ);
	bzero(message_from_server,MAXSZ);
	
	sprintf(message_to_server,"CWD %s\r\n",user_input + 3);
	sprintf(buff,"Command: CWD %s\nResponse: ",user_input+3);
	print_buff(app_state);
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
	bzero(buff,MAXSZ);
	
	sprintf(message_to_server,"PWD\r\n");
	
	send(app_state->sockfd,message_to_server,strlen(message_to_server),0);
		
	while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0)
	{
		message_from_server[no_of_bytes] = '\0';
		sprintf(buff,"You are in directory: %s",message_from_server + 4);
		print_buff(app_state);
				
		if(strstr(message_from_server,"257 ") > 0 || strstr(message_from_server,"500 ") > 0 || strstr(message_from_server,"501 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"502 ") > 0 || strstr(message_from_server,"550 ") > 0)
			break;
	}
	sprintf(buff,"\n");
	print_buff(app_state);

	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;
	return;
}

void cli_cd_dir(Appstate *app_state,char *user_input)
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

	char cwd[MAXSZ];	
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
	app_state->running = 1;
	
	if(chdir(user_input + 4) == 0)
	{
		getcwd(cwd,MAXSZ);
		sprintf(buff,"Directory successfully changed\nYou are in directory: \"%s\"\n\n",cwd);
	}			
	else
	{
		sprintf(buff,"Error: %s\n\n",strerror(errno));
	}
	
	print_buff(app_state);
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;

	return;
}

void cli_rmfile(Appstate *app_state,char *user_input)
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
	
	char cwd[MAXSZ];
	char file_path[MAXSZ];
	bzero(cwd,MAXSZ);
	bzero(file_path,MAXSZ);

	getcwd(cwd,MAXSZ);
	sprintf(file_path,"%s/%s",cwd,user_input + 4);

	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
	app_state->running = 1;
	
	if(unlink(file_path) == 0)
	{
		sprintf(buff,"File succesfully removed!\n\n");
	}			
	else
	{
		sprintf(buff,"Error: %s\n\n",strerror(errno));
	}
	
	print_buff(app_state);

	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;
	return;

}

void ls_cli_func(GtkWidget *widget,Appstate *app_state)
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
	
	char working_dir[MAXSZ];

	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
	app_state->running = 1;

	getcwd(working_dir,MAXSZ);	
	ls_l_dir(app_state,working_dir);

	app_state->running = 0;
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	return;

}

void ls_ser_func(GtkWidget *widget,Appstate *app_state)
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

	char user_input[]="ls -l";
	char *address = (char *)malloc(MAXSZ);
	
	address = (char *)gtk_entry_get_text(GTK_ENTRY(app_state->entry));
	char *argv = address;	


	list_content(argv,user_input,app_state);	
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;
	return;

}

void serv_rmfile(Appstate *app_state,char *user_input)
{

	int no_of_bytes;
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

	char message_to_server[MAXSZ];
	char message_from_server[MAXSZ];
	
	bzero(message_to_server,MAXSZ);
	bzero(message_from_server,MAXSZ);
	
	sprintf(message_to_server,"DELE %s\r\n",user_input + 3);
	sprintf(buff,"Command: DELE %s\nResponse: ",user_input+3);	
	print_buff(app_state);
	

	send(app_state->sockfd,message_to_server,strlen(message_to_server),0);
	while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0 )
	{
		message_from_server[no_of_bytes] = '\0';
		if(strncmp(message_from_server,"550",3) == 0)/* DEL fails*/
		{
			sprintf(buff,"Error: Removing file failed.\n\n");
		}
		else
			sprintf(buff,"%s\n",message_from_server);
		print_buff(app_state);
				
		if(strstr(message_from_server,"250 ") > 0 || strstr(message_from_server,"450 ") > 0 || strstr(message_from_server,"530 ") > 0 || strstr(message_from_server,"500 ") > 0|| strstr(message_from_server,"501 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"502 ") > 0 || strstr(message_from_server,"550 ") > 0)
			break;
			
	}

	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;
	return;
}

void run_command(GtkWidget *widget, Appstate *app_state)
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

	char *user_input = (char *)malloc(MAXSZ);
	char *address = (char *)malloc(MAXSZ);
	
	address = (char *)gtk_entry_get_text(GTK_ENTRY(app_state->entry));
	char *argv = address;	
	int count;

	clock_t start,end;
	double cpu_time;

	char message_from_server[MAXSZ];/* message from server*/
	char message_to_server[MAXSZ];/* message to server */
	char dir[MAXSZ];/* directory name */
	char working_dir[MAXSZ];
	char old_name[MAXSZ];
	char new_name[MAXSZ];

	char *home_dir= find_home_dir(argv);

	int no_of_bytes;
	
	/* Initialise strings */
	bzero(user_input,MAXSZ);
	bzero(message_to_server,MAXSZ);
	bzero(message_from_server,MAXSZ);
	bzero(dir,MAXSZ);
	bzero(working_dir,MAXSZ);
	bzero(old_name,MAXSZ);
	bzero(new_name,MAXSZ);

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
	
	}

	/* Current working directory on client side */
	if(strncmp(user_input,"!pwd ",5) == 0 || strcmp(user_input,"!pwd") == 0)
	{
		app_state->running = 1;
		gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Running");
		getcwd(working_dir,MAXSZ);	
		sprintf(buff,"%s\n\n",working_dir);
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


	
	/* Change directory on server side */
	if(strncmp(user_input,"cd ",3) == 0)
	{
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
	}

	/* Removing directory on server */
	if(strncmp(user_input,"rmdir ",6) == 0)
	{	
		rmdir_serv(app_state,user_input);
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
			if(strncmp(message_from_server,"550",3) == 0)/* DEL fails*/
			{
				sprintf(buff,"Error: Removing file failed.\n\n");
			}
			else
				sprintf(buff,"%s\n",message_from_server);
			print_buff(app_state);
				
			if(strstr(message_from_server,"250 ") > 0 || strstr(message_from_server,"450 ") > 0 || strstr(message_from_server,"530 ") > 0 || strstr(message_from_server,"500 ") > 0|| strstr(message_from_server,"501 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"502 ") > 0 || strstr(message_from_server,"550 ") > 0)
				break;
			
		}
	
	}
	
	app_state->running = 0;
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
		
	return;
}

void close_func(GtkWidget *widget, Appstate *app_state)
{
	int no_of_bytes;
	char message_to_server[MAXSZ];
	char message_from_server[MAXSZ];

	bzero(message_to_server,MAXSZ);
	bzero(message_from_server,MAXSZ);
	
	if(app_state->running == 1)
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
		if(app_state->status == 1)
		{
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
			usleep(500000);
		}	
		
		gtk_main_quit();
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


int connect_func(GtkWidget *widget, Appstate *app_state)
{
	int no_of_bytes;/* number of bytes sent or received from server */

	char message_from_server[MAXSZ];/* message from server*/
	char message_to_server[MAXSZ];/* message from server*/
	bzero(message_from_server,MAXSZ);
	bzero(message_to_server,MAXSZ);
	
	if(app_state->running == 1)
	{
		close_func(widget,app_state);
		return 2;
	}
	
	if(app_state->status == 1)
	{
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
	bzero(buff,MAXSZ);
	
	address = (char *)gtk_entry_get_text(GTK_ENTRY(app_state->entry));
	username = (char *)gtk_entry_get_text(GTK_ENTRY(app_state->user));
	password = (char *)gtk_entry_get_text(GTK_ENTRY(app_state->password));
	
	char *argv = address;
	
	/*Temporary Variables*/
	int connect_value;
	int ip_valid;
	int temp = MIN_VALUE;

	struct sockaddr_in serverAddress;/* client will connect on this */
	



	ip_valid = validate_ip(argv);/* Validate ip-address entered by user */
	
	if(ip_valid == MIN_VALUE)/* Invalid ipaddress */
	{
		sprintf(buff,"Error: Invalid ip-address\n");
		buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app_state->text_view));
		gtk_text_buffer_set_text(buffer,buff,-1);
		while(gtk_events_pending())
			gtk_main_iteration();
		return -1;
	}
	
//	home_dir = find_home_dir(argv[0]);/* Home directory of user executing the program */

	app_state->sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);/* Create socket */

	if(app_state->sockfd == -1)/* Error in socket creation */
	{
        	sprintf(buff,"Error: %s\n",strerror(errno)); 
		buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app_state->text_view));
		gtk_text_buffer_set_text(buffer,buff,-1);
		while(gtk_events_pending())
			gtk_main_iteration();
		return -1;
	}

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
		gtk_text_buffer_set_text(buffer,buff,-1);
		while(gtk_events_pending())
			gtk_main_iteration();
		return -1;
	}

	app_state->status = 1;
	gtk_button_set_label(GTK_BUTTON(app_state->button),"Disconnect");
	gtk_entry_set_editable(GTK_ENTRY(app_state->entry),FALSE);
	
	sprintf(buff,"Connected to %s.\n",argv);
	
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app_state->text_view));
	gtk_text_buffer_set_text(buffer,buff,-1);
	
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
			message_from_server[no_of_bytes] = '\0';

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

	return 1;
}



GdkPixbuf *create_pixbuf(const gchar *filename)
{
	GdkPixbuf *pixbuf;
	GError *error = NULL;
	pixbuf = gdk_pixbuf_new_from_file(filename,&error);

	return pixbuf;
}

void func_upload(GtkWidget *widget,Appstate *app_state)
{
	char *user_input = malloc(MAXSZ);
	char *argv = (char *)gtk_entry_get_text(GTK_ENTRY(app_state->entry));
	char *entry = (char *)gtk_entry_get_text(GTK_ENTRY(app_state->fileselect));

	char cur_dir[MAXSZ];	
	
	clock_t start;
	clock_t end;

	double cpu_time;

	int i;
	int j;	

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
		
		for(i = 0,j = 0;entry[i]!='\0';i++)
		{
			if(entry[i] == '/')
				j = i;
			while(gtk_events_pending())
				gtk_main_iteration();

		}

		for(i=0;i < j; i++)
		{
			cur_dir[i]=entry[i];
		}
		cur_dir[i]='\0';

		sprintf(user_input,"uniqput %s",entry + j + 1);
		if(chdir(cur_dir) == -1)
			return;
		start = clock();
		app_state->running = 1;
		gtk_button_set_label(GTK_BUTTON(app_state->ok_file_button),"Uploading");
		put_unique(argv,user_input,app_state);
		gtk_button_set_label(GTK_BUTTON(app_state->ok_file_button),"Upload");
		
		app_state->running = 0;
		end = clock();
		cpu_time = ((double)(end - start))/CLOCKS_PER_SEC;
		sprintf(buff,"Time taken %lf\n\n",cpu_time);
		print_buff(app_state);
		
	}
	gtk_entry_set_text(GTK_ENTRY(app_state->fileselect),"Select file to upload"); 
	
	return;
}


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
	
}

void mkdir_serv(Appstate *app_state,char *user_input)
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
	
	int no_of_bytes;
	char message_to_server[MAXSZ];
	char message_from_server[MAXSZ];

	bzero(message_from_server,MAXSZ);
	bzero(message_to_server,MAXSZ);

	sprintf(message_to_server,"MKD %s\r\n",user_input + 6);
	sprintf(buff,"Command: MKD %s\nResponse: ",user_input+6);	
	print_buff(app_state);
	
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
	
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;
	return;

}


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

        if(strlen(entry_line) == 0 || entry_line == NULL || strcmp(entry_line,"Enter name") == 0)
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
		mkdir_cli(app_state,user_input);
	}

        break;

    case 1:
		break;
    default:
		break;
    }

    gtk_widget_destroy(dialog);

}

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

        if(strlen(entry_line) == 0 || entry_line == NULL ||strcmp(entry_line,"Enter name") == 0)
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
		mkdir_cli(app_state,user_input);
	}

        break;

    case 1:
		break;
    default:
		break;
	}

	gtk_widget_destroy(dialog);	
}

void rename_serv_func(GtkWidget *widget,Appstate *app_state)
{
    GtkWidget *dialog;
    GtkWidget *entry;
    GtkWidget *entry1;
    GtkWidget *content_area;

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

        if(strlen(old_name) == 0 || old_name == NULL || strlen(new_name) == 0 || new_name == NULL || strcmp(old_name,"Name of file") == 0)
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
		rename_serv(app_state,old_name,new_name);
	}

        break;

    case 1:
		break;
    default:
		break;
	}

	gtk_widget_destroy(dialog);	
}

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

        if(strlen(entry_line) == 0 || entry_line == NULL || strcmp(entry_line,"Enter name") == 0)
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
		mkdir_serv(app_state,user_input);
	}

        break;

    case 1:
		break;
    default:
		break;
    }

    gtk_widget_destroy(dialog);
}


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

        if(strlen(entry_line) == 0 || entry_line == NULL || strcmp(entry_line,"Enter name") == 0)
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
		rmdir_serv(app_state,user_input);
	}

        break;

    case 1:
		break;
    default:
		break;
    }

    gtk_widget_destroy(dialog);
}
	

void rename_cli_func(GtkWidget *widget,Appstate *app_state)
{
    GtkWidget *dialog;
    GtkWidget *entry;
    GtkWidget *entry1;
    GtkWidget *content_area;

    char cwd[MAXSZ];
    char file_name[MAXSZ];
    bzero(cwd,MAXSZ);
    bzero(file_name,MAXSZ);

    getcwd(cwd,MAXSZ);

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

    char *old_name = (char *)malloc(MAXSZ); // variable for entered text
    char *new_name = (char *)malloc(MAXSZ); // variable for entered text
   
    switch(result)
    {
    case 0:
        old_name = (char *)gtk_entry_get_text(GTK_ENTRY(entry1));
        new_name = (char *)gtk_entry_get_text(GTK_ENTRY(entry));
    	sprintf(file_name,"%s/%s",cwd,new_name);

        if(strlen(old_name) == 0 || old_name == NULL || strlen(new_name) == 0 || new_name == NULL || strcmp(old_name,"Name of file") == 0)
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
	else if(access(file_name,F_OK) == 0)
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
		label = gtk_label_new("\t\tFile already present!");

		horiz_align = gtk_alignment_new(0,0,0,0);
    
		gtk_container_add(GTK_CONTAINER(horiz_align),label);
		gtk_table_attach(GTK_TABLE(table),horiz_align,1,2,0,1,GTK_FILL,GTK_FILL,0,0);
	
		gtk_widget_show_all (popup_window);
		gtk_widget_grab_focus (popup_window);
	}
	else
		rename_cli(app_state,old_name,new_name);
		

        break;

    case 1:
		break;
    default:
		break;
	}

	gtk_widget_destroy(dialog);	
}


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

        if(strlen(entry_line) == 0 || entry_line == NULL || strcmp(entry_line,"Enter name") == 0)
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
		down_serv(app_state,user_input);
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

        if(strlen(entry_line) == 0 || entry_line == NULL || strcmp(entry_line,"Enter directory path") == 0)
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
		cli_cd_dir(app_state,user_input);
	}

        break;

    case 1:
		break;
    default:
		break;
    }

    gtk_widget_destroy(dialog);
}

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

        if(strlen(entry_line) == 0 || entry_line == NULL || strcmp(entry_line,"Enter directory path") == 0)
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
		serv_cd_dir(app_state,user_input);
	}

        break;

    case 1:
		break;
    default:
		break;
    }

    gtk_widget_destroy(dialog);
}

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

	char working_dir[MAXSZ];
	getcwd(working_dir,MAXSZ);	
	sprintf(buff,"%s\n\n",working_dir);
	print_buff(app_state);
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;
	return;
}

void pwd_serv_func(GtkWidget *widget,Appstate *app_state)
{
	int no_of_bytes;
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

	char message_to_server[MAXSZ];
	char message_from_server[MAXSZ];
	bzero(message_to_server,MAXSZ);
	bzero(message_from_server,MAXSZ);
		
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
	gtk_button_set_label(GTK_BUTTON(app_state->ok_button),"Run");
	app_state->running = 0;
	return;
}

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

        if(strlen(entry_line) == 0 || entry_line == NULL || strcmp(entry_line,"Enter filename") == 0)
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
		cli_rmfile(app_state,user_input);
	}

        break;

    case 1:
		break;
    default:
		break;
    }

    gtk_widget_destroy(dialog);
}

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

        if(strlen(entry_line) == 0 || entry_line == NULL || strcmp(entry_line,"Enter filename") == 0)
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
		serv_rmfile(app_state,user_input);
	}

        break;

    case 1:
		break;
    default:
		break;
    }

    gtk_widget_destroy(dialog);
}


int main( int argc,char *argv[] )
{

	Appstate app_state;
	app_state.status = 0;
	app_state.running = 0;
	GtkWidget *table;
	GtkWidget *label1;
	GtkWidget *horiz_align1;
	GtkWidget *label;
	GtkWidget *horiz_align;

	GtkWidget *close_button;
	GtkWidget *select_button;
	GtkWidget *menubar, *client,*server, *help, *clientmenu, *servermenu, *helpmenu;
	GtkWidget *cr_dir_cli,*up_cli,*rename_cli,*rm_dir_cli,*cd_dir_cli,*pwd_cli,*rm_file_cli,*ls_l_cli;
	GtkWidget *cr_dir_ser, *down_ser, *rename_ser,*rm_dir_ser,*cd_dir_ser,*pwd_ser,*rm_file_ser,*ls_l_ser;
	GtkWidget *contents, *about;
	GtkWidget *quit, *quitmenu;
	
	GtkWidget *scrolled_window;
	GtkAccelGroup *group;

	gdk_threads_init();
	gdk_threads_enter();
	gtk_init (&argc, &argv);
	GdkColor color;
	gdk_color_parse("white smoke",&color);
	
  /* create a new window */
	app_state.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request( GTK_WIDGET (app_state.window), 660, 480);
	gtk_window_set_resizable(GTK_WINDOW(app_state.window), TRUE);
	gtk_window_set_title(GTK_WINDOW(app_state.window), "ST-FTP");
	gtk_widget_modify_bg(app_state.window,GTK_STATE_NORMAL,&color);
	gtk_window_set_position (GTK_WINDOW (app_state.window), GTK_WIN_POS_CENTER);
	gtk_window_set_icon(GTK_WINDOW(app_state.window),create_pixbuf("ftp-lo-new.png"));
	gtk_signal_connect(GTK_OBJECT (app_state.window), "delete_event",(GtkSignalFunc) gtk_exit, NULL);

	gtk_container_set_border_width (GTK_CONTAINER (app_state.window),10);

	table = gtk_table_new (8,8,FALSE);
	gtk_table_set_col_spacings(GTK_TABLE(table),0);
//	gtk_widget_modify_bg(table,GTK_STATE_NORMAL,&color);
	gtk_container_add(GTK_CONTAINER(app_state.window),table);
	label = gtk_label_new("\nIP Address:\t\t\t          Username: \t\t\t\t     Password:");

	horiz_align = gtk_alignment_new(0,0,0,0);
    
	gtk_container_add(GTK_CONTAINER(horiz_align),label);
	gtk_table_attach(GTK_TABLE(table),horiz_align,0,6,1,2,GTK_FILL,0,6,1);
	
	label1 = gtk_label_new("\nCommand/Response:");

	horiz_align1 = gtk_alignment_new(0,0,0,0);
    
	gtk_container_add(GTK_CONTAINER(horiz_align1),label1);
	gtk_table_attach(GTK_TABLE(table),horiz_align1,0,6,4,5,GTK_FILL,0,6,1);

	group = gtk_accel_group_new();	
	menubar = gtk_menu_bar_new ();
//	gdk_color_parse("black",&color);
//	gtk_widget_modify_bg(menubar,GTK_STATE_NORMAL,&color);
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
	
	quit = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, group);
	gtk_menu_shell_append (GTK_MENU_SHELL (quitmenu), quit);
	gtk_signal_connect(GTK_OBJECT(quit),"activate",GTK_SIGNAL_FUNC(close_func),&app_state);

	gtk_table_attach(GTK_TABLE(table),menubar,0,6,0,1,GTK_FILL|GTK_EXPAND,0,6,1);

	
	app_state.entry = gtk_entry_new_with_max_length(20);
	gtk_entry_set_text (GTK_ENTRY (app_state.entry),"IP Address");
	gtk_entry_select_region (GTK_ENTRY (app_state.entry),0, GTK_ENTRY(app_state.entry)->text_length);
	gtk_table_attach(GTK_TABLE(table),app_state.entry,0,1,2,3,GTK_FILL,0,1,1);
    
	app_state.user = gtk_entry_new_with_max_length(50);
	gtk_entry_set_text (GTK_ENTRY (app_state.user),"Username");
	gtk_entry_select_region (GTK_ENTRY (app_state.user),0, GTK_ENTRY(app_state.user)->text_length);
	gtk_table_attach(GTK_TABLE(table),app_state.user,1,2,2,3,GTK_FILL,0,1,1);

	app_state.password = gtk_entry_new_with_max_length(50);
	gtk_entry_set_text (GTK_ENTRY (app_state.password),"Password");
	gtk_entry_select_region (GTK_ENTRY (app_state.password),0, GTK_ENTRY(app_state.password)->text_length);
	gtk_entry_set_visibility(GTK_ENTRY(app_state.password),FALSE);
	gtk_table_attach(GTK_TABLE(table),app_state.password,2,3,2,3,GTK_FILL,0,1,1);

	app_state.command = gtk_entry_new_with_max_length(200);
	gtk_entry_set_text (GTK_ENTRY (app_state.command),"Command");
	gtk_entry_select_region (GTK_ENTRY (app_state.command),0, GTK_ENTRY(app_state.command)->text_length);
	gtk_table_attach(GTK_TABLE(table),app_state.command,0,3,3,4,GTK_FILL,0,2,1);
    
	app_state.fileselect = gtk_entry_new_with_max_length(4096);
	gtk_entry_set_text (GTK_ENTRY (app_state.fileselect),"Select file to upload");
	gtk_entry_select_region (GTK_ENTRY (app_state.fileselect),0, GTK_ENTRY(app_state.fileselect)->text_length);
	gtk_table_attach(GTK_TABLE(table),app_state.fileselect,0,3,6,7,GTK_FILL,0,2,1);
    
	
	app_state.text_view = gtk_text_view_new_with_buffer(buffer);                                    
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(app_state.text_view),buffer);
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app_state.text_view));
	gtk_text_buffer_set_text(buffer,"\t\t\t\t\t\t\t\t\tWelcome to ST-FTP!",-1);

	scrolled_window = gtk_scrolled_window_new(NULL, NULL);              
	gtk_container_add(GTK_CONTAINER(scrolled_window), app_state.text_view);       
	gtk_table_attach(GTK_TABLE(table),scrolled_window,0,6,5,6,GTK_FILL,GTK_FILL|GTK_EXPAND,6,1);


	app_state.button = gtk_button_new_with_label("Connect");
	gtk_signal_connect(GTK_OBJECT(app_state.button), "clicked",GTK_SIGNAL_FUNC(connect_func),&app_state);
	gtk_widget_set_size_request(app_state.button,BUTTON_WIDTH,BUTTON_HEIGHT);
	gtk_table_attach(GTK_TABLE(table),app_state.button,3,6,2,3,GTK_FILL,0,3,1);

	app_state.ok_button = gtk_button_new_with_label("Run");
	gtk_signal_connect(GTK_OBJECT(app_state.ok_button), "clicked",GTK_SIGNAL_FUNC(run_command),&app_state);
	gtk_widget_set_size_request(app_state.ok_button,BUTTON_WIDTH,BUTTON_HEIGHT);
	gtk_table_attach(GTK_TABLE(table),app_state.ok_button,3,6,3,4,GTK_FILL,0,3,1);

	select_button = gtk_button_new_with_label("Select");
	gtk_signal_connect(GTK_OBJECT(select_button), "clicked",GTK_SIGNAL_FUNC(file_select),&app_state);
	gtk_widget_set_size_request(select_button,BUTTON_WIDTH,BUTTON_HEIGHT);
	gtk_table_attach(GTK_TABLE(table),select_button,3,6,6,7,GTK_FILL,0,3,1);
	
	app_state.ok_file_button = gtk_button_new_with_label("Upload");
	gtk_signal_connect(GTK_OBJECT(app_state.ok_file_button), "clicked",GTK_SIGNAL_FUNC(func_upload),&app_state);
	gtk_widget_set_size_request(app_state.ok_file_button,BUTTON_WIDTH,BUTTON_HEIGHT);
	gtk_table_attach(GTK_TABLE(table),app_state.ok_file_button,3,6,7,8,GTK_FILL|GTK_EXPAND,0,3,1);
	
	gtk_window_add_accel_group (GTK_WINDOW (app_state.window), group);


	gtk_widget_show_all(app_state.window);

	gtk_main();
	gdk_threads_leave();
	
	return 0;
}
