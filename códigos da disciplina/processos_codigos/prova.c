#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
 
int main(void) {
   int i = 0;
   int valor = 10;
   pid_t pid;
   for(i=0; i < 2; ){
      i++;
      pid = fork();
      if(pid == 0){//filho
	valor++;
      }else{//pai
        valor--;
        //waitpid(pid,NULL,0);
      }
   }
    printf("Processo %d, pai: %d; valor: %d\n",getpid(),getppid(),valor);
   
   return 0;
}
