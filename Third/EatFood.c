#include<stdio.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
#define OMAX 3
#define AMAX 3
#define MAX 11
union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};
int mutexid;
int said;
int soid;
void main(){
    //信号量数据结构 P V
    struct sembuf P,V;
    //信号量初始化数据结构
    union semun arg;

    //共享内存ID
    int fruitid;
    //共享内存虚地址
    char *fruit;
    //字符缓冲区
    char buffer[BUFSIZ];
    int i,j;
    
    //创建共享存储区
    fruitid=shmget(IPC_PRIVATE,BUFSIZ,0666|IPC_CREAT);
    //附接
    fruit=(char*)shmat(fruitid,0,0);

    //创建信号量
    mutexid=semget(IPC_PRIVATE,1,IPC_CREAT|0666);
    said=semget(IPC_PRIVATE,1,IPC_CREAT|0666);
    soid=semget(IPC_PRIVATE,1,IPC_CREAT|0666);
    //初始化信号量
    arg.val=1;
    if(semctl(mutexid,0,SETVAL,arg)==-1)perror("Setctl Setval Error.[mutex]");
    arg.val=0;
    if(semctl(said,0,SETVAL,arg)==-1)perror("Settcl Setval Error.[sa]");
    arg.val=0;
    if(semctl(soid,0,SETVAL,arg)==-1)perror("Settcl Setval Error.[so]");

    //P V操作初始化
    P.sem_num=0;
    P.sem_op=-1;
    P.sem_flg=SEM_UNDO;
    V.sem_num=0;
    V.sem_op=1;
    V.sem_flg=SEM_UNDO;
    if(fork()==0){
        //父亲向盘中放苹果
        i=1;
        while(1){
            //mutex 信号量 P操作
            printf("[Father] has a apple\n");
            semop(mutexid,&P,1);
            printf("[Father] put apple No.%d\n",i);
            char num[MAX];
            sprintf(num,"%d",i);
            strcpy(buffer,"Apple");
            strcat(buffer,num);
            strcpy(fruit,buffer);
            //sa 信号量 V操作
            semop(said,&V,1);
            if(i++>=AMAX)break;
        }
        printf("[Father] has put all apples.\n");
        exit(0);
    }else{
        if(fork()==0){
            //母亲向盘中放桔子
            j=1;
            while (1){
                //mutex 信号量 P操作
                printf("[Mother] has a orange\n");
                semop(mutexid,&P,1);
                printf("[Mother] put Orange No.%d\n",j);
                char num[MAX];
                sprintf(num,"%d",j);
                strcpy(buffer,"Orange");
                strcat(buffer,num);
                strcpy(fruit,buffer);
                //so 信号量 V操作
                semop(soid,&V,1);
                if(j++>=OMAX)break;
            }
            printf("[Mother] has put all Oranges.\n");
            exit(0); 
        }else{
            if(fork()==0){
                //女儿从盘中拿苹果
                i=1;
                while(1){
                    //sa 信号量 P 操作
                    semop(said,&P,1);
                    printf("[Daughter] gets an apple.\n");
                    semop(mutexid,&V,1);
                    printf("[Daughter] eats the %s\n",fruit);
                    if(i++>=AMAX)break;
               }
               printf("[Daughter] has eaten all apples.\n");
               exit(0);
            }else{
                if(fork()==0){
                    //儿子从盘中拿桔子
                    j=1;
                    while(1){
                        //so 信号量P操作
                        semop(soid,&P,1);
                        printf("[Son] gets an orange.\n");
                        semop(mutexid,&V,1);
                        printf("[Son] eats the %s\n",fruit);
                        if(j++>=OMAX)break;
                    }
                    printf("[Son] has eaten all oranges.\n");
                    exit(0);
                }    
            }
        }  
    }        
    
    wait(0);
    wait(0);
    wait(0);
    wait(0);
    //断开附接
    shmdt(fruit);
    shmctl(fruitid,IPC_RMID,0);
    //撤销信号量
    semctl(mutexid,IPC_RMID,0);
    semctl(said,IPC_RMID,0);
    semctl(soid,IPC_RMID,0);
   
    exit(0);
                  
}

