#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "errno.h"
#include "sys/wait.h"

int main(){
    int i = 0;
    int valor = 10;

    for (i = 0; i < 3; i++) {
        pid_t pid = fork();
        
        if (pid == 0)  { // filho
            valor++;
        } else { // pai -> Wait for a child matching PID to die.
            valor--;
            // waitpid(pid, NULL, 0); força pai a esperar filho terminar ants de serguir (print)
        }
    }

    printf("valor = %d\n", valor);
    
    return 0;
}

/*
COM waitpid
valor = 13
valor = 11
valor = 11
valor = 9
valor = 11
valor = 9
valor = 9
valor = 7

SEM waitpid
valor = 7
valor = 9
valor = 9
valor = 11
valor = 11
valor = 13
valor = 9
valor = 11
*/