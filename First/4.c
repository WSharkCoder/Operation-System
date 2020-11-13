#include<stdio.h>
int main(){
	int p1;
    int x=1;
    while((p1=fork())==-1);
    
    if(p1==0){
            putchar('b');
            x=9;
            printf("%d",x);
    }else{
            putchar('a');
            printf("%d",x);
    }
    return 0;
}

