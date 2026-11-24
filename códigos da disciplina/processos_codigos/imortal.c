#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
/* Funcao para tratamento do sinal */
void handler( int sinal ) {
   printf("SOU IMORTAL !!!\n");
}

/* Funcao principal */
int main() {
	//pid_t procID, ppid;
	signal(SIGTERM, handler);
	signal(SIGKILL, handler);
	signal(SIGINT, handler);
	while(1){
   		sleep(1);
	}

return 0;
}
