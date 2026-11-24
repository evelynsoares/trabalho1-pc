#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

#define PR 1 // número de produtores
#define CN 1 // número de consumidores
#define N 5  // tamanho do buffer

void *produtor(void *meuid);
void *consumidor(void *meuid);

int buffer[N];   // onde os itens ficam de fato
int inserir = 0; // prox posicao q o prod escreve
int remover = 0; // procx posicao q o cons le

sem_t mutex; // protege o buffer
sem_t vazio; // quantas posicoes livres existem
sem_t cheio; // quantas posicoes ocupadas existem


void main(argc, argv) int argc;
char *argv[];
{

	int erro;
	int i, n, m;
	int *id;

	pthread_t tid[PR];

	sem_init(&mutex, 0, 1); // 1: só uma thread por vez mexe no buffer
	sem_init(&vazio, 0, N); // começa com N posições livres
	sem_init(&cheio, 0, 0); // começa sem nenhum item

	for (i = 0; i < PR; i++) {
		id = (int *)malloc(sizeof(int));
		*id = i;
		erro = pthread_create(&tid[i], NULL, produtor, (void *)(id));

		if (erro) {
			printf("erro na criacao do thread %d\n", i);
			exit(1);
		}
	}

	pthread_t tCid[CN];

	for (i = 0; i < CN; i++) {
		id = (int *)malloc(sizeof(int));
		*id = i;
		erro = pthread_create(&tCid[i], NULL, consumidor, (void *)(id));

		if (erro) {
			printf("erro na criacao do thread %d\n", i);
			exit(1);
		}
	}

	pthread_join(tid[0], NULL);
}

void *produtor(void *pi) {
	int id = *(int *)pi;
	while (1) {
		int item = (int)(drand48() * 1000.0); // "produz" fora da região crítica
		sleep(1);

		sem_wait(&vazio); // down(empty): espera ter posição livre
		sem_wait(&mutex); // down(mutex): entra na região crítica
		buffer[inserir] = item;
		printf("produtor %d: inseri %d na posicao %d\n", id, item, inserir);
		inserir = (inserir + 1) % N;
		sem_post(&mutex); // up(mutex): sai da região crítica
		sem_post(&cheio); // up(full): avisa que tem mais um item
	}
	pthread_exit(0);
}

void *consumidor(void *pi) {
	int id = *(int *)pi;
	while (1) {
		sem_wait(&cheio); // espera ter item
		sem_wait(&mutex);
		int item = buffer[remover];
		printf("consumidor %d: removi %d da posicao %d\n", id, item, remover);
		remover = (remover + 1) % N;
		sem_post(&mutex);
		sem_post(&vazio); // libera uma posição

		sleep(3); // "consome" fora da região crítica
	}
	pthread_exit(0);
}
