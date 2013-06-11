#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

void my_exit(char* s) {
	perror(s);
	exit(1);
}

int get_stdin(char* ts) {
	memset(ts, 0, 32);
	int gs = read(0, ts, 32);
	return gs;
}

int main(int i, char* args[]) {

	char* path_name = "/sourcecode/arm-workspace/my-test/ipc1/fifo.d";

	int mk_fifo = mkfifo(path_name, 0777);

	if (mk_fifo < 0) {
		my_exit("11111");
	}

	pid_t fork_s = fork();

	if (fork_s > 0) {
		int w_file = open(path_name, O_WRONLY, 0777);
		if (w_file < 0) {
			my_exit("333333");
		}

		char* ts = (char*) malloc(sizeof(char) * 32);

		int ls;
		while (1) {
			if((ls=get_stdin(ts))>0){
				printf("%d",ls);
				write(w_file, ts, strlen(ts));
			}
		}

//		char* s = "zekezanghello";
//		int u = write(w_file, s, strlen(s));
		close(w_file);

	} else if (fork_s == 0) {
		char* s = (char*) malloc(sizeof(char) * 32);
		int p = 0;
		int r_file = open(path_name, O_RDONLY | O_NONBLOCK, 0777);
		if (r_file < 0) {
			my_exit("44444");
		}
		while (1) {
			memset(s,0,32);
			p = read(r_file, s, 32);
			if (p > 0) {
				printf("%s-ww-%d\n", s, getpid());
//				free(s);
//				unlink(path_name);
//				break;
			}

			sleep(1);
			//			if(p<0){
			//				unlink(path_name);
			//				break;
			//			}
		}
		close(r_file);
		exit(9);
	} else {
		my_exit("222222");
	}

	wait(NULL);

	return 0;
}
