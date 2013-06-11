#include <sys/types.h>  
#include <sys/socket.h>  
#include <stdio.h>  
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <unistd.h>  
#include <string.h>
#include <stdlib.h>  
int main() {
	int sockfd;
	int len;
	int optval = 100;
	char *char_send;

	struct sockaddr_nl local;

	memset(&local, 0, sizeof(local));
	local.nl_family = AF_NETLINK;
	local.nl_groups = RTMGRP_LINK;

	if ((sockfd = socket(PF_NETLINK, SOCK_RAW, NETLINK_ROUTE)) == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval));

	char_send = (char*) malloc(64);
	strcpy(char_send, "hello");

	while (1) {
		sendto(sockfd, char_send, strlen(char_send), 0, (struct sockaddr *) &local, sizeof(local));
		sleep(1);
	}

	close(sockfd);
	exit(0);
}
