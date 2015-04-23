/*
Each thread will establish an instruction connection and a passive connection to server.
*/
void *function(void *point)
{
	int sockfd;
	int temp = 0;
	int no_of_bytes;
	int newsockfd;
	int fd;
	int port;
	int total;
	int p;
	int size;
	int temp_size;
	
	struct sockaddr_in serverAddress;

	struct userdetails *obj;
	obj = (struct userdetails *)point;

	char message_from_server[MAXSZ];
	char message_to_server[MAXSZ];
	char data[MAXSZ];
	char file[MAXSZ];// File name
	char file_name[MAXSZ];// File name with instruction to server
	char file_home_dir[MAXSZ];// Location of file i.e. User's home directory(Complete path to file).

	/* Initialise all the character arrays */
	bzero(message_from_server,MAXSZ);
	bzero(message_to_server,MAXSZ);
	bzero(file_name,MAXSZ);
	bzero(file,MAXSZ);
	bzero(file_home_dir,MAXSZ);
	bzero(data,MAXSZ);

	sockfd = socket(AF_INET,SOCK_STREAM,0);

	bzero(&serverAddress,sizeof(serverAddress));/* Initialise structure */

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(obj->arg);
	serverAddress.sin_port = htons(PORT);

	/* Connect to server */
	connect(sockfd,(struct sockaddr *)&serverAddress,sizeof(serverAddress));
/*	if(connect_value == -1)// Connection Error 
	{
       		perror("Error");
        	exit(1);
	}

*/
	/* Receive message from server "Server will send 220" */
	while((no_of_bytes = recv(sockfd,message_from_server,MAXSZ,0)) > 0 )
	{
		message_from_server[no_of_bytes] = '\0';
	
		if(strstr(message_from_server,"220 ") > 0 || strstr(message_from_server,"421 ") > 0)	
			break;
	}

	send(sockfd,user,strlen(user),0);/* Send username to server */
	

	/*
		Receive message from server after sending user name.
		Message with code 331 asks you to enter password corresponding to user.
		Message with code 230 means no password is required for the entered username(LOGIN successful).
	*/
	while((no_of_bytes = recv(sockfd,message_from_server,MAXSZ,0)) > 0)
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
		if(strstr(message_from_server,"230 ") > 0 || strstr(message_from_server,"500 ") > 0 || strstr(message_from_server,"501 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"332 ") > 0 || strstr(message_from_server,"530 ")|| strstr(message_from_server,"331 ") > 0)
			break;
	}

	if(temp == 1)//Password is required for server
	{
		send(sockfd,pass,strlen(pass),0);/* Send password to server */

		/* Receive message from server */
		while((no_of_bytes = recv(sockfd,message_from_server,MAXSZ,0)) > 0)
		{
			message_from_server[no_of_bytes] = '\0';

			if(strstr(message_from_server,"230 ") > 0 || strstr(message_from_server,"500 ") > 0 || strstr(message_from_server,"501 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"332 ") > 0 || strstr(message_from_server,"530 ")|| strstr(message_from_server,"503 ") > 0 || strstr(message_from_server,"202 ") > 0)
				break;

		}
	}

	
	/* Tell server to change to BINARY mode */
	send(sockfd,"TYPE I\r\n",8,0);

	while((no_of_bytes = recv(sockfd,message_from_server,MAXSZ,0)) > 0)
	{
		message_from_server[no_of_bytes] = '\0';
	
		if(strstr(message_from_server,"200 ") > 0 || strstr(message_from_server,"501 ") > 0 ||strstr(message_from_server,"500 ") > 0 ||strstr(message_from_server,"504 ") > 0 ||strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"530 ") > 0)
			break;
	}

	
	/* Send request for PASSIVE connection */	
	send(sockfd,passive,strlen(passive),0);

	while((no_of_bytes = recv(sockfd,message_from_server,MAXSZ,0)) > 0)
	{
		message_from_server[no_of_bytes] = '\0';
		if(strstr(message_from_server,"227 ") > 0 || strstr(message_from_server,"501 ") > 0 ||strstr(message_from_server,"500 ") > 0 ||strstr(message_from_server,"502 ") > 0 ||strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"530 ") > 0)
			break;
	}
		
	/* Server accepts request and sends PORT variables */
	if(strncmp(message_from_server,"227",3) == 0)
	{
		/* Generate a PORT number using PORT variables */
		port = passive_port_number(message_from_server); 
	
		/* Connect to server using another PORT for file transfers */
		newsockfd = func_to_connect_passive(obj->arg,port);

		sprintf(message_to_server,"REST %d\r\n",(obj->t_id)*(obj->size));//Starting byte of chunk of data that is required
	
		send(sockfd,message_to_server,strlen(message_to_server),0);
		
		while((no_of_bytes = recv(sockfd,message_from_server,MAXSZ,0)) > 0)
		{
			message_from_server[no_of_bytes] = '\0';
			if(strstr(message_from_server,"350 ") > 0 || strstr(message_from_server,"500 ") > 0 || strstr(message_from_server,"501 ") > 0 || strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"502 ") > 0 || strstr(message_from_server,"530 "))
				break;
		}
		
		sprintf(file,"%s",(obj->user_input) + 4);


		/* Send file name */
		sprintf(file_name,"RETR %s/%s\r\n",obj->pwd,(obj->user_input) + 4);	
		send(sockfd,file_name,strlen(file_name),0);
				
		while((no_of_bytes = recv(sockfd,message_from_server,MAXSZ,0)) > 0)
		{
			message_from_server[no_of_bytes] = '\0';
			if(strstr(message_from_server,"425 ") > 0|| strstr(message_from_server,"125 ") > 0 ||strstr(message_from_server,"150 ") > 0 || strstr(message_from_server,"501 ") > 0 ||strstr(message_from_server,"500 ") > 0 ||strstr(message_from_server,"550 ") > 0 ||strstr(message_from_server,"421 ") > 0 || strstr(message_from_server,"530 ") > 0)
				break;
		}
		
		sprintf(file_home_dir,"%s%d",file,obj->t_id);
		
		/* Create file on client system */	
		fd = open(file_home_dir,O_CREAT|O_WRONLY|O_TRUNC,0644);			
		size = 0;
		while((no_of_bytes = recv(newsockfd,data,MAXSZ,0))>0)//Get specific chunk of data
		{
			if(size >= obj->size)
				break;		
	
			size += no_of_bytes;
			if(size > obj->size)
			{
				temp_size = no_of_bytes - (size - obj->size);	
				
				size -= no_of_bytes;
				size += temp_size;
				no_of_bytes = temp_size;
			}	

			total = 0;
			/* For partial write operations */
			while(total < no_of_bytes)
                	{
                	        p = write(fd,data + total,no_of_bytes - total);
                       		 total += p;
			}
			
		}
		
				
		/* Close PASSIVE socket and file */
		close(newsockfd);	
		close(fd);
		while((no_of_bytes = recv(sockfd,message_from_server,MAXSZ,0)) > 0)
		{
			message_from_server[no_of_bytes] = '\0';
			if(strstr(message_from_server,"226 ") > 0 || strstr(message_from_server,"426 ") > 0)
				break;
		}
	
	}
	close(sockfd);//Close connection
	
	
	pthread_exit((void *)point);
}	
