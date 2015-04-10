#include<gtk/gtk.h>
#include<stdio.h>
#include<sys/types.h>//socket
#include<sys/socket.h>//socket
#include<stdlib.h>//sizeof
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#include<ctype.h>//isdigit()
#include<fcntl.h>//open()
#include<dirent.h>
#include<sys/stat.h>//stat()
#include<grp.h>
#include<pwd.h>
#include<time.h>
#include<pthread.h>
#include<errno.h>
#include<glib.h>

#define PORT 21
#define MAXSZ 4096
#define MAXSZ1 5000

#define MIN_VALUE -1
#define INITIALISE 0
#define MIN_IP 0
#define MAX_IP 255

#define BUTTON_WIDTH 70
#define BUTTON_HEIGHT 30

char user[MAXSZ];/* user details sent to server */
char pass[MAXSZ];/* password details sent to server */
char buff[MAXSZ1];  
char passive[]="PASV\r\n";

GtkTextBuffer *buffer;

typedef struct _Appstate
{
	GtkWidget *window;
	GtkWidget *entry;
	GtkWidget *entry_dir;
	GtkWidget *entry_dir_cli;
	GtkWidget *text_view;
	GtkWidget *view;
	GtkWidget *view1;
	GtkWidget *user;
	GtkWidget *password;
	GtkWidget *fileselect;
	GtkWidget *button;
	GtkWidget *ok_file_button;
	GtkWidget *ok_button;
	GtkWidget *command;
	int status;
	int sockfd;
	int running;
	int seek;
	int seek_cli;
	int temp_file_descriptor;
	int temp_file_descriptor_cli;
}Appstate;

struct userdetails
{
	char arg[MAXSZ];
	char user_input[MAXSZ];
	char home_dir[MAXSZ];
	char pwd[MAXSZ];
	int t_id;
	int size;
	Appstate app_state;
};


