
/**
 *
 * mkdir /mq_xxx
 * cd /mq_xxx
 * sudo mount -t mqueue none /mq_xxx
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <string.h>

#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
#define path "/mq_xxx"
struct mq_attr attr;
#define len 1024

int main(int argc, char **argv) {
	int c, flags;
	mqd_t mqd;
	unsigned int priop = 10;
	flags = O_RDWR;
//	flags |= O_EXCL;
	attr.mq_maxmsg = 5;
	attr.mq_msgsize = 1024;

	mqd = mq_open(path, flags, FILE_MODE, &attr);
	struct mq_attr temp_attr;
	mq_getattr(mqd,&temp_attr);
	printf("------temp_attr.mq_msgsize:%lu--\n",temp_attr.mq_msgsize);

	if (mqd < 0) {
		perror("mq_open");
	}

	char *s = (char *)malloc(len);

	int i = 0;
	int k = 10;

	while (k > 0) {
		memset(s,0,len);
		i = mq_receive(mqd, s, len, &priop);
		if (i < 0) {
			perror("mq_receive");
		}

		printf("---%s---\n", s);

		k--;
		sleep(1);
	}

	mq_close(mqd);
	exit(0);
}
