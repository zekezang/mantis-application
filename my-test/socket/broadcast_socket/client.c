#include <sys/types.h>  
#include <sys/socket.h>  
#include <stdio.h>  
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>  
#include <string.h>
#include <stdlib.h>  
int main() {
	int sockfd;
	int len;
	struct sockaddr_in address;
	int optval = 100;
	char *char_send;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval));

	address.sin_family = AF_INET;
	//address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = 8080;

	char_send = (char*)malloc(64);
	strcpy(char_send,"hello");

	while(1){
		sendto(sockfd, char_send, strlen(char_send), 0, (struct sockaddr *) &address, sizeof(address));
		sleep(1);
	}


	close(sockfd);
	exit(0);
}
