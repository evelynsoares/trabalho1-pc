#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
 
int main(void) {
   pid_t pid;
 
   pid = fork();
 
   if(pid == 0){
	printf("Processo %d criado por %d\n",getpid(),getppid());
        pid = fork();
        if(pid == 0){
            printf("Processo %d criado por %d\n",getpid(),getppid());
        }else{
            printf("Processo %d criou %d\n",getpid(),pid);
        }
   }else{
        printf("Processo %d criou %d\n",getpid(),pid);
	pid = fork();
        if(pid == 0){
            printf("Processo %d criado por %d\n",getpid(),getppid());
        }else{
            printf("Processo %d criou %d\n",getpid(),pid);
        }
      

   }

   getchar();
   return 0;
}
