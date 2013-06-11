#include <sys/types.h>  
#include <sys/socket.h>  
#include <stdio.h>  
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>  
#include <stdlib.h>  
#include <string.h>
#include <sys/stat.h>

int main() {
	int server_sockfd;
	int server_len, client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	int i, btye;
	char *char_recv;


	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	//server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_address.sin_port = 8080;
	server_len = sizeof(server_address);

	server_sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	bind(server_sockfd, (struct sockaddr *) &server_address, server_len);

	char_recv = (char*) malloc(64);

	i = sizeof(client_address);

	while (1) {
		recvfrom(server_sockfd, char_recv, 64, 0, (struct sockaddr *) &client_address, &i);
		printf("---server:%s----\n", char_recv);
	}

	shutdown(server_sockfd, 2);
	return 0;
}
