#include <sys/types.h>  
#include <sys/socket.h>  
#include <stdio.h>  
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>  
#include <stdlib.h>  
#include <string.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <poll.h>

int main() {
	int server_sockfd;
	int server_len, client_len;
	struct sockaddr_un server_address;
	struct sockaddr_un client_address;
	int i, btye;
	char *char_recv;

	unlink("server_socket"); /*删除原有server_socket对象*/

	server_address.sun_family = AF_UNIX;
	strcpy(server_address.sun_path, "server_socket");
	server_len = sizeof(server_address);

	server_sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);

	bind(server_sockfd, (struct sockaddr *) &server_address, server_len);

	char_recv = (char*) malloc(64);

	i = sizeof(client_address);

	struct pollfd pollfds[2];
	pollfds[0].fd = server_sockfd;
	pollfds[0].events = POLLIN;

	int reindex = 0, k = 0;
	while (1) {
		reindex = poll(pollfds, 2, 100);
		for (k = 0; k < reindex; k++) {
			switch (pollfds[k].revents) {
			case POLLIN:
				recvfrom(pollfds[k].fd, char_recv, 64, 0, (struct sockaddr *) &client_address, &i);
				printf("------fd:%d---event:POLLIN-----server:%s----\n", k, char_recv);
				break;
			default:
				break;
			}
		}
		printf("------reindex:%d----\n", reindex);
	}

	shutdown(server_sockfd, 2);
	return 0;
}
