#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <pthread.h>


pthread_t t_id, tt_id;

int t_r, tt_r;

int ticket = 100;



pthread_mutex_t pthread_silgn;

void my_exit(char* s) {
	perror(s);
	exit(1);
}

//锁保护
int getTicket(){
	pthread_mutex_lock(&pthread_silgn);
	ticket--;
	pthread_mutex_unlock(&pthread_silgn);
	return ticket;
}


//线程1执行函数
void say1(void* v) {
	printf("%s--p1--%lu\n", "1111111", pthread_self());
	int t = 0;
	while ((t=getTicket()) > 0) {
		usleep(10000);
		printf("sale ticket A %d\n", t);
	}
	pthread_exit((void *) 1);
}

//创建线程1
void create_pthread_1() {
	int si = pthread_create(&t_id, NULL, (void*)say1, NULL);
	if (si < 0) {
		my_exit("p1 die");

	}
}

//线程2执行函数
void say2(void* v) {
	printf("%s--p2--%lu\n", "2222222", pthread_self());
	int t = 0;
	while ((t=getTicket()) > 0) {
		usleep(50000);
		printf("sale ticket B %d\n", t);
	}
	pthread_exit((void *) 2);
}

//创建线程1
void create_pthread_2() {
	int si = pthread_create(&tt_id, NULL, (void*)say2, NULL);
	if (si < 0) {
		my_exit("p2 die");
	}
}

int main(int i, char* args[]) {

	//初始化锁
	pthread_mutex_init(&pthread_silgn,NULL);


	create_pthread_1();

	create_pthread_2();

	pthread_join(t_id, (void*) &t_r);
	pthread_join(tt_id, (void*) &tt_r);

	pthread_mutex_destroy(&pthread_silgn);

	printf("p1:%d;p2:%d\n", t_r, tt_r);
	return 0;
}
