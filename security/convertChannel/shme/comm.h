#ifndef _COMM_H_
#define _COMM_H_

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

#define PATHNAME    "." // ftok函数 生成key使用
#define PROJ_ID     66 // ftok 函数生成key使用
#define PROJ_ID_1   67 // ftok 函数生成key使用
#define MEM_SIZE    4096
#define VAL_NUM     6
#define TIME        500

int create_shm( int size);//  分配指定大小的共享内存块
int destroy_shm( int shmid); // 释放指定id的共享内存块
int get_shmid(); // 获取已经存在的共享内存块
int create_shm_syn (int size);
void synchronize(int sender);

#endif /*_COMM_H_*/
