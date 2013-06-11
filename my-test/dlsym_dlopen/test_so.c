/*
 * test_so.c
 *
 *  Created on: 2013-5-8
 *      Author: zekezang
 */
#include "test.h"
#include <stdlib.h>
#include <stdio.h>

void say(char c){
	printf("---%c---\n",c);
}


struct xxx test_xxx = {
	.a = 90,
	.func = say,
};
