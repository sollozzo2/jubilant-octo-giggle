// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <dirent.h>
#define PORT 6969


void list_dir(char *dir_name, char* file_str);
void cgiHandler(char *filename, int new_socket);
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
            strcpy(filename,current2);
            return 1;
        }
    } else {
        printf("Running while loop\n");
		current2++;
		printf("current2 is %s\n",current2);
		char* firstHalf = strtok(current2,"?");
		char* secondHalf = strtok(NULL,"?");
		printf("firstHalf: %s",firstHalf);
		
		if (secondHalf!=NULL) {
			printf(" secondHalf: %s",secondHalf);
			char queryString[255];
			strcpy(queryString,"QUERY_STRING=\"");
			strcat(queryString, secondHalf);
			strcat(queryString,"\"");
			printf("%s %s","Query String: ",queryString);
			int ret;
			ret = setenv("QUERY_STRING",secondHalf,1);
		}
        
        strcpy(filename,firstHalf);
        while (*firstHalf!='.') {
            *firstHalf++;
        }
        *firstHalf++;
	
        char *ext;
        ext = firstHalf;
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

    char files[10000];
    char *file_string = files;
    list_dir(filename, file_string);

    char * header = "HTTP/1.1 200 OK\r\nContent-type: text/plain\r\n\r\n";
    
    send(new_socket, header, strlen(header), 0);
    send(new_socket, file_string, strlen(file_string), 0);

}


void list_dir(char *dir_name, char* file_str) {


    printf("running list dir\n");
    struct dirent *dir;

    DIR *d = opendir(dir_name);

    if (d == NULL) {
        printf("couldnt open current directory %s\n", dir_name);
        return;
    }

    while ((dir = readdir(d)) != NULL) {
        printf("%s\n", dir->d_name);
        strcat(file_str, dir->d_name);
        strcat(file_str, "\n");

    }
    closedir(d);

    printf("contents of buffer\n");
    printf("%s\n", file_str);

    return;
}


int main(int argc, char const *argv[]) 
{ 
	FILE * fptr;
	pid_t pid;
	int new_socket;
	int server_fd, valread; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	/*
	static char *var = "QUERY_STRING=\"test1=HERE\"";
	int ret;

	ret = putenv(var);
	printf(ret);
	*/
	
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
            char filename[100];
            char *fp = filename;
            int req_type = parse(buffer, fp);

            printf("filename: %s\n", filename);

            switch(req_type) {
                case -1:
                    // bad case no args
                    break;

                case 1:
                    // directory 
                    directory_request(fp, new_socket);
                    break;

                case 2:
                    // gif case 
                    gifhandler(fp, new_socket);
                    break;
                case 3:
                    //jpg 
                    break;

                case 4:
                    //jpeg
                    break;
                case 5:
                    //html
                    htmlhandler(fp, new_socket);
                    break;
                case 6:
                    cgiHandler(fp, new_socket);
					break;
                default:
                    //unrecognized file type 
                    printf("unrecognized file type\n");
                    break;

            }

			printf("%s\n",buffer );
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

void cgiHandler(char *filename, int new_socket) {
	FILE *output;
	FILE *script;
	char buf[256];
	char scriptBuf[256];
	char response[10000];
	char commandstring[500];
	script = fopen(filename,"r");
	fgets(scriptBuf,256,script);
	
	if (strcmp(scriptBuf,"#!/bin/sh")==0) {
		strcat(commandstring,"sh ");
	} else {
		strcat(commandstring,"perl ");
	}
	
	strcat(commandstring,filename);
	if ((output = popen(commandstring,"r"))==NULL) {
		perror("Popen did not work");
	} else {
		char * header = "HTTP/1.1 200 OK\r\n";
		while (fgets(buf,256,output)!=NULL) {
			strcat(response,buf);
			strcat(response,"\n");
		}
		pclose(output);
		send(new_socket,header,strlen(header),0);
		send(new_socket,response,strlen(response),0);
	}
	

}	





