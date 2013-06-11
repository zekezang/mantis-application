#include<stdio.h>
#include<unistd.h>
int main(int argc, char **argv) {
	int ch;
	opterr = 0;
	while ((ch = getopt(argc, argv, "a:b:c:")) != -1) {
		printf("optind:%d\n", optind);
		switch (ch) {
		case 'a':
			printf("option a:'%s'\n", optarg);
			break;
		case 'b':
			printf("option b:'%s'\n", optarg);
			break;
		default:
			printf("other option :%c\n", ch);
		}
	}
	printf("optopt +%c\n", optopt);
}
