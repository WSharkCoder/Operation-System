#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
int main(){
        int pid,i;
        while((pid=fork())==-1);
        if(pid>0){
                wait(0);
                for(i=0;i<5;i++){
                        printf("I am Parent!\n");
                        sleep(1);
                }
                printf("\n");
        }else{
                for(i=0;i<5;i++){
                        printf("I am Child!\n");
                        sleep(1);
                }
                printf("\n");
                exit(0);
           
        }
        return 0;
}

