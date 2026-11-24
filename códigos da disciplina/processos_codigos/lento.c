#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

int main() {
  pid_t pid;

  pid = fork();

  if (pid > 0) {
    printf("Processo lento entrando automaticamente em segundo plano ... meu PID: %d\n", pid);
    return 0;
  }

  int x=0;
  struct timeval t1, t2;
  gettimeofday(&t1, NULL);
  while (1) {
    sleep(1);
    gettimeofday(&t2, NULL);
    float dt = 1e3*(t2.tv_sec - t1.tv_sec) + 1e-3*(t2.tv_usec - t1.tv_usec);
    printf("Lento com PID %d: tempo de sono = %.2f milissegundos\n", getpid(), dt);
    gettimeofday(&t1, NULL);
  }

  return 0;
}
