#include<stdio.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
#define MAXSHM 5
union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};  
//3个信号量内部标识
int emptyid;
int fullid;
int mutexid;
void main(){
    //信号量数据结构
    struct sembuf P,V;
    //信号量初始化数据结构
    union semun arg;
    
    //共享内存ID 
    int arrayid;
    int getid;
    //共享内存虚地址
    int *array;
    int *get;
    

    //创建共享内存
    arrayid=shmget(IPC_PRIVATE,sizeof(int)*MAXSHM, IPC_CREAT|0666);
    getid=shmget(IPC_PRIVATE,sizeof(int)*MAXSHM,IPC_CREAT|0666);

    array=(int*)shmat(arrayid,0,0);
    get=(int*)shmat(getid,0,0);
    *get=0;

    //创建信号量
    fullid=semget(IPC_PRIVATE,1,IPC_CREAT|0666);
    emptyid=semget(IPC_PRIVATE,1,IPC_CREAT|0666);
    mutexid=semget(IPC_PRIVATE,1,IPC_CREAT|0666);

    //初始化信号量
    arg.val=0;
    if(semctl(fullid,0,SETVAL,arg)==-1)perror("Semctl Setval Error");
    arg.val=5;
    if(semctl(emptyid,0,SETVAL,arg)==-1)perror("Semctl Setval Error");
    arg.val=1;
    if(semctl(mutexid,0,SETVAL,arg)==-1)perror("Semctl Setval Error");

    //初始化P　V操作
    P.sem_num=0;
    P.sem_op=-1;
    P.sem_flg=SEM_UNDO;
    V.sem_num=0;
    V.sem_op=1;
    V.sem_flg=SEM_UNDO;

    if(fork()==0){
        int i=0;
        int set=0;
        while(i<10){
            //empty 信号量P操作
            semop(emptyid,&P,1);
            //mutex 信号量P操作
            semop(mutexid,&P,1);
            array[set%MAXSHM]=i+1;
            printf("Producer put number %d to NO. %d\n",array[set%MAXSHM],set%MAXSHM);
            set++;
            //mutex 信号量V操作
            semop(mutexid,&V,1);
            //full 信号量V操作
            semop(fullid,&V,1);
            i++;
            }
            sleep(3);
            printf("Producer is over\n");
            exit(0);
    }else{
        //A 消费者进程
        if(fork()==0){
            while(1){
                if(*get ==10)break;
                //full 信号量P操作
                semop(fullid,&P,1);
                //mutex 信号量P操作
                semop(mutexid,&P,1);

                printf("The Comsumer A get number from NO. %d\n",(*get)%MAXSHM);
                (*get)++;
                semop(mutexid,&V,1);
                semop(emptyid,&V,1);
                sleep(1);
            }
            printf("Cosumer A is over\n");
            exit(0);
        }else{
        //B 消费者进程
            if(fork()==0){
                while(1){
                    if((*get)==10)break;
                    //full 信号量P操作
                    semop(fullid,&P,1);
                    //mutex 信号量P操作
                    semop(mutexid,&P,1);
                    printf("The Comsumer B get number from NO. %d\n",(*get)%MAXSHM);
                    (*get)++;
                    //mutex 信号量V操作
                    semop(mutexid,&V,1);
                    semop(emptyid,&V,1);
                    sleep(1);
                }
            printf("Comsumer B is over\n");
            exit(0);   
            }
        }
    
    }
    wait(0);
    wait(0);
    wait(0);
    //断开连接并撤销共享存储区
    shmdt(array);
    shmctl(arrayid,IPC_RMID,0);
    shmdt(get);
    shmctl(getid,IPC_RMID,0);
    //撤销3个信号量集
    semctl(emptyid,IPC_RMID,0);
    semctl(fullid,IPC_RMID,0);
    semctl(mutexid,IPC_RMID,0);
    exit(0);
}
