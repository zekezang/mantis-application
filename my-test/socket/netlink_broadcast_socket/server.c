#include <sys/types.h>  
#include <sys/socket.h>  
#include <stdio.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <string.h>
#include <sys/stat.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

int main() {
	int server_sockfd;
	int server_len;
	int i, btye;
	char *char_recv;

	struct sockaddr_nl local;


	memset(&local, 0, sizeof(local));
	local.nl_family = AF_NETLINK;
	local.nl_groups = RTMGRP_LINK;
	server_len = sizeof(local);

	server_sockfd = socket(PF_NETLINK, SOCK_RAW, NETLINK_ROUTE);

	bind(server_sockfd, (struct sockaddr *) &local, server_len);

	char_recv = (char*) malloc(64);

	while (1) {
		recvfrom(server_sockfd, char_recv, 64, 0, NULL, NULL);
		printf("---server:%s----\n", char_recv);
	}

	close(server_sockfd);
	return 0;
}
