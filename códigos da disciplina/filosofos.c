#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

// nessa solucao ainda nao esta livre de starvation

#define N 5
#define ESQ(id) (id + N - 1) % N // talher da esquerda
#define DIR(id) (id + 1) % N 	 // talher da direta

sem_t s[N]; 					// semaforo que protege talheres

void *filosofos(void *arg);
void pega_talher(int n);
void devolve_talher(int n);

int main() {
	int i;
	int *id;
	// semaforo
	for (i = 0; i < N; i++) {
		sem_init(&s[i], 0, 1); // inicia 5 semaforos =  5 talheres
	}
	pthread_t r[N];

	// criacao das threads de filosofos
	for (i = 0; i < N; i++) {
		id = (int *)malloc(sizeof(int));
		*id = i;
		pthread_create(&r[i], NULL, filosofos, (void *)(id));
	}

	pthread_join(r[0], NULL);
	return 0;
}

void *filosofos(void *arg) {
	int n = *((int *)arg);
	while (1) {
		// pensar
		printf("Filosofo %d pensando ...\n", n);
		sleep(3);
		pega_talher(n); // chama funcao que lida com o semaforo
		// comer
		printf("\tFilosofo %d comendo ...\n", n);
		sleep(3);
		printf("\tFilosofo %d acabou de comer ...\n", n);
		devolve_talher(n); // chama funcao que lida com o semaforo
	}
}

void pega_talher(int n) {
	if (n == 4) { // solucao assimetrica, sem isso da deadlock
		sem_wait(&s[DIR(n)]); // os 5 filosofos esperam o talher da direita ficar livre
		sleep(1);
		sem_wait(&s[ESQ(n)]);

	} else {
		sem_wait(&s[ESQ(n)]); // os 5 tentam pegar o da esquerda
		sleep(1);
		sem_wait(&s[DIR(n)]);
	}
}

void devolve_talher(int n) { // sinaliza que os dois talheres estao disponiveis
	sem_post(&s[ESQ(n)]);
	sem_post(&s[DIR(n)]);
}
