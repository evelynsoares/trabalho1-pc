#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

void run() {
  while (1) {
    sleep(1);
  }
}

int main() {
  pid_t pid, opid;
  int n=0;
  
  while (1) {
    opid = pid;
    pid = fork();

    if (pid < 0) {
      printf("Ops ... nao consegue criar mais processos !\n");
      break;
    }
    if (pid > 0) {
      printf("... mais um processo criado (ja tem %d). PID=%d\n", n, pid);
      n++;
    } else run();
    
  }

  //printf("Matando um dos processos, para se possa ao menos dizimar todos os demais !\n");
  //kill(pid, SIGKILL);

  return 0;
}
