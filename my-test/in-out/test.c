#include <stdio.h>
#include <stdlib.h>

#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#include <string.h>

struct user {
	char name[8];
	int age;
	unsigned x :2;
};

int main(int argc, char **argv) {

	struct user u;
	strcpy(u.name, "zkezang");
	u.age = 3;
	u.x = 1;

	umask(0);
	int x = creat("test", 0777);

	FILE* f = fopen("test", "a+");

	printf("ppp\n");

	if (f != NULL) {

		int i = fwrite(&u, sizeof(struct user), 1, f);
		printf("%d--\n", i);

		u.x = 6;
		i = fwrite(&u, sizeof(struct user), 1, f);
		printf("%d--\n", i);

		u.x = 0;
		i = fwrite(&u, sizeof(struct user), 1, f);
		printf("%d--\n", i);
		fflush(f);

		fclose(f);
	} else {
		perror(NULL);
		exit(-1);
	}

	f = fopen("test", "a+");
	struct user* uu = (struct user*) malloc(sizeof(struct user) * 2);
	if (f != NULL) {

		fread(uu, sizeof(struct user), 1, f);
		printf("--%s--%d--%d\n", uu[0].name, uu[0].age, uu[0].x);
		fseek(f,sizeof(struct user),SEEK_CUR);
		fread(uu,sizeof(struct user),1,f);
		printf("--%s--%d--%d\n", uu[0].name, uu[0].age, uu[0].x);

		fclose(f);
	} else {
		perror(NULL);
		exit(-1);
	}

	return 0;
}
