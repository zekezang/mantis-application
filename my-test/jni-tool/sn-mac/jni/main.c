/*
 * main.c
 *
 *  Created on: 2013-4-22
 *      Author: zekezang
 */

#include <stdio.h>

extern void get_mac_addr(char *mac_addr);
extern void read_sn(char *sn_data);

char mac_addr[32];
char sn_data[32];

int main(int argc, char **argv) {

	read_sn(sn_data);
	get_mac_addr(mac_addr);

	printf("sn: %s		mac: %s\n",sn_data,mac_addr);

	return 0;
}
