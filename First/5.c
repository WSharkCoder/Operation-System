#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
int main(){
        int pid; 
        while((pid=fork())==-1);
        if(pid>0){
                wait(0);
                putchar('P');
        }else{
                putchar('C');
                exit(0);
        }
        return 0;
}
