#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <string.h>

#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
struct mq_attr attr;

#define len 1024 //一定要一样大
#define path "/mq_xxx"

int main(int argc, char **argv) {
	int c, flags;
	mqd_t mqd;
	unsigned int priop = 10;
	flags = O_RDWR | O_CREAT;
	flags |= O_EXCL;
	attr.mq_maxmsg = 5;
	attr.mq_msgsize = 1024;

//	int o = mq_unlink(path);
//	if (o < 0) {
//		perror("mq_unlink");
//	}

	mqd = mq_open(path, flags, FILE_MODE, &attr);
//	mq_setattr(mqd, &attr, NULL);//只能设置flags

	if (mqd < 0) {
		perror("mq_open");
	}

	char *s = (char *) malloc(len);
	strcpy(s, "sdfgghghg");

	int i = 0;
	int k = 10;

	while (k > 0) {
		i = mq_send(mqd, s, len, priop);

		if (i < 0) {
			perror("mq_send");
		}

		printf("---send %d--\n", i);

		k--;
		sleep(1);
	}

	mq_close(mqd);
	exit(0);
}
