#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
/* Funcao para tratamento do sinal */
void handler( int sinal ) {
printf("Apanhei o sinal %d que o filho mandou !!!\n",sinal);
}

/* Funcao principal */
int main() {
	pid_t procID, ppid;
	signal(SIGUSR1, handler);
	procID = fork();
	if (procID < 0 ) {
	  printf("Erro na criacao do processo\n");
	  return -1;
	}else if ( procID == 0 ) {
	   printf("Vou enviar o sinal SIGUSR1 ao pai\n");
	   ppid = getppid();
	   kill(ppid, SIGUSR1);
	}else {
	   waitpid(procID, NULL, 0);
	}
	return 0;
}
