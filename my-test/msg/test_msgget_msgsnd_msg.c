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

struct my_msg{
long type;
char mtext[32];
};

int main(int i, char* args[]) {

	int mk = msgget(key, 0666 | IPC_CREAT);
	if(mk<0){
		my_exit("create failed");
	}
	printf("ipc msg id:%d\n",mk);



    struct my_msg mm;
    memset(&mm,0,sizeof(mm));
    mm.type = 1000;
    strcpy(mm.mtext, "zekezang");
    if(msgsnd(mk,&mm,strlen(mm.mtext),0)<0){
        my_exit("send:");
    }


	return 0;
}
