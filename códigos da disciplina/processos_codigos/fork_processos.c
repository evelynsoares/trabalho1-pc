#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
int i = 10;
    

int main(void) {
   if(fork() == 0){
      i--;
   }else{
      i++;
   }
   fork();
   i++;
   printf("Valor de i para o processo %d, pai: %d; i: %d\n",getpid(),getppid(),i);
   return 0;
}
