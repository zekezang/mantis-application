#include <string.h>
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <stdint.h>
#include <errno.h>
#include <android/log.h>
#include <sys/ioctl.h>

#include "rock.h"

#ifndef LOG_H__
#define LOG_H__
#define LOG_TAG "zekezang"
#define LOGW(a)  __android_log_write(ANDROID_LOG_INFO,LOG_TAG,a)
#define LOGFMTW(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#endif

static RKNAND_SYS_STORGAE sysData;

void read_whole(RKNAND_SYS_STORGAE *sysData) {
	int ret;
	int sys_fd = open("/dev/rknand_sys_storage", O_RDWR, 0);
	if (sys_fd < 0) {
		LOGFMTW("rknand_sys_storage open fail\n");
	}
	sysData->tag = VENDOR_SECTOR_OP_TAG;
	sysData->len = RKNAND_SYS_STORGAE_DATA_LEN - 8;

	// read the current value stored in vendor partition first
	ret = ioctl(sys_fd, RKNAND_GET_VENDOR_SECTOR0, sysData);

	//  rknand_print_hex_data("vendor_sector load:",(uint32*)sysData.data,32);
	if (ret) {
		LOGFMTW("get vendor_sector error\n");
	}
	close(sys_fd);
//	int i = 0;
//	for (i = 0; i < 64; i++) {
//		LOGFMTW("----#%d----%X--\n",i,sysData->data[i]);
//	}
}

int write_SN(RKNAND_SYS_STORGAE *sysData) {
	int ret;
	int sys_fd = open("/dev/rknand_sys_storage", O_RDWR, 0);
	if (sys_fd < 0) {
		LOGFMTW("rknand_sys_storage open fail\n");
	}
	sysData->tag = VENDOR_SECTOR_OP_TAG;
	sysData->len = RKNAND_SYS_STORGAE_DATA_LEN - 8;

	ret = ioctl(sys_fd, RKNAND_STORE_VENDOR_SECTOR0, sysData);
	close(sys_fd);
	return ret;
}

void read_sn(char *sn_data){
	memset(&sysData, 0, sizeof(RKNAND_SYS_STORGAE));
	read_whole(&sysData);
	char *sn_offset = (&sysData)->data + 32;
	char *s = (char*) malloc(512);
	memset(s, 0, 512);
	memcpy(s, sn_offset, 32);

	memset(sn_data,0,32);
	sprintf(sn_data,"%s",s);
}



/**
JNIEXPORT jint JNICALL Java_com_rockchip_tool_Rock_1jni_write(JNIEnv *env, jobject obj, jstring jstr) {
	const char *sn = (*env)->GetStringUTFChars(env, jstr, NULL);
	int len = strlen(sn);
	if (len < 1 || len > 32) {
		(*env)->ReleaseStringUTFChars(env, jstr, sn);
		return 0;
	}

	int ret = 0;

	memset(&sysData, 0, sizeof(RKNAND_SYS_STORGAE));

	read_whole(&sysData);

	char *sn_offset;
	sn_offset = (&sysData)->data + 32;
	memcpy(sn_offset, sn, 32);

	ret = write_SN(&sysData);

	(*env)->ReleaseStringUTFChars(env, jstr, sn);

	return ret == 0 ? 1 : 0;
}

JNIEXPORT jstring JNICALL Java_com_rockchip_tool_Rock_1jni_read_1sn(JNIEnv *env, jobject obj) {
	memset(&sysData, 0, sizeof(RKNAND_SYS_STORGAE));
	read_whole(&sysData);
	char *sn_offset = (&sysData)->data + 32;
	char *s = (char*) malloc(512);
	memset(s, 0, 512);
	memcpy(s, sn_offset, 32);
	return (*env)->NewStringUTF(env, s);
}

JNIEXPORT jstring JNICALL Java_com_rockchip_tool_Rock_1jni_read_1id(JNIEnv *env, jobject obj) {
	memset(&sysData, 0, sizeof(RKNAND_SYS_STORGAE));
	read_whole(&sysData);

	char *s = (char*) malloc(512);
	memset(s, 0, 512);

	char buffer[2];

	int i = 0;
	while(i<32){

		LOGFMTW("---data:---%X--\n",(&sysData)->data[i]);
		buffer[0] = '-';
		buffer[1] = '-';
	   sprintf(buffer,"%X",(&sysData)->data[i]);

	   if(buffer[1] == '-'||buffer[1]=='\0'||buffer[1] == NULL){
		   buffer[1] = buffer[0];
		   buffer[0] = '0';
	   }

	   memcpy(s+i*2,buffer,2);
	   i++;
	}

	LOGFMTW("----%s\n",s);

	return (*env)->NewStringUTF(env, s);
}

JNIEXPORT jstring JNICALL Java_com_rockchip_tool_Rock_1jni_testRead(JNIEnv *env, jobject obj) {
//	read_whole_test(&sysData);

	return NULL;
}
**/

