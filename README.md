#                               Multisegmented FTP client program in C                                               
Apart from all functionalities of standard FTP, the important thing on which I have worked is to increase the downloading speed of very large files using multithreading(used `POSIX` threads).                                 
For very large files, multiple threads are created and each thread creates an `instruction connection` and `data connection` with the server. Each thread downloads a specific chunk of data and then all are combined together to get the file.                                                                                                             
#Note:
Use only for very large files because creating and managing threads is also an overhead for CPU. Performance will degrade for smaller files.                                                                                                  

Following functions are included. For example:                                                                  
`ls` : To list files in current directory on server side.                                                             
`ls -l` : Same as `ls`, with some more details.                                                                       
`pwd` : Current working directory on server side.                                                                     
`get filename` : Download file from server.(File should be present on server side).                                   
`put filename` : Upload file on server.                                                                               
`uniqput filename` : Uniquely upload file on server.                                                                 
`mkdir directory_name` : Create directory on server.                                                                  
`rmdir directory_name` : Remove directory on server.                                                                  
`rm filename` : Remove file on server.                                                                                
`cd directory_name` : Traverse file system on server side.                                                            
`rename old_filename new_filename` : Renaming file on server.                                                        
                                                                                                                      
`!ls` : To list files in current directory on client side.                                                            
`!ls -l` : Same as `!ls`, with some more details.                                                                     
`!pwd` : Current working directory on client side.                                                                    
`!cd directory_name` : Traverse file system on client side.                                                          
`!mkdir directory_name` : Create directory on client side.                                                            
`!rmdir directory_name` : Remove directory on client side.                                                            
                                                                                                                     
To run:                                                                                                           
Run command `make all` to compile program.                                                                              
To execute, run `./myftp server-ip-address`.                                                                            
Enter user credentials to login on server.                                                                             
#Features in GUI(Apart from all the functionalities)                                                                  
* Drag N Drop to upload and download files.                                                                          
* Double click to enter directory.                                                                                   

#Acknowledgements
* Professor `Khushil Saini`, for the project idea and concepts about multithreading.
* Stackoverflow community(http://stackoverflow.com).
* The very helpful documentation of FTP `RFC 959`(https://www.ietf.org/rfc/rfc959.txt).
