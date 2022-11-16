#if 1

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef READRFID_H
#define READRFID_H

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>   //串口配置所需头文件

#include <sys/select.h>
#include <unistd.h>

int init_tty(int fd);

unsigned char calc_BCC(unsigned char* SerBfr);

int piccRequest(int fd);

int piccAnticol(int fd);

void readCardId();

#endif // READRFID_H

#ifdef __cplusplus
}
#endif

#endif
