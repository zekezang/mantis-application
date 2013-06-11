#include <sys/types.h>  
#include <sys/socket.h>  
#include <stdio.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <unistd.h>  
#include <stdlib.h>  
int main() {
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	int i, byte;
	char char_recv, char_send;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = 8080;
	len = sizeof(address);
	if ((result = connect(sockfd, (struct sockaddr *) &address, len)) == -1) {
		perror("connect");
		exit(EXIT_FAILURE);
	}

while(1){
	printf("please enter the context you want to send to server :");
	scanf("%c", &char_send);

	if ((byte = send(sockfd, &char_send, 1, 0)) == -1) {
		perror("send");
		exit(EXIT_FAILURE);
	}
	if ((byte = recv(sockfd, &char_recv, 1, 0)) == -1) {
		perror("recv");
		exit(EXIT_FAILURE);
	}
	printf("receive from server %c\n", char_recv);
}
	close(sockfd);
	exit(0);
}
