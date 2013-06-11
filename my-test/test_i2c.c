/*
 * Copyright (C) 2012 Shuttle Inc.
 * Author: Wilson H. Tien wilson_tien@tw.shuttle.com
 *
 */
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

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG  , "JNI",__VA_ARGS__)

static int
open_mcu_port(void)
{
  static const char *ECUPDATE = "/sys/mcu/update";
  static const char *ECCOMMAND = "/sys/mcu/command";
  int mcu_port = -1;
  mcu_port = open(ECCOMMAND,O_RDWR );
  if (mcu_port == -1)
    mcu_port = open("/proc/mcu/command",O_RDWR );
  return mcu_port;
}
// ec command
#define ECCMD_VERSION		0
#define ECCMD_MODEL_NAME	1
#define ECCMD_SET_SEGMENT	2
#define ECCMD_RW_SEGMENT	3
#define ECCMD_CHECKSUM		4
#define ECCMD_ERASE		5

#define ECSPEC_VERSION			1
#define ECSPEC_MODEL_NAME		2
#define ECSPEC_ERASE			3
#define ECSPEC_ASSIGN_SEGMENT		4
#define ECSPEC_RW_SEGMENT		5
#define ECSPEC_CHECKSUM			6

#define ECCMD_BAT_TEMPVOLTAGE 0
#define ECCMD_BAT_VOLTAGE 1
#define ECCMD_BAT_CHARGE_STATE 2
#define ECCMD_BAT_REMAIN_CAP 3
#define ECCMD_BAT_FULLCAP 4
#define ECCMD_BAT_STATEOFCHARGE 5
#define ECCMD_BAT_CURRENT 6

#define ECSPEC_BAT_TEMPVOLTAGE 0x10
#define ECSPEC_BAT_VOLTAGE 0x11
#define ECSPEC_BAT_CHARGE_STATE 0x12
#define ECSPEC_BAT_REMAIN_CAP 0x13
#define ECSPEC_BAT_FULLCAP 0x14
#define ECSPEC_BAT_STATEOFCHARGE 0x15
#define ECSPEC_BAT_CURRENT 0x16
struct ECCMD_TABLE {
  char count;
  char cmd;
} table [] =
  {
    { 3,  ECSPEC_VERSION },	// version
    { 5,  ECSPEC_MODEL_NAME },	// model name
    { 3, ECSPEC_ASSIGN_SEGMENT },	// segment N
    { 64, ECSPEC_RW_SEGMENT },	// segment N
    { 1, ECSPEC_CHECKSUM },	// checksum
    { 1, ECSPEC_ERASE },	// erase
  }, bat_table[] =
    {
      {2, ECSPEC_BAT_TEMPVOLTAGE}, // for battery voltage
      {2, ECSPEC_BAT_VOLTAGE}, // for battery voltage
      {2, ECSPEC_BAT_CHARGE_STATE}, // for battery charging state
      {2, ECSPEC_BAT_REMAIN_CAP}, // for battery remaining capacity
      {2, ECSPEC_BAT_FULLCAP}, // for battery full capacity
      {1, ECSPEC_BAT_STATEOFCHARGE}, // for battery state of charge
      {2, ECSPEC_BAT_CURRENT}, // for battery state of charge
    };


const int DEBUG = 0; // 1 printf, 2 logcat

static inline void SetMCUSegment(int fp, int seg, int offset, int len)
{
  static unsigned char buf[8];
  // driver
  buf[0] = table[ECCMD_SET_SEGMENT].count;// count
  buf[1] = table[ECCMD_SET_SEGMENT].cmd;	// command
  // ec
  buf[2] = seg;
  buf[3] = (char)offset;		// position
  buf[4] = (char)len;		// position
  lseek(fp, 0, 0);
  write(fp, buf, table[ECCMD_SET_SEGMENT].count+2);// ec side need +2 , total 5 .
}

/* the stuff read back by MCU is in the form of ascii. Will need to do
   conversion.
   the return value should be the REAL number of read in bytes
 */
static inline int readMCUSegment(int fp, int len, unsigned char *data)
{
  static unsigned char buf[4];
  static unsigned char outbuf[100];
  // driver
  buf[0] = len;			// count
  buf[1] = table[ECCMD_RW_SEGMENT].cmd;	// command
  lseek(fp, 0, 0);
  write(fp, buf, 2);// for driver

  lseek(fp, 0, 0);
  read(fp, outbuf, len*3); //use space for integer trans to ASCII code
  outbuf[len*3] = 0;
  //printf("Buffer: %s\n", outbuf);
  // pack the readin buf first
  unsigned char* s, *t;
  s = t = outbuf;
  while (t < &outbuf[3*len]) {
    if (*t == ' ')
      t++;
    else
      *s++ = *t++;
  }
  *s = '\0';
  //printf("Buffer: %s\n", outbuf);
  // now, buf contains a packed ascii string
  int len1 = ascii2hex(outbuf, data);
  if (len1 < 0) {
    printf("Incorrect ascii\n");
    return -1;
  }
  return len1;
}

void Dump(char *data, int len)
{
  char ts[8];
  char str[512] = {0};
  int i;

  for(i = 0; i < len; i++) {
    sprintf(ts, "[0x%02x]", data[i]);
    strcat(str, ts);
  }
  if(DEBUG & 1)
    printf("data:%s", str);
  if(DEBUG & 2)
    LOGD("data:%s", str);
}


static inline unsigned int ReadMCUChecksum (int fp)
{
  static unsigned char buf[4];
  // driver
  buf[0] = table[ECCMD_CHECKSUM].count;	// count
  buf[1] = table[ECCMD_CHECKSUM].cmd;	// command
  lseek(fp, 0, 0);
  write(fp, buf, 2);

  lseek(fp, 0, 0);
  read(fp, buf, 1*3); //space
  // should be one byte value
  unsigned char seed;
  unsigned char c;
  c = buf[0];
  //printf ("processing char 0x%02x(%c)\n", c, c);
  seed = 0;
  if (c >= '0' && c <= '9')
    seed += c - '0';
  else if (c >= 'a' && c <= 'f')
    seed += c - 'a' + 10;
  else if (c >= 'A' && c <= 'F')
    seed += c - 'A' + 10;
  else  {
    printf("Failing char 0x%02x\n", c);
    return -1;
  }
  c = buf[1];
  //printf ("processing char 0x%02x(%c)\n", c, c);
  seed <<= 4;
  if (c >= '0' && c <= '9')
    seed += c - '0';
  else if (c >= 'a' && c <= 'f')
    seed += c - 'a' + 10;
  else if (c >= 'A' && c <= 'F')
    seed += c - 'A' + 10;
  else {
    printf("Failing char 0x%02x\n", c);
    return -1;
  }
  return seed;
}

static inline void EraseMCUSegment(int fp, int seg)
{
  static unsigned char buf[4];
  // ERASE SEGMENT
  // driver
  buf[0] = table[ECCMD_ERASE].count;	// count
  buf[1] = table[ECCMD_ERASE].cmd;	// command
  // ec
  buf[2] = seg;
  lseek(fp, 0, 0);
  write(fp, buf, 3);
}

static inline void WriteMCU(int fp, unsigned char *data, int len)
{
  static unsigned char buf[100];
  buf[0] = len;				// count
  buf[1] = table[ECCMD_RW_SEGMENT].cmd;	// command
  memcpy(buf+2, data, len);
  lseek(fp, 0, 0);
  write(fp, buf, 2+len);
}

// Read MCU Version
static int ECReadMCUVersion(unsigned char *data)
{
  int fp;
  static unsigned char buf[10];
  int len;
  int idx = 0;
  int err = 0;
  fp = open_mcu_port();
  if(fp == -1) return -1;
  // driver
  buf[0] = table[ECCMD_VERSION].count;	// count
  buf[1] = table[ECCMD_VERSION].cmd;	// command
  lseek(fp, 0, 0);
  write(fp, buf, 2);// for driver
  lseek(fp, 0, 0);
  read(fp, buf, 3*3); //space
  // should be three bytes value
  buf[3*3]='\0';
  printf("MCU Version: Phase #: %s\n", buf);
  unsigned char* s, *t;
  s = t = buf;
  while (t < &buf[3*3]) {
    if (*t == ' ')
      t++;
    else
      *s++ = *t++;
  }
  *s = '\0';
  //printf("Buffer: %s\n", outbuf);
  // now, buf contains a packed ascii string
  int len1 = ascii2decs(buf, data);
  if (len1 < 0) {
    printf("Incorrect ascii\n");
    return -1;
  }

  return 0; // 0 = pass; !=0 is fail
}
// Read MCU Battery Current
static int ECReadMCUBatteryCurrent(unsigned char *data)
{
  int fp;
  static unsigned char buf[10];
  int len;
  int idx = 0;
  int err = 0;
  fp = open_mcu_port();
  if(fp == -1) return -1;
  // driver
  int count = bat_table[ECCMD_BAT_CURRENT].count;	// count
  buf[0] = count;
  buf[1] = table[ECCMD_BAT_CURRENT].cmd;	// command
  lseek(fp, 0, 0);
  write(fp, buf, count);// for driver
  lseek(fp, 0, 0);
  read(fp, buf, count*3); //space
  // should be three bytes value
  buf[count*3]='\0';
  printf("MCU BAT CURRENT: %s\n", buf);
  unsigned char* s, *t;
  s = t = buf;
  while (t < &buf[count*3]) {
    if (*t == ' ')
      t++;
    else
      *s++ = *t++;
  }
  *s = '\0';
  //printf("Buffer: %s\n", outbuf);
  // now, buf contains a packed ascii string
  int len1 = ascii2hex(buf, data);
  if (len1 < 0) {
    printf("Incorrect ascii\n");
    return -1;
  }

  return 0; // 0 = pass; !=0 is fail
}

// Read Command
static int ECReadSegment(int fp, char seg,unsigned char *data)
{
  int len;
  int idx = 0;
  int Count = table[ECCMD_RW_SEGMENT].count;
  // these two are for checksum
  unsigned int total;
  unsigned char checksum;
  int err = 0;

  do {
    /* We can write up to 32 bytes data */
    len = (Count > 32) ? 32 : Count;
    Count -= len;
    SetMCUSegment(fp, seg, idx, len);
    int len1 = readMCUSegment(fp, len, data);
    if (len1 < 0) {
      // failed
      return -1;
    }
    // compute the checksum
    int i;
    total = 0;
    for (i=0; i<len1; i++) total += data[i];
    total &= 0xff;

    // now move the PTR to correct location
    data += len1;
    idx += len1;

    // do checksum to make sure that we do read the correct result
    checksum = ReadMCUChecksum(fp);
    //printf("checksum=%x, total=%x\n", checksum, total);
    if(total != checksum)
      err++;
  } while(Count > 0);
  return err; // 0 = pass; !=0 is fail
}


// Read Command
void ECWriteSegment(int fp, char seg,unsigned char *data, int count)
{
  int len;
  int idx	= 0;
  int Count = count;
  char buf[512];

  //printf("b/f EraseMCUSegment\n");
  EraseMCUSegment (fp, seg);
  //printf("a/f EraseMCUSegment\n");
  do {
    len = (Count > 32) ? 32 : Count;
    Count -= len;
    //printf("b/f SetMCUSegment\n");
    SetMCUSegment(fp, seg, idx, len);
    //printf("a/f SetMCUSegment\n");
    WriteMCU(fp, data, len);
    data += len;
    idx += len;
  } while(Count > 0);
}

static int WriteEx(char *str, int len)
{
  char data[100];
  char verdata[100];
  int fp;
  int count = 30;
  fp = open_mcu_port();
  if(fp == -1)
    return 5;

  LOGD("Read old data");
  if(ECReadSegment(fp, 0, data) != 0) {	// 64 byte
    close(fp);
    return 2;	// read fail
  }
  // now we replace the second 32 bytes with the STR
  int i = 0;
  for (i = 0; i < 32; i++)
    data[32+i] = (i < len) ? str[i] : 0xff;
  /*
  memset(data+32, 0xFF, 32);
  memcpy(data+32, str, len);
  */
  //	LOGD("write new data");
  //	Dump(data, 64);
  do {
    //printf("count = %d\n", count);
    //Dump(data, 64);
    ECWriteSegment(fp, 0, data, 64);

  readagain:
    if(ECReadSegment(fp, 0, verdata) != 0) {	// 64 byte
      if(count == 1){
	close(fp);
	return 3;	// read fail
      }
      count--;
      goto readagain;
    }
    //Dump(data, 64);
    //Dump(verdata, 64);

    if(memcmp(data, verdata, 64) != 0) {
      printf("verify FAILED\n");
      if(count == 1){
	close(fp);
	return 4;	// read fail
      }else{
	// the stuff wrote in is different from the data supposed to be
	// writen in. need to re-write again
	continue;
      }
    }
    break;
  }while (--count > 0);
  close(fp);
  return 0;
}

void ReadEx(unsigned char *ptr, int count, int detail)
{
  unsigned char data[100];
  //  char str[512];
  int fp;
  int err;
  int x;


  fp = open_mcu_port();
  if(fp != -1) {
    err = ECReadSegment(fp, 0, data);
    close(fp);
    if(err != 0) {
      if(DEBUG&1) {
	printf("ECReadSegment fail\n");
	Dump(data, 64);
	printf("\n");
      }
      strcpy(ptr, "fail");
      return;
    }
    else {
      data[64]	= 0;
      return;
    }
  }
  else {
    if(DEBUG&1)
      printf("can't open MCU port\n");
  }
  strcpy(ptr, "fail");
}


inline int
ascii2dec(unsigned char *in)
{
  int seed = 0;
  // in should be in ascii form, such as 35 and end with '/0'
  while (*in) {
    char c;
    seed *= 10;
    c = *in;
    if (c >= '0' && c <= '9')
      seed += c - '0';
    else
      return -1;
    ++in;
  }
  return seed;
}
inline int
ascii2hex(unsigned char *in, unsigned char *out)
{
  unsigned int len = 0;
  // in should be in ascii form, such as AA5043... and end with '/0'
  while (*in) {
    unsigned char seed;
    unsigned char c;
    c = *in;
    //printf ("processing char 0x%02x(%c)\n", c, c);
    seed = 0;
    if (c >= '0' && c <= '9')
      seed += c - '0';
    else if (c >= 'a' && c <= 'f')
      seed += c - 'a' + 10;
    else if (c >= 'A' && c <= 'F')
      seed += c - 'A' + 10;
    else  {
      printf("Failing char 0x%02x\n", c);
      return -1;
    }
    c = *++in;
    //printf ("processing char 0x%02x(%c)\n", c, c);
    seed <<= 4;
    if (c >= '0' && c <= '9')
      seed += c - '0';
    else if (c >= 'a' && c <= 'f')
      seed += c - 'a' + 10;
    else if (c >= 'A' && c <= 'F')
      seed += c - 'A' + 10;
    else {
      printf("Failing char 0x%02x\n", c);
      return -1;
    }
    *out++ = seed;
    len++;
    in++;
  }
  return len;
}
inline int
ascii2decs(unsigned char *in, unsigned char *out)
{
  unsigned int len = 0;
  // in should be in ascii form, such as 5043... and end with '/0'
  while (*in) {
    unsigned char seed;
    unsigned char c;
    c = *in;
    //printf ("processing char 0x%02x(%c)\n", c, c);
    seed = 0;
    if (c >= '0' && c <= '9')
      seed += c - '0';
    else  {
      printf("Failing char 0x%02x\n", c);
      return -1;
    }
    c = *++in;
    //printf ("processing char 0x%02x(%c)\n", c, c);
    seed *= 10;
    if (c >= '0' && c <= '9')
      seed += c - '0';
    else {
      printf("Failing char 0x%02x\n", c);
      return -1;
    }
    *out++ = seed;
    len++;
    in++;
  }
  return len;
}
void main(int argc, char *argv[])
{
  unsigned char data[100];
  char buf[100];
  int len;

  if(argc < 2) {
    printf("\nbidtool is a command line interface for accessing mcu.\n\nCommands:\nread\nwrite [Boardid]\n\n");
    return;
  }

  if(strcmp(argv[1],"readMCUVersion") == 0) {
    int count;

    if (ECReadMCUVersion(data) == 0) {
      printf("MCU Version: Phase #: %d %d %d\n", data[0], data[1], data[2]);
    }
  }
  else if(strcmp(argv[1],"BAT-Current") == 0) {
    int count;

    if (ECReadMCUBatteryCurrent(data) == 0) {
      unsigned short charge = data[0] << 8 | data[1];
      unsigned int charging = charge & 0x8000;
      if (charging) charge &= 0x7fff;
      printf("Battery Current(%s): %dmA\n", (charging ? "Charging" : "Not in  Charging"), charge);
    }
  }
}
