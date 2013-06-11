#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

key_t key = 0x1234;

void my_exit(char* s) {
	perror(s);
	exit(1);
}

int main(int i, char* args[]) {

	int mk = msgget(key, 0666 | IPC_CREAT);
	if(mk<0){
		my_exit("create failed");
	}
	printf("ipc msg id:%d\n",mk);


	return 0;
}
