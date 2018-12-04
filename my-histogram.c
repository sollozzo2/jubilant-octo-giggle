#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
/*
 * main takes in a directory name, passes it to mydir
 * mydir counts the number of each type of file
 * creates an int array and passes it to formatdat
 * formatdat takes the int array and makes it readable for gnuplot
 * writes it to data1.dat
 * then it prints out the right instructions for making a hist in gnuplot
 * using the data in data1.dat
 * 
 * to make it run:
 * apt-get install gnuplot
 * gcc my-histogram.c -o my-histogram
 * ./my-histogram | gnuplot
 * then there will be a gif image to send back to the client
 * 
 * */
void formatdat(int data[7]) {
	FILE * datafile;
	char strbuf[50];
	const char *types[] = {"Regular","Directory","FIFO","Sockets","Symlinks","CHR","BLK"};
	
	if ((datafile = fopen("data1.dat","w")) == NULL) {
        printf("Error! opening file in formatdat");
        exit(1);
    }
    
    int i;
    char numstr[10];
    for (i = 0; i < 7; i++) {
		strcpy(strbuf, types[i]);
		strcat(strbuf, "    ");
		sprintf(numstr, "%d", data[i]);
		strcat(strbuf, numstr);
		strcat(strbuf, "\n");
		fwrite(strbuf, 1, strlen(strbuf), datafile);
		strbuf[0] = '\0';
	}
	fclose(datafile);
}

void mydir(char * pathname) {
	DIR *dp;
	struct dirent *dirp;
	char final[500];
	struct stat curfile;
	struct stat lnkfile;
	char curfilestr[100];
	char * curfilename;

	int tally[7] = {0};
	//memset(tally, 0, 6);
	if ((dp = opendir(pathname)) == NULL) {
		write(2,"cant open\n",11);
	}
	while ((dirp = readdir(dp)) != NULL) {
		curfilename = dirp->d_name;
		strcpy(curfilestr,pathname);
		strcat(curfilestr,"/");
		strcat(curfilestr,curfilename);
		//printf("curfilename: %s\n",curfilename);
		if (stat(curfilestr, &curfile) != 0) {
			printf("statting error\n");
		}
		switch(curfile.st_mode & S_IFMT) {
			case S_IFREG:
				tally[0]++;
				break;
			case S_IFDIR:
				tally[1]++;
				break;
			case S_IFIFO:
				tally[2]++;
				break;
			case S_IFSOCK:
				tally[3]++;
				break;
			case S_IFLNK:
				tally[4]++;
				break;
			case S_IFCHR:
				tally[5]++;
				break;
			case S_IFBLK:
				tally[6]++;
				break;

		}
	}
	formatdat(tally);
}

int main(int argc, char * argv[]) {
	if (argc < 2) printf("you have to enter a directory\n");
	if (argc > 2) printf("too many args but proceeding anyway\n");
	mydir(argv[1]);
	char * toprint = "reset\n\nset xlabel \"File Type\"\nset ylabel \"Number of Files\"\nset grid\nset boxwidth 0.95 relative\nset style fill transparent solid 0.5 noborder\nplot \"data1.dat\" u 2:xticlabels(1) w boxes lc rgb\"green\" notitle\n\nset yrange [GPVAL_Y_MIN-1:GPVAL_Y_MAX+2]\nset terminal gif\nset output \"myhistresult.gif\"\nreplot";
	printf(toprint);
	return 1;
	//send result gif to client after this
}
