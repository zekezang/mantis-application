/*
 * main.c
 *
 *  Created on: 2013-5-8
 *      Author: zekezang
 */
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include "test.h"


int main(int argc, char **argv) {


void *handler = dlopen("/sourcecode/arm-workspace/my-test/dlsym_dlopen/test_so.so", RTLD_NOW);

	printf("----%s---\n",dlerror());

	struct xxx *x = (struct xxx *)dlsym(handler, "test_xxx");

	printf("----%s---\n",dlerror());

	printf("----%d---\n",x->a);

	x->func('s');


	dlclose(handler);

	return 0;
}

