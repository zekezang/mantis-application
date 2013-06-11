/*
 * android-log.h
 *
 *  Created on: 2012-6-12
 *      Author: zekezang
 */


#include <android/log.h>
#ifndef LOG_H__ZEKEZANG
#define LOG_H__ZEKEZANG
#define LOG_TAG "zekezang"
#define zekezang_log(a)  __android_log_write(ANDROID_LOG_WARN,LOG_TAG,a)
#define zekezang_log(fmt,a)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,fmt,a)
#define zekezang_log(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#endif
