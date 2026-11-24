#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h> 

void funcao(){
      // execlp("prog", "prog", 0);
	printf("Chamou\n");
}


void main(){
           pid_t pid;
           int i;
           pid = fork();
           if (pid == 0)
                   for(i=0; i<5; i++){
                          kill(getppid(), SIGUSR1);
                           sleep(2);
                   }
            else
                if (pid >0){
                       signal(SIGUSR1,funcao);
                       while(1){
                             pause();
                       }
}


