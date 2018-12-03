// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <dirent.h>
#define PORT 6969

int static new_socket;

void htmlhandler(char * filename, int new_socket) {
    FILE * toopen;
    if ((toopen = fopen(filename,"r")) == NULL) {
        printf("Error! opening file in htmlhandle");
        exit(1);
    }
    char * header = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
    char htmlfile[5000];
    fread(htmlfile, 5000, 1, toopen);
    
    send(new_socket, header, strlen(header), 0);
    send(new_socket, htmlfile, strlen(htmlfile), 0);
    return;
}

void gifhandler(char * filename, int new_socket) {
    FILE * toopen;
    if ((toopen = fopen(filename,"rb")) == NULL) {
        printf("Error! opening file in gifhandle");
        //send 404?
        //send(new_socket, ", , 0);
        exit(1);
    }   
    char * header = "HTTP/1.1 200 OK\r\nContent-type: image/gif\r\n\r\n";
    //might be better if imagebuf size is FILE_MAX_SIZE
    char imagebuf[1500000];
    fread(imagebuf, 1500000, 1, toopen);
    send(new_socket, header, strlen(header), 0);
    send(new_socket, imagebuf, 1500000, 0);
    return;
}
int parse(char * buffer, char *filename) {
    strtok(buffer,"\n");
    char * current = strtok(buffer, " ");
    printf("%s %s\n","First token:",current);
    char * current2 = strtok(NULL," ");
    printf("%s %s\n","Second token:",current2);
    char * ptr;
    ptr = strchr(current2, '.');
    int i =0;
    if (ptr==NULL) {
        if (strcmp("/",current2)==0) {
            printf("%s\n","No args");
            return -1;
        } else {
            printf("Running null\n");
            
            current2++;
            printf("directory name is %s\n", current2);
            filename = current2;
            return 1;
        }
    } else {
        printf("Running while loop\n");
        current2++;
        strcpy(filename,current2);
        while (*current2!='.') {
            *current2++;
        }
        *current2++;
        char *ext;
        ext = current2;
        printf("%s %s","Filename:",filename);
        printf("%s %s","Extension:",ext);
        if (strcmp("gif",ext)==0) {
            return 2;
        } else if (strcmp("jpg",ext)==0) {
            return 3;
        } else if (strcmp("jpeg",ext)==0) {
            return 4;
        } else if (strcmp("html",ext)==0) {
            return 5;
        } else if (strcmp("cgi",ext)==0) {
            return 6;
        } else {
            return -1;
        }
    }
    
} 

void directory_request(char* filename, int new_socket) {

}


int list_dir(char *dir_name) {
    printf("running list dir\n");
    struct dirent *dir;

    DIR *d = opendir(dir_name);

    if (d == NULL) {
        printf("couldnt open current directory\n");
        return 0;
    }

    while ((dir = readdir(d)) != NULL) {
            printf("%s\n", dir->d_name);
    }
    closedir(d);

    return 0;
}


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
            char *filename;
            int req_type = parse(buffer, filename);

            switch(req_type) {
                case -1:
                    // bad case no args

                case 1:
                    // directory 
                    directory_request(filename, new_socket);

                case 2:
                    // gif case 
                case 3:
                    //jpg 
                case 4:
                    //jpeg
                case 5:
                    //html
                    htmlhandler(filename, new_socket);
                case 6:
                    //cgi
                default:
                    //unrecognized file type 
                    printf("unrecognized file type\n");
            }

            // case of request for directory listing
            char dirname[7] = "testdir";
            char *p = dirname;
            list_dir(p); 
			printf("%s\n",buffer );
			char * response = "HTTP/1.1 200 OK\nContent-type: text/html\n\n<html><body><h1>Hello, World!</h1></body></html>";
			send(new_socket , response , strlen(response) , 0 ); 
            close(new_socket);
            exit(0);
			printf("Hello message sent\n"); 
		} else {
		    close(new_socket);	
		}
	}

	//finished opening gif
	
	return 0; 
} 



