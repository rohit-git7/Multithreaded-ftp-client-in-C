CC=gcc
FLG=-c -Wall

all: ftp_graphic

ftp_graphic: gtk_final.o
	@$(CC) gtk_final.o -o ftp_graphic `pkg-config --cflags --libs gtk+-2.0 gthread-2.0` -lpthread

gtk_final.o: gtk_final.c
	@$(CC) $(FLG) gtk_final.c `pkg-config --cflags --libs gtk+-2.0 gthread-2.0` -lpthread

clean:
	@rm *.o
