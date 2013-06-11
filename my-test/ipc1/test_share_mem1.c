#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void my_exit(char* s) {
	perror(s);
	exit(1);
}

int mem_id;

//计算标示位
int ftok_men() {
	char path[256];
	sprintf(path, "%s/TOG", (char*) getenv("HOME"));
//	mem_id = ftok(path, IPCKEY);
	return mem_id;
}

//开辟内存块
void create_mem() {
	ftok_men();
	int si = shmget(mem_id, sizeof(char) * 32, IPC_CREAT);
	if (si < 0) {
		my_exit("zekezang:");
	}
}

int main(int i, char* args[]) {

	char path[256];

	sprintf(path, "%s/etc/config.ini", (char*) getenv("HOME"));

//	mem_id = ftok(path, IPCKEY);




	return 0;
}
