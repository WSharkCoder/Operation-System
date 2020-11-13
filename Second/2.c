#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/wait.h>
int main(){
        int chld,shmid;
        char *viraddr;
        char buffer[BUFSIZ];
        
        shmid=shmget(IPC_PRIVATE,BUFSIZ,0666|IPC_CREAT);
        viraddr=(char*)shmat(shmid,0,0);
        
        while((chld=fork())==-1);
        if(chld==0){
                while(1){
                        puts("Enter some text:");
                        fgets(buffer,BUFSIZ,stdin);
                        strcat(viraddr,buffer);
                        if(strncmp(buffer,"end",3)==0)
                                break;
                }
                exit(0);
        }else{
                wait(0);
                printf("Your message is: \n%s", viraddr);

                shmdt(viraddr);
                shmctl(shmid,IPC_RMID,0);
                exit(0);
        }
        return 0;
}
