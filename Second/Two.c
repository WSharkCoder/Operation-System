#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/wait.h>
void  int_func(int sign);
int k;
void int_func(int sign){
        k=0;
}
int main(){
        int pid,shmid,running=1;
        char *viraddr1,*viraddr2;
        char buffer[BUFSIZ];
        signal(SIGUSR1,int_func);
        k=1;
        shmid=shmget(IPC_PRIVATE,BUFSIZ,0666|IPC_CREAT);
        while((pid= fork())==-1);
        if(pid>0){
                viraddr1=(char*)shmat(shmid,0,0);
                while(running){
                        puts("Enter some text:");
                        fgets(buffer,BUFSIZ,stdin);
                        strcat(viraddr1,buffer);
                        if(strncmp(buffer,"end",3)==0)
                                running=0;
                }
                shmdt(viraddr1);
                kill(pid,SIGUSR1);
                wait(0);
                shmctl(shmid,IPC_RMID,0);
                exit(0);
        }else{
                while(k==1);
                viraddr2=shmat(shmid,0,0);
                printf("Your message is :\n%s",viraddr2);
                shmdt(viraddr2);
                exit(0);
        }
        return 0;
}
