#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<sys/types.h>
int main(){
        int shmid;
        char *viraddr;
        char buffer[BUFSIZ];//字符缓冲区
        shmid=shmget(1234,BUFSIZ,0666|IPC_CREAR);//创建共享内存
        viraddr=(char *)shmat(shmid,0,0);//附接到进程的虚拟内存空间
        while(1){
                puts("Enter some text:");
                fgets(buffer,BUFSIZ,stdin  
