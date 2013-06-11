/*
 * peekadb.c
 *
 *  Created on: 2012-5-31
 *      Author: zekezang
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static FILE* fp;

int main(int argc, char **argv) {
	char buf_path[200];
	char* path = getcwd(buf_path, sizeof(buf_path));
	path = strcat(path, "/");

	char* adb_command = strcat(path, "adb devices");

//	printf("%s\n", adb_command);

	while (1) {
		fp = popen(adb_command, "r");
		if (fp) {
			char buffer[80];
			fgets(buffer, sizeof(buffer), fp);
			fgets(buffer, sizeof(buffer), fp);

			if(strlen(buffer)>0){
				printf("%s\n", buffer);
			}

			pclose(fp);
		}
		sleep(1);
	}

	exit(EXIT_SUCCESS);
}

