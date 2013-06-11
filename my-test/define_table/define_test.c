/*
 * define_test.c
 *
 *  Created on: 2012-7-27
 *      Author: zekezang
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct component {
  char* desc;
  unsigned int value;
  unsigned int support;
};

struct component audio_support[] = {
#define define_audio_support(desc, value, support) { #desc, value, support },
#include "config"
#undef define_audio_support
};

struct component vedio_support[] = {
#define define_vedio_support(desc, value, support) { #desc, value, support },
#include "config"
#undef define_vedio_support
};


void print_test(){
	printf("---media_support[0]:%s----%d\n",audio_support[0].desc,audio_support[0].value);
}

static inline struct component get_component(struct component* comp, int type){
	struct component c;
	memset(&c,0,sizeof(struct component));
	int i = 0;
	while(comp[i].desc != NULL){
		if(comp[i].value == type){
			c = comp[i];
			break;
		}
		i++;
	}
			return c;
}

int main(int argc, char **argv) {
	struct component com;
	int k = 0;
	for(k=0;k<24;k++){
		com = get_component(vedio_support,k);
		printf("-%d--desc:%s--value:%d--support:%d\n",k,com.desc,com.value,com.support);
	}

	return 0;
}

