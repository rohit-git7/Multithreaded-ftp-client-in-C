/*
List files in current working directory on client side.
*/

/* Return result of character comparison(Ignoring LETTER CASE)*/
int mycompare(const void *a, const void *b)
{
	return strcasecmp(*(const char **)a,*(const char **)b);
}


/* `ls` comand in linux. List files in current working directory without details.*/
void ls_dir(Appstate *app_state,char *dir_name)
{
	DIR *fd;

	char *arg[MAXSZ];

	int i = INITIALISE;
	int n;
	
	struct dirent *entry;
	fd = opendir(dir_name);/* Open directory */
	if(fd == NULL)
	{
		sprintf(buff,"Error: %s\n",strerror(errno));
		print_buff(app_state);	
	
		return;
	}

	while((entry = readdir(fd))!=NULL)/* Ignore hidden files */
	{
	    if(entry->d_name[0] != '.' && strcmp(entry->d_name,".") != 0 && strcmp(entry->d_name,"..") != 0)
		{
			*(arg + i) = entry->d_name;
			i++;
		}
	}
	
	n = i;
	
	qsort(arg,n,sizeof(const char *),mycompare);/* Sort file names in alphabetical order ignoring LETTER CASE*/

	i = 0;
	sprintf(buff,"Here comes the directory listing\n\n");
	print_buff(app_state);	
	while(i < n)
	{
		sprintf(buff,"%s\n",*(arg + i));
		print_buff(app_state);	
	
		i++;
	}
	sprintf(buff,"\n");
	print_buff(app_state);	
	
	closedir(fd);
}

void ls_l_dir(Appstate *app_state,char *dir_name)
{
	char time_buff[MAXSZ];
	char *arg[MAXSZ];
	
	int i = INITIALISE;
	int n;
	int val = INITIALISE;
	int temp;

	struct group *gp;/* structure contating group details */
	struct passwd *pw;/* structure containing passwd file */
	struct tm *info;
	struct stat file_buff;
	struct dirent *entry;
	DIR *fd;
	fd  = opendir(dir_name);/* Open directory */
	if(fd == NULL)
	{
		sprintf(buff,"Error: %s\n",strerror(errno));
		print_buff(app_state);	
	
		return;
	}
	
	while((entry = readdir(fd))!= NULL)/* Read directory */
	{
		
	/* Call kiya tha */
	   if(strcmp(entry->d_name,".") != 0 && strcmp(entry->d_name,"..") != 0 && entry->d_name[0]!= '.')
 		{
			*(arg + i) = entry->d_name;
			i++;
		}

	}

	n = i;
	qsort(arg,n,sizeof(const char *),mycompare);

	i = 0;
	sprintf(buff,"Here comes the directory listing\n\n");
	print_buff(app_state);	
	while(i < n)
	{
		lstat(*(arg + i),&file_buff);
	   	gp = getgrgid(file_buff.st_gid);		
		pw = getpwuid(file_buff.st_uid);
		info = localtime(&(file_buff.st_mtime));
		strftime(time_buff,sizeof(time_buff),"%b %d %H:%M",info);
		
			if(file_buff.st_size % 1024 == 0)
			{
				temp = (int)file_buff.st_size / 1024;
				val += temp ;
			}
			else
			{
				temp = ((int)file_buff.st_size / 1024);
				val += (temp + (4 - (temp % 4)));
			}

			/* Selecting file type */		
			switch(file_buff.st_mode & S_IFMT)
			{
				case S_IFCHR:
					sprintf(buff,"c");
					break;
				case S_IFBLK:
					sprintf(buff,"b");
					break;
				case S_IFDIR:
                        		sprintf(buff,"d");
					break;
				case S_IFLNK:
					sprintf(buff,"l");
					break;
				case S_IFIFO:
					sprintf(buff,"p");
					break;
				case S_IFSOCK:
					sprintf(buff,"s");
					break;
				default:
					sprintf(buff,"-");
					break;
			}

		print_buff(app_state);	

			/* File permissions */
			if(file_buff.st_mode & S_IRUSR)
				sprintf(buff,"r");
			else
				sprintf(buff,"-");

			if(file_buff.st_mode & S_IWUSR)
				strcat(buff,"w");
			else
				strcat(buff,"-");
		
			if(file_buff.st_mode & S_IXUSR)
				strcat(buff,"x");
			else
				strcat(buff,"-");

			if(file_buff.st_mode & S_IRGRP)
				strcat(buff,"r");
			else
				strcat(buff,"-");

			if(file_buff.st_mode & S_IWGRP)
				strcat(buff,"w");
			else
				strcat(buff,"-");

			if(file_buff.st_mode & S_IXGRP)
				strcat(buff,"x");
			else
				strcat(buff,"-");

			if(file_buff.st_mode & S_IROTH)
				strcat(buff,"r");
			else
				strcat(buff,"-");

			if(file_buff.st_mode & S_IWOTH)
				strcat(buff,"w");
			else
				strcat(buff,"-");
			
			if(file_buff.st_mode & S_IXOTH)
				strcat(buff,"x");
			else
				strcat(buff,"-");

		print_buff(app_state);	

			if(((file_buff.st_mode & S_IFMT)^S_IFCHR) == 0 || ((file_buff.st_mode & S_IFMT)^S_IFBLK) ==0)
				sprintf(buff," %9d %20s %20s %8d, %8d %15s %25s\n",(int)file_buff.st_nlink,pw->pw_name,gp->gr_name,major(file_buff.st_rdev),minor(file_buff.st_rdev),time_buff,*(arg + i));
			else
				sprintf(buff," %9d %20s %20s %18u %15s %25s\n",(int)file_buff.st_nlink,pw->pw_name,gp->gr_name,(unsigned int)file_buff.st_size,time_buff,*(arg + i));
		
		print_buff(app_state);	
		i++;
	}

	sprintf(buff,"total %d\n\n",val);
	print_buff(app_state);	
	
	
	closedir(fd);

}

