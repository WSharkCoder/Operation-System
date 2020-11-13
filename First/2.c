#include<stdio.h>
int main(){
    int p1;
    putchar('x');
    while((p1=fork())==-1);
    if(p1==0){
        putchar('b');
    }else{
        putchar('a');

    }
    putchar('y');
    return 0;
}
