CC=gcc
FLG=-c -Wall

all: myftp

myftp: ftp_client.o
	@$(CC) ftp_client.o -o myftp -lpthread

ftp_client.o: ftp_client.c
	@$(CC) $(FLG) ftp_client.c -lpthread

clean:
	@rm *.o
