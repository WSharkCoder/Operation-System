#include<stdio.h>
#include<sys/types.h>
#include<sys/sem.h>
union semun{
    int val;
};
int main(){
    int semid;//信号量标识符
    struct sembuf P,V;//P V操作数据结构
    union semun arg;//信号量赋初值的参数数据结构
    //申请只含有一个信号量的信号量集
    semid=semget(IPC_CREAT,1,IPC_CREAT|0666);
    //semid信号量赋初值 
    arg.val=1;
    semctl(semid,0,SETVAL,arg);
    //P操作
    P.sem_num=0;
    P.sem_op=-1;
    P.sem_flg=SEM_UNDO;
    semop(semid,&P,1);
    //V操作
    V.sem_num=0;
    V.sem_op=1;
    V.sem_flg=SEM_UNDO;
    semop(semid,&V,1);
    
    //撤销信号量
    semctl(semid,IPC_RMID,0);
    return 0;
}
