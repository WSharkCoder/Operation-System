#include<stdio.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<stdlib.h>//exit()
#include<unistd.h>//fork() sleep()
#include<sys/wait.h>

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short  *array;
};
int mutexid;//信号量标识
void main(){
    int chld,i,j;

    //定义PV操作数据类型
    struct sembuf P,V;
    //定义信号量初始化参数
    union semun arg;

    //创建只有一个互斥信号量元素的信号集
    mutexid=semget(IPC_PRIVATE,1,0666|IPC_CREAT);
    
    //信号量赋初值
    arg.val=1;
    if(semctl(mutexid,0,SETVAL,arg)==-1)perror("Semctl SetVal Error!");
   
    /*定义P　V操作*/
    P.sem_num=0;
    P.sem_op=-1;
    P.sem_flg=SEM_UNDO;
    V.sem_num=0;
    V.sem_op=1;
    V.sem_flg=SEM_UNDO;
    
    //创建子进程
    while((chld=fork())==-1);
    if(chld>0){
        i=1;
        while(i<=3){
            sleep(1);
            //P操作
            semop(mutexid,&P,1);
            printf("prnt In\n");
            sleep(1);
            printf("prnt Out\n");
            //V操作
            semop(mutexid,&V,1);
            i++;
        }
        wait(0);//等待子进程终止
        semctl(mutexid,IPC_RMID,0);//撤销信号量
        exit(0);
    }else{
        j=1;
        while(j<=3){
            sleep(1);
            //P操作
            semop(mutexid,&P,1);
            printf("chld In\n");
            sleep(1);
            printf("chld Out\n");
            //V操作
            semop(mutexid,&V,1);
            j++;
        }
        exit(0);
    }
}
        
    
