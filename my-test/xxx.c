#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//inline int hex_to_dec(char* a) {
//	int j, i, n = 0;
//	for (i = strlen(a) - 1, j = 0; i >= 0; i--, j++) {
//		if (a[i] >= '0' && a[i] <= '9')
//			n += pow((long double) 16, j) * (a[i] - '0');
//		else if (a[i] >= 'a' && a[i] <= 'f')
//			n += pow((long double) 16, j) * (a[i] - 'a' + 10);
//		else if (a[i] >= 'A' && a[i] <= 'F')
//			n += pow((long double) 16, j) * (a[i] - 'A' + 10);
//	}
//	return n; //return n
//}


static const char xx[12];
int main(int argc, char **argv) {

//	struct timeval start;
//	struct timeval end;
//
//	gettimeofday(&start, NULL);
//
//	sleep(2);
//
//	gettimeofday(&end, NULL);
//
//	time_t timeuse = 1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec;
//	timeuse /= 1000000;
//
//	printf("%lu\n",timeuse);
//
//	int charge = 0;
//	char b[10];
//	memset(b, 0, 10);
//	b[0] = '0';
//	b[1] = '2';
//	b[2] = ' ';
//	b[3] = 'B';
//	b[4] = '0';
//	b[5] = ' ';
//	char tmp[3];
//	memset(tmp,0,3);
//	memcpy(tmp, b, 2);
//	charge = hex_to_dec(tmp) * 256;
//	printf("%d\n", charge);
//	memcpy(tmp, b + 3, 2);
//	charge = charge + hex_to_dec(tmp);
//	printf("%d\n", charge);
//	charge &= 32767;
//	printf("%d\n", charge);



//	char* s = "47adad5570899";
//
//	char sa[65];
//	memset(sa,'-',sizeof(sa));
//	sa[65] = 0;
//	int i  = 0;
//	for(i=0;i<10;i++){
//		memcpy(sa+i,(void*)(s+i),1);
//	}
//
//	printf("##%s##\n",sa);


//	int a = -5869;
//	int b = !!a;
//	int k = 2;
//	int c = !k;
//	int d = ~k;
//
//	printf("b:%d\nc:%d\nd:%d\n",b,c,d);




	memset(xx,0,sizeof(xx));
	printf("--strlen(xx):%d--\n",strlen(xx));
	strcpy(xx,"dsd");


	printf("---sizeof(xx):%d----strlen(xx):%d--\n",sizeof(xx),strlen(xx));

	int k = strlen(xx);
	if( strlen(xx) == 0){
		printf("---dsd--\n");
	}

	printf("---%d--\n",(strlen(xx)) == 0);



	return 0;
}
