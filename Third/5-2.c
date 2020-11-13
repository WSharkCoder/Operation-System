#include<stdio.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<string.h>
#include<sys/wait.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

//信号量内部标识
int emptyid;
int fullid;

void main(){
    int chld;

    //信号量数据结构
    struct sembuf P,V;
    //信号量初始化参数
    union semun arg;

    //共享内存
    int shmid;
    char *viraddr;
    char buffer[BUFSIZ];
    
    //创建信号量并初始化
    emptyid=semget(IPC_PRIVATE,1,IPC_CREAT|0666);
    arg.val=1;
    if(semctl(emptyid,0,SETVAL,arg)==-1)perror("Semctl Setval Error");
    fullid=semget(IPC_PRIVATE,1,IPC_CREAT|0666);
    arg.val=0;
    if(semctl(fullid,0,SETVAL,arg)==-1)perror("Semctl Setval Error");
    

    //定义P　V操作
    P.sem_num=0;
    P.sem_op=-1;
    P.sem_flg=SEM_UNDO;
    V.sem_num=0;
    V.sem_op=1;
    V.sem_flg=SEM_UNDO;

    //创建并附接共享存储区
    shmid=shmget(IPC_PRIVATE,BUFSIZ,0666|IPC_CREAT);
    viraddr=(char*)shmat(shmid,0,0);
    
    while((chld=fork())==-1);
    if(chld>0){
        while(1){
            //full 信号量P操作
            semop(fullid,&P,1);
            printf("Your Message is:\n%s",viraddr);
            //empty 信号量V操作
            semop(emptyid,&V,1);
            if(strncmp(viraddr,"end",3)==0)break;
        }
        wait(0);
        shmdt(viraddr);
        shmctl(shmid,IPC_RMID,0);
        semctl(emptyid,IPC_RMID,0);
        semctl(fullid,IPC_RMID,0);
        printf("Parent exit!\n");
        exit(0);
    }else{
        while(1){
            //empty 信号量P操作
            semop(emptyid,&P,1);
            puts("Enter your text");
            fgets(buffer,BUFSIZ,stdin);
            strcpy(viraddr,buffer);
            //full 信号量V操作
            semop(fullid,&V,1);
            if(strncmp(viraddr,"end",3)==0){
                sleep(1);
                break;
            }
        }
        printf("Child exit!\n");
        exit(0);
    }   
}
