/*
 * textx.c
 *
 *  Created on: 2013-4-30
 *      Author: zekezang
 */

#include <stdio.h>
#include <string.h>

extern char **environ;
int main(int argc, char *argv[]) {

	setenv("a","bc",1);

	printf("--a:%s\n",getenv("a"));

	strcpy(argv[0],"zekezang-dddd");


//	sleep(100);


	printf("evriron=%s\n", environ[0]);



	float k = 0;

	k = 1.0/100;


	printf("%f\n",k);




	return 0;
}
