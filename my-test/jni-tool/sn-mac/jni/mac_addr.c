/*
 * mac_addr.c
 *
 *  Created on: 2013-4-22
 *      Author: zekezang
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <net/if.h>
#include <net/if_arp.h>

int sockfd;
struct ifreq ifr;

void get_mac_addr(char *mac_addr) {

	int err;
	memset(mac_addr, 0, 32);

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
	}

	strcpy(ifr.ifr_name, "wlan0");
	err = ioctl(sockfd, SIOCGIFHWADDR, &ifr);

	if (!err) {
		unsigned char* hw = ifr.ifr_hwaddr.sa_data;
		sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X", hw[0], hw[1], hw[2], hw[3], hw[4], hw[5]);
	}

	close(sockfd);
}

