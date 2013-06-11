/*
 * chardev.h
 *
 *  Created on: 2012-6-28
 *      Author: zekezang
 */

#ifndef CHARDEV_H_
#define CHARDEV_H_

#include <linux/ioctl.h>
#define BAO_IOCTL 't'
#define IOCTL_READ  _IOR(BAO_IOCTL, 0, int)
#define IOCTL_WRITE  _IOW(BAO_IOCTL, 1, int)
#define BAO_IOCTL_MAXNR 1

#endif /* CHARDEV_H_ */
