/*
 * run_1.c
 *
 *  Created on: 2012-4-12
 *      Author: zekezang
 */

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

static int pid;
void handler(){
	printf("zekezang--come\n");
	kill(pid,9);
}

int main(int argc, char **argv) {

	signal(SIGALRM,handler);
	alarm(5);

	pid = getpid();

	while(1){
		sleep(1);
		printf("pid:%d, --------\n",pid);
	}
	return 0;
}

