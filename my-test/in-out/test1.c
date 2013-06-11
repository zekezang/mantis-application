#include <stdio.h>
#include <stdlib.h>

#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#include <string.h>


int main(int argc, char **argv) {

	printf("ppp::::%s\n","sss");
	if(access("/.airplane_state",F_OK) == -1){
		creat("/.airplane_state", 0777);
	}

//	char b[1] = "0";
//
//	mode_t t = umask(0);
//	int x = creat("test", 0777);
//
//	FILE* f = fopen("test", "r+");
//
//
//
//	if (f != NULL) {
//
//		int i = fwrite(b, 1, 1, f);
//
//		fflush(f);
//		fclose(f);
//	} else {
//		perror(NULL);
//		exit(-1);
//	}
//	umask(t);
//
//
//	f = fopen("test", "r+");
//
//	if (f != NULL) {
//
//		int i = fread(b, 1, 1, f);
//		if(atoi(b) == 0){
//			printf("ppp::::%s\n",b);
//		}
//		fclose(f);
//	} else {
//		perror(NULL);
//		exit(-1);
//		}

	return 0;
}
