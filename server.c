// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 6969

int static new_socket;
int main(int argc, char const *argv[]) 
{ 
	FILE * fptr;
	pid_t pid;
	int server_fd, valread; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	char buffer[1024] = {0}; 
	char *hello = "Hello from server"; 
	
	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	// Forcefully attaching socket to the port 6969
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
												&opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, 
								sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	}
	
	while (1) {
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen))<0) 
		{ 
			perror("accept"); 
			exit(EXIT_FAILURE); 
		}
		pid = fork();
		if (pid==0) {
			valread = read( new_socket , buffer, 1024); 
			printf("%s\n",buffer );
			char * response = "HTTP/1.1 200 OK\nContent-type: text/html\n\n<html><body><h1>Hello, World!</h1></body></html>";
			send(new_socket , response , strlen(response) , 0 ); 
			printf("Hello message sent\n"); 
		} else {
			
		}
		
		 	
	}
	//here's to opening a gif
	/* if ((fptr = fopen("/home/pi/Desktop/Rotating_earth_(large).gif","rb")) == NULL) {
		printf("Error! opening file");
		exit(1);
	} */

	//finished opening gif
	
	return 0; 
} 
