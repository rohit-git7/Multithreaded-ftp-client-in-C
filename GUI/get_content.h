
/*
Download files from server.
*/

void get_content(char *arg,char *user_input,Appstate *app_state,char *home_dir)
{
	
	/* Temporary variables*/
	int no_of_bytes;
	int port;	
	int newsockfd;
	int i;
	int j = 0;
	int count = 0;
	int size;
	int no_of_threads;
	int total;
	int partial_bytes;
	int filehandle;
	int fd;

	char message_from_server[MAXSZ];
	char message_to_server[MAXSZ];
	char file[MAXSZ];// File name
	char file_name[MAXSZ];// File name with instruction to server
	char file_home_dir[MAXSZ];// Location of file i.e. User's home directory(Complete path to file).
	char data[MAXSZ];// Data transfer
	char pwd[MAXSZ];
	char size_file[MAXSZ];// size of file
	char temp_file[MAXSZ];//temporary files created on client side

	struct stat file_buff;

	/* Initialise all the character arrays */
	bzero(message_from_server,MAXSZ);
	bzero(message_to_server,MAXSZ);
	bzero(file_name,MAXSZ);
	bzero(file,MAXSZ);
	bzero(file_home_dir,MAXSZ);
	bzero(data,MAXSZ);
	bzero(pwd,MAXSZ);
	bzero(size_file,MAXSZ);	
	bzero(temp_file,MAXSZ);
	bzero(buff,MAXSZ1);

	/* Getting file size from server*/
	sprintf(size_file,"SIZE %s\r\n",user_input + 4);
	send(app_state->sockfd,size_file,strlen(size_file),0);

	while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0)
	{
		message_from_server[no_of_bytes] = '\0';
		while(gtk_events_pending())
			gtk_main_iteration();
		if(strstr(message_from_server,"213 ") > 0 || strstr(message_from_server,"501 ") > 0 ||strstr(message_from_server,"500 ") > 0 ||strstr(message_from_server,"502 ") > 0 ||strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"550 ") > 0)
			break;
	}
	
	size = atoi(message_from_server + 4);/* Convertin string to integer */

	/* Getting current working dirctory */
	send(app_state->sockfd,"PWD\r\n",5,0);

	while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0)
	{
		message_from_server[no_of_bytes] = '\0';
		while(gtk_events_pending())
			gtk_main_iteration();
		if(strstr(message_from_server,"257 ") > 0 || strstr(message_from_server,"501 ") > 0 ||strstr(message_from_server,"500 ") > 0 ||strstr(message_from_server,"502 ") > 0 ||strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"550 ") > 0)
			break;
	}
	
	/* Exract current working directory name from the message */
	for(i = 0; message_from_server[i]!='\0';i++)
	{
		while(gtk_events_pending())
			gtk_main_iteration();
		if(message_from_server[i] == '\"' && count == 1)
		{
			break;
		}
		
		if(count == 1)
		{
			pwd[j++] = message_from_server[i];
		}
		
		if(message_from_server[i] == '\"' && count == 0)
		{
			count = 1;
		}

	}	
	pwd[j] = '\0';


	/* Tell server to change to BINARY mode */
	send(app_state->sockfd,"TYPE I\r\n",8,0);
	sprintf(buff,"Command: TYPE I\nResponse: ");
	print_buff(app_state);

	while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0)
	{
		message_from_server[no_of_bytes] = '\0';
		sprintf(buff,"%s",message_from_server);
		print_buff(app_state);

		if(strstr(message_from_server,"200 ") > 0 || strstr(message_from_server,"501 ") > 0 ||strstr(message_from_server,"500 ") > 0 ||strstr(message_from_server,"504 ") > 0 ||strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"530 ") > 0)
			break;
	}
	sprintf(buff,"\n");	
	print_buff(app_state);

	if(strstr(message_from_server,"501 ") > 0 ||strstr(message_from_server,"500 ") > 0 ||strstr(message_from_server,"504 ") > 0 ||strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"530 ") > 0)
		return;	
	
	/* Send request for PASSIVE connection */	
	send(app_state->sockfd,passive,strlen(passive),0);
	sprintf(buff,"Command: %sResponse: ",passive);
	print_buff(app_state);
	
	while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0)
	{
		message_from_server[no_of_bytes] = '\0';
		sprintf(buff,"%s",message_from_server);
		print_buff(app_state);
	
		if(strstr(message_from_server,"227 ") > 0 || strstr(message_from_server,"501 ") > 0 ||strstr(message_from_server,"500 ") > 0 ||strstr(message_from_server,"502 ") > 0 ||strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"530 ") > 0)
			break;
	}
	
	sprintf(buff,"\n");
	print_buff(app_state);
	
	if(strstr(message_from_server,"501 ") > 0 ||strstr(message_from_server,"500 ") > 0 ||strstr(message_from_server,"502 ") > 0 ||strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"530 ") > 0)
		return;
		
	/* Server accepts request and sends PORT variables */
	if(strncmp(message_from_server,"227",3) == 0)
	{
		/* Generate a PORT number using PORT variables */
		port = passive_port_number(message_from_server); 
	
		/* Connect to server using another PORT for file transfers */
		newsockfd = func_to_connect_passive(arg,port);
	//	fcntl(newsockfd,F_SETFL,FNDELAY);
	
		sprintf(file,"%s",user_input + 4);

		/* Send file name */
		sprintf(file_name,"RETR %s\r\n",user_input + 4);	
		sprintf(buff,"Command: RETR %s\nResponse: ",user_input + 4);
		print_buff(app_state);
	
		send(app_state->sockfd,file_name,strlen(file_name),0);
				
		while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0)
		{
			message_from_server[no_of_bytes] = '\0';
			sprintf(buff,"%s",message_from_server);
			print_buff(app_state);
	
			if(strstr(message_from_server,"425 ") > 0|| strstr(message_from_server,"125 ") > 0 ||strstr(message_from_server,"150 ") > 0 || strstr(message_from_server,"501 ") > 0 ||strstr(message_from_server,"500 ") > 0 ||strstr(message_from_server,"550 ") > 0 ||strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"530 ") > 0)
				break;
		}
		sprintf(buff,"\n");
		print_buff(app_state);
	
		
		/* Permission Denied */
		if(strncmp(message_from_server,"550",3) == 0 || strncmp(message_from_server,"425",3) == 0)
		{
			close(newsockfd);
				return;
		}
		else
		{
			sprintf(file_home_dir,"%s/%s",home_dir,file);
		
			/* Create file on client system */	
			filehandle = open(file_home_dir,O_CREAT|O_WRONLY|O_TRUNC,0644);			
			
			/* Decide total number of threads to be created */
			if(size <= 5000)
				no_of_threads = 1;
			else if(size > 5000 && size < 100000)
				no_of_threads = 4;
			else
				no_of_threads = 10;
		
			if(no_of_threads > 1)// Multithreading
			{
				/* initilise structure for each thread */
				struct userdetails user[no_of_threads];
				pthread_t thre[no_of_threads];
				pthread_attr_t attr;
				pthread_attr_init(&attr);
			//	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
		

				for(i = 0;i < no_of_threads;i++)// Initialise structures
				{
					strcpy(user[i].arg,arg);
					strcpy(user[i].home_dir,home_dir);	
					strcpy(user[i].user_input,user_input);
					strcpy(user[i].pwd,pwd);	
					if(size % no_of_threads == 0)
						user[i].size = size/no_of_threads;
					else	
						user[i].size = size/no_of_threads + 1;
					while(gtk_events_pending())
						gtk_main_iteration();
				}
		
				for( i = 0;i < no_of_threads;i++)// Create threads
			        {
					user[i].t_id = i;
        		        	pthread_create(&thre[i],&attr,function,(void *)&user[i]);
					
					while(gtk_events_pending())
						gtk_main_iteration();
       			 	}
				
				sprintf(buff,"Downloading...\n");
				print_buff(app_state);
	
				pthread_attr_destroy(&attr);
			       /* for(i = 0;i < no_of_threads;i++)// Wait for completion of all threads
        			{
					while(gtk_events_pending())
						gtk_main_iteration();
       	         			pthread_join(thre[i],NULL);
        			}
			*/
				i = 0;
				total = 0;
				while(1)
				{
					if(i >= no_of_threads)
						break;
					while(gtk_events_pending())
						gtk_main_iteration();
					sprintf(temp_file,"%s/%s%d",home_dir,user_input + 4,i);
					if(lstat(temp_file,&file_buff)== -1)
						continue;
						
					if(i < (no_of_threads -1))
					{
						if(user[i].size == (int)file_buff.st_size)
						{
							total += (int)file_buff.st_size;
							i++;

						}	
					}
					else
					{
						if(file_buff.st_size == (size - total))
						{
							i++;
						}
					}
				}
				sprintf(buff,"Downloading Completed\n");
				print_buff(app_state);
				
				close(newsockfd);// Close passive connection
				while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0)) > 0)//Server might send an error message. We can ignore this.
				{
					message_from_server[no_of_bytes] = '\0';
					while(gtk_events_pending())
						gtk_main_iteration();
					if(strstr(message_from_server,"451 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"226 ") > 0 || strstr(message_from_server,"426 ") > 0)
						break;
				}		
			
				sprintf(buff,"Combining data...\n");
				print_buff(app_state);
	
				for(i = 0; i < no_of_threads; i++)//Joining temporary files
				{
					sprintf(temp_file,"%s/%s%d",home_dir,user_input + 4,i);
					fd = open(temp_file,O_RDONLY);
						
					while((no_of_bytes = read(fd,data,MAXSZ)) > 0)
					{	
						data[no_of_bytes] = '\0';
						total = 0;
						while(total < no_of_bytes)
						{
							while(gtk_events_pending())
								gtk_main_iteration();
                	        			partial_bytes = write(filehandle,data + total,no_of_bytes - total);
							total += partial_bytes;
						}
					}
					close(fd);
					unlink(temp_file);
				}
				close(filehandle);
				sprintf(buff,"226 Transfer completed\n\n");
				print_buff(app_state);
	
	
			}
			else//if file size is small. No need of multithreading
			{
				
				while((no_of_bytes = recv(newsockfd,data,MAXSZ,0))>0)//get data on passive connection
				{
					while(gtk_events_pending())
						gtk_main_iteration();
					total = 0;
					/* For partial write operations */
					while(total < no_of_bytes)
                			{
                	        		partial_bytes = write(filehandle,data + total,no_of_bytes - total);
                       		 		total += partial_bytes;
					}
				}
					
				/* Close PASSIVE socket and file */
				close(newsockfd);
				close(filehandle);
				while((no_of_bytes = recv(app_state->sockfd,message_from_server,MAXSZ,0))>0)
				{
					message_from_server[no_of_bytes] = '\0';
					sprintf(buff,"%s",message_from_server);
					print_buff(app_state);
	
					if(strstr(message_from_server,"226 ") > 0 || strstr(message_from_server,"426 ") > 0)
						break;
				}
				sprintf(buff,"\n");
				print_buff(app_state);
	
			}
		}	
	}
}
		
