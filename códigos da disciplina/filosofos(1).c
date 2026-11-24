#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#define N 5
#define ESQ(id) (id+N-1)%N
#define DIR(id)	(id+1)%N
#define PENSANDO 0
#define FAMINTO 1
#define COMENDO 2

int estados[N];

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
sem_t s[N];

void *filosofos (void *arg);
void pega_talher (int n);
void devolve_talher (int n);
void teste (int n);

int main () {
	int i;
	int *id;

	//semaforo 
	for (i=0; i<N; i++) {
		sem_init(&(s[i]), 0, 0);
	}

	pthread_t r[N]; 

	//criacao das threads de filosofos
    for (i = 0; i < N ; i++) {
	     id = (int *) malloc(sizeof(int));
	     *id = i;
	     pthread_create(&r[i], NULL, filosofos, (void*) (id));
	}

        pthread_join(r[0],NULL);
	return 0;
}


void * filosofos (void *arg) {
	int n = *((int *) arg);
	while(1) {
		//pensar
		printf("Filosofo %d pensando ...\n", n);
		sleep(3);
		pega_talher(n);
		//comer
		printf("\tFilosofo %d comendo ...\n", n);
		sleep(3);
                printf("\tFilosofo %d acabou de comer ...\n", n);
		devolve_talher(n);
	} 
}

void pega_talher (int n) {
	pthread_mutex_lock(&lock);
	estados[n] = FAMINTO;
	teste(n);
	pthread_mutex_unlock(&lock);
	sem_wait(&s[n]);
} 

void devolve_talher (int n) {
	pthread_mutex_lock(&lock);
	estados[n] = PENSANDO;
	teste(ESQ(n));
	teste(DIR(n));
	pthread_mutex_unlock(&lock);
}

void teste (int n) {
	if(estados[n] == FAMINTO 
		&& estados[ESQ(n)] != COMENDO
		&& estados[DIR(n)] != COMENDO) 
	{
		estados[n] = COMENDO;
		sem_post(&s[n]);
	}
}








