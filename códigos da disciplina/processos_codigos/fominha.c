#include <stdio.h>
#include <unistd.h>

int main() {
  pid_t pid;

  pid = fork();

  if (pid > 0) {
    printf("Processo fominha entrando automaticamente em segundo plano ... meu PID: %d\n", pid);
    return 0;
  }

  int x=0;
  while (1) {
    x ++;
  }

  return 0;
}
