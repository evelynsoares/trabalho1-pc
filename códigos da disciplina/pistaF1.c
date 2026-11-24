#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>


#define NUMCARROS 2
#define NUMEQUIPES 10
#define CAPACIDADEPISTA 5


pthread_t car[NUMCARROS * NUMEQUIPES];
int equipes[NUMEQUIPES];


void *piloto(void *arg);


int main(int argc, char **argv) {
	int i;
	int *id;
	srand48(time(NULL));
	for (i = 0; i < NUMCARROS * NUMEQUIPES; i++) {
		id = (int *)malloc(sizeof(int));
		*id = i;
		pthread_create(&(car[i]), NULL, piloto, (void *)(id));
	}
	pthread_join(car[0], NULL);
}


void *piloto(void *arg) {
	int id = *((int *)arg);
	int eq = id % NUMEQUIPES;
	printf("Carro %d da equipe %d criado\n", id, eq);
	while (1) {
		// ENTRAR NA PISTA
		// TREINAR
		// SAIR DA PISTA
	}
}
