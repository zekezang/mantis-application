#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void reversestr(char *source, char target[], unsigned int length) {
	int i;
	for (i = 0; i < length; i++)
		target[i] = source[length - 1 - i];
	target[i] = 0;
}

void tohex(unsigned long num, char *hexStr) {
	unsigned long n = num;
	char hextable[] = "0123456789ABCDEF";
	char temphex[16], hex[16];
	int i = 0;
	while (n) {
		temphex[i++] = hextable[n % 16];
		n /= 16;
	}
	temphex[i] = 0;
	reversestr(temphex, hex, i);
	strcpy(hexStr, hex);
}

void to20(unsigned long num, char *str) {
	unsigned long n = num;
	char hextable[] = "0123456789ABCDEFGHIJK";
	char temphex[20], hex[20];
	int i = 0;
	while (n) {
		temphex[i++] = hextable[n % 20];
		n /= 20;
	}
	temphex[i] = 0;
	reversestr(temphex, hex, i);
	strcpy(str, hex);
}

int main(int argc, char **argv) {

	char s[32];

	tohex(254,s);
	printf("----%s---\n",s);

	to20(46555,s);
	printf("----%s---\n",s);

	return 0;
}

