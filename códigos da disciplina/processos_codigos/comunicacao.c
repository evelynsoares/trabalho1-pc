#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

pid_t procID;
/* Funcao para tratamento do sinal */

void paiProduzDado(){
  printf("Pai vai produzir um dado.\n");
  sleep(1);
  kill(procID, SIGUSR1);
}

void handlerPai( int sinal ) {
    //printf("Apanhei o sinal %d que o filho mandou !!!\n",sinal);
    paiProduzDado(); 
}


void handlerFilho( int sinal ) {
   printf("Filho usou dado.\n");
   pid_t ppid = getppid();
   kill(ppid, SIGUSR1);

}
/* Funcao principal */
int main() {
procID = fork();
signal(SIGUSR1, handlerFilho); //para evitar de o processo filho receber um signal antes de setar o handler
if (procID < 0 ) {
  printf("Erro na criacao do processo\n");
  return -1;
}else if ( procID == 0 ) {
   sleep(5); 
   
   while(1){
         sleep(1);
   }
}else {
   signal(SIGUSR1, handlerPai);
   paiProduzDado();
   while(1){
         sleep(1);
   }
}
return 0;
}
