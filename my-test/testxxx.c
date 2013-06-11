#include <stdio.h>
#include <stdlib.h>

#include <linux/string.h>
static const char *GB9664_HCIATTACH = "hciattach_gb9664";

struct xxx {
	char *name;
	int a;
};

int main(void){

	char hciattach[64];

	strncpy(hciattach, GB9664_HCIATTACH, strlen(GB9664_HCIATTACH));

	*(hciattach+strlen(GB9664_HCIATTACH)) = '\0';
	printf("------%s------\n",hciattach);


	struct xxx *a;
	a = (struct xxx *)calloc(2, sizeof(struct xxx));

	printf("#############name:%s#######int:%d#####\n",a->name,&a->name);	
	if(!a->name){
		printf("dsdsdsds\n");
	}


	a->name = (char*)"zekezang";
	a->a = 10;

	(a+sizeof(struct xxx))->name = (char*)"uuuuuu";
	(a+sizeof(struct xxx))->a = 90;
	
	printf("###xxx->name:%s####a:%d###\n", a->name, a->a);

	printf("#####xxx[1].name:%s#####a:%d####\n",(a+sizeof(struct xxx))->name, (a+sizeof(struct xxx))->a);


	a = (struct xxx *)realloc(a, 3);
	
	a[2].name = "ewrqwq";

	printf("11111###xxx->name:%s####a:%d###\n", a->name, a->a);

    printf("11111#####xxx[1].name:%s#####a:%d####\n",(a+sizeof(struct xxx))->name, (a+sizeof(struct xxx))->a);


	printf("11111###sdsa[2].name:%s###\n",a[2].name);



	free(a);	
	a = NULL;


return 0;
}
