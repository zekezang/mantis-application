#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int i, char* args[]) {

	printf("%s\n", "will come in sub pid");

	int state;
	
	int pipe_file[2];
	
	if(pipe(pipe_file)<0){
		exit(0);
	}
	
	pid_t pid = fork();
	
	char s[10] = "zekezang";
	char* ss = (char*)malloc(sizeof(char)*10);

	if (pid > 0) {
		close(pipe_file[0]);
		write(pipe_file[1],s,sizeof(s));
		close(pipe_file[1]);
		printf("%s=%d\n", "this is main ", getpid());
	} else if (pid == 0) {
		close(pipe_file[1]);
		read(pipe_file[0],ss,sizeof(s));
		close(pipe_file[0]);
		printf("%s ->%s=%d\n", ss, "this is sub ", getpid());
		exit(9);
	}
	
	wait(&state);
	printf("%d\n", WEXITSTATUS(state));
	
	return 0;
}

