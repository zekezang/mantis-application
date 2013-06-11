#include <sys/types.h>  
#include <sys/socket.h>  
#include <stdio.h>  
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>  
#include <string.h>
#include <stdlib.h>
#include <sys/un.h>
int main() {
	int sockfd;
	int len;
	struct sockaddr_un address;
	int optval = 100;
	char *char_send;
	if ((sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	address.sun_family = AF_UNIX;
	strcpy (address.sun_path, "server_socket");

	char_send = (char*)malloc(64);
	strcpy(char_send,"hello");

	while(1){
		sendto(sockfd, char_send, strlen(char_send), 0, (struct sockaddr *) &address, sizeof(address));
		sleep(1);
	}


	close(sockfd);
	exit(0);
}
