#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"

#define N 4
#define X 100000

int contador = 0;
int contador_unsafe = 0;

pthread_mutex_t lock_contador = PTHREAD_MUTEX_INITIALIZER;

void * incrementar(void * arg){
    int id = *((int *) arg); 
    printf("Criou um pthread com id = %d \n",id);
    int i = 0;
    for(i = 0; i < X; i++){
        pthread_mutex_lock(&lock_contador); // pegar o lock
        contador++; // REGIAO CRITICA -> LE O VALOR, INCREMENTA UM E GRAVA EM CONTADOR
        pthread_mutex_unlock(&lock_contador); // liberar o lock
        contador_unsafe++;
    }
    pthread_exit(0);    
}

int main() {
    // pthread_mutex_t lock_contador;
    // pthread_mutex_init(&lock_contador, NULL); // pode ser inicializado assim tb

    pthread_t a[N];
    int i;
    int * id;
    for (i = 0; i < N ; i++) {
        id = (int *) malloc(sizeof(int));
        *id = i;
        pthread_create(&a[i], NULL, incrementar, (void *) (id));
    }

    for (i = 0; i < N ; i++) {
        pthread_join(a[i],NULL);
    }
    printf("Valor final do contador = %d\n",contador); 
    printf("Valor final do contador_unsafe = %d\n",contador_unsafe); 
    printf("FIM!!!\n");
    return 0;
}

/* SEM mutex
Criou um pthread com id = 0 
Criou um pthread com id = 1 
Criou um pthread com id = 2 
Criou um pthread com id = 3 
Valor final do contador = 129833
Valor final do contador_unsafe = 131810
FIM!!!

COM mutex
Criou um pthread com id = 0 
Criou um pthread com id = 1 
Criou um pthread com id = 2 
Criou um pthread com id = 3 
Valor final do contador = 400000
Valor final do contador_unsafe = 398213
FIM!!!
*/