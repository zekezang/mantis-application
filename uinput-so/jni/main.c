

/*
 * main.c
 *
 *  Created on: 2013-5-6
 *      Author: zekezang
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "input.h"
#include "uinput.h"

/* Setup the uinput device */
int setup_uinput_device(int fd) {
	struct uinput_user_dev uidev; // uInput device structure
	int i = 0;

	memset(&uidev, 0, sizeof(struct uinput_user_dev)); // Intialize the uInput device to NULL
	snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-keyboard");
	uidev.id.bustype = BUS_USB;
	uidev.id.vendor = 0x1;
	uidev.id.product = 0x1;
	uidev.id.version = 1;
	// Setup the driver I/O channels
	ioctl(fd, UI_SET_EVBIT, EV_KEY);
	for (i = 0; i < 256; i++) {
		ioctl(fd, UI_SET_KEYBIT, i);
	}
	ioctl(fd, UI_SET_KEYBIT, BTN_MOUSE);
	ioctl(fd, UI_SET_KEYBIT, BTN_TOUCH);
	ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);
//	ioctl(fd, UI_SET_KEYBIT, BTN_MIDDLE);
//	ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT);
//	ioctl(fd, UI_SET_KEYBIT, BTN_FORWARD);
//	ioctl(fd, UI_SET_KEYBIT, BTN_BACK);

	ioctl(fd, UI_SET_EVBIT, EV_REL);
	ioctl(fd, UI_SET_RELBIT, REL_X);
	ioctl(fd, UI_SET_RELBIT, REL_Y);


//	ioctl(fd, UI_SET_EVBIT, EV_ABS);
//	ioctl(fd, UI_SET_ABSBIT, ABS_X);
//	ioctl(fd, UI_SET_ABSBIT, ABS_Y);

	/* Create input device into input sub-system */
	write(fd, &uidev, sizeof(struct uinput_user_dev));
	if (ioctl(fd, UI_DEV_CREATE) < 0) {
		printf("error: create uinput device\n");
		return -1;
	}

	return 0;
}

static void send_event(int fd, uint16_t type, uint16_t code, int32_t value) {
	struct input_event event;
	int err;

	memset(&event, 0, sizeof(event));

	event.type = type;
	event.code = code;
	event.value = value;
	gettimeofday(&event.time, NULL);
	if (write(fd, &event, sizeof(event)) < 0) {
		printf("event sent error: %s\n", strerror(errno));
	}
}

void send_key_event(int fd, uint16_t key, int press) {
	printf("send key fd = %d, 0x%04x press: %s\n", fd, key, press ? "true" : "false");
	send_event(fd, EV_KEY, key, press ? 1 : 0);
	send_event(fd, EV_SYN, SYN_REPORT, 0);
}

void send_click_event(int fd, int32_t X, int32_t Y) {
	printf("send click fd = %d, X=%d Y=%d\n", fd, X, Y);

	send_event(fd, EV_REL, REL_X, X);
	send_event(fd, EV_REL, REL_Y, Y);
	send_event(fd, EV_SYN, SYN_REPORT, 0);

	send_event(fd, EV_KEY, BTN_LEFT, 1);
	send_event(fd, EV_SYN, SYN_REPORT, 0);

	send_event(fd, EV_KEY, BTN_LEFT, 0);
	send_event(fd, EV_SYN, SYN_REPORT, 0);

}

int main(int argc, char *argv[]) {
	int fd = -1;
	int key;
	int i;

	fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK); // Open the input device
	if (fd < 0) {
		printf("error: open /dev/uinput\n");
		return -1;
	}

	setup_uinput_device(fd);


	int x, y;

	while(1){
		scanf("%d %d",&x,&y);
		send_click_event(fd, x, y);
	}

//	int x = atoi(argv[1]);
//	int y = atoi(argv[2]);
//
//	send_click_event(fd, x, y);

//	int k = 0;
//	while(k<1000){
//		send_click_event(fd, x, y);
//		k++;
//		msleep(100);
//	}


	if (ioctl(fd, UI_DEV_DESTROY) < 0) {
		printf("error: ioctl");
		return -1;
	}

	close(fd);
	fd = -1;

	return 0;
}

