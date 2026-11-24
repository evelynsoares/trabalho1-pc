#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "unistd.h"

#define N 10
int contador_compartilhado = 0;

void * pthread_func(void * arg){
    int id = *((int *) arg); 
    sleep(1);
    for (int i = 0; i < 100; i++) {
        contador_compartilhado++;
    }
    printf("Criou um pthread com id = %d, contador = %d, id: %lu \n", id, contador_compartilhado, pthread_self());
    pthread_exit(0);    
}

int main() {
    pthread_t a[N];
    printf("Meu id: %lu\n",pthread_self());

    int i;
    int * id;
    for (i = 0; i < N ; i++) {
        id = (int *) malloc(sizeof(int));
        *id = i;
        pthread_create(&a[i], NULL, pthread_func, (void *) (id));
    }
    
    printf("Valor do contador: %d antes do join\n", contador_compartilhado);

    for (i = 0; i < N ; i++) {
        pthread_join(a[i],NULL);
    }

    printf("Valor do contador: %d depois do join\n", contador_compartilhado);
    return 0;
}

















