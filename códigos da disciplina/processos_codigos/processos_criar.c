#include <unistd.h>
#include <stdio.h>


int main() {

int i = 0;
pid_t procID;
for(i=0;i<2;i++){
procID = fork();
}

printf("Processo executando!\n");
return 0;
}
