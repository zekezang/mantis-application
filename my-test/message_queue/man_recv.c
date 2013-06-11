#include <pthread.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#define path "/mq_xxx"

#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
struct mq_attr attr;

#define len 1024 //一定要一样大

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

static void /* Thread start function */
tfunc(union sigval sv) {
	struct mq_attr attr;
	ssize_t nr;
	void *buf;
	mqd_t mqdes = *((mqd_t *) sv.sival_ptr);

	/* Determine max. msg size; allocate buffer to receive msg */

	if (mq_getattr(mqdes, &attr) == -1)
		handle_error("mq_getattr");
	buf = malloc(attr.mq_msgsize);
	if (buf == NULL)
		handle_error("malloc");

	nr = mq_receive(mqdes, buf, attr.mq_msgsize, NULL);
	if (nr == -1)
		handle_error("mq_receive");

	printf("Read %ld bytes from MQ\n", (long) nr);
	free(buf);
	exit(EXIT_SUCCESS); /* Terminate the process */
}
int main(int argc, char *argv[]) {
	mqd_t mqdes;
	struct sigevent not;

	int flags;
	flags = O_RDWR;
//	flags |= O_EXCL;
	attr.mq_maxmsg = 5;
	attr.mq_msgsize = 1024;

	mqdes = mq_open(path, flags, FILE_MODE, &attr);
	if (mqdes == (mqd_t) -1)
		handle_error("mq_open");

	not.sigev_notify = SIGEV_THREAD;
	not.sigev_notify_function = tfunc;
	not.sigev_notify_attributes = NULL;
	not.sigev_value.sival_ptr = &mqdes; /* Arg. to thread func. */
	if (mq_notify(mqdes, &not) == -1)
		handle_error("mq_notify");


	pause(); /* Process will be terminated by thread function */
}
