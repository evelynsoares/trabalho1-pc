#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define PR 5 // número de produtores
#define CN 2 // número de consumidores
#define N 5  // tamanho do mutex_prateleira

void *produtor(void *meuid);
void *consumidor(void *meuid);

int prateleira[N];
// tres variaveis pra mexer no indice de prateleira
int endereco_inserir;
int endereco_remover;
int contador_mutex_prateleira = 0;

pthread_mutex_t mutex_prateleira = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t prateleira_tem_espaco = PTHREAD_COND_INITIALIZER;
pthread_cond_t prateleira_tem_item = PTHREAD_COND_INITIALIZER;

int main() {
	int erro;
	int i, n, m;
	int *id;

	pthread_t tid[PR];

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
	return 0;
}

void *produtor(void *pi) { // produz itens
	while (1) {
		int item = (int)(drand48() * 1000.0);

		printf("Produtor %d está produzindo um item\n", *(int *)(pi));
		sleep(2); // tempo de produção

		// pega o lock
		pthread_mutex_lock(&mutex_prateleira);
		while (contador_mutex_prateleira == N) { // o mutex_prateleira esta cheio
			// produtor espera espaço no mutex_prateleira pra produzir de novo
			printf("Produtor %d: vou esperar espaço na prateleira\n", *(int *)(pi));
			pthread_cond_wait(&prateleira_tem_espaco, &mutex_prateleira);
		} // produtores foram acordados

		printf("produtor %d: vou inserir item %d na posição %d\n", *(int *)(pi), item, endereco_inserir);
		contador_mutex_prateleira++;
		prateleira[endereco_inserir] = item;
		endereco_inserir++;

		// caso a prateleira ja foi toda preenchida
		if (endereco_inserir >= N) {
			endereco_inserir = 0;
		}

		// acorda os consumidores
		pthread_cond_broadcast(&prateleira_tem_item);

		// libera o lock
		pthread_mutex_unlock(&mutex_prateleira);
	}
	pthread_exit(0);
}

void *consumidor(void *pi) { // consome itens
	while (1) {
		// pega o lock
		pthread_mutex_lock(&mutex_prateleira);

		// se nao ha itens na prateleira
		while (contador_mutex_prateleira == 0) {
			printf("Consumidor %d: vou esperar por itens\n", *(int *)(pi));
			pthread_cond_wait(&prateleira_tem_item, &mutex_prateleira);
		} // consumidores foram acordados

		int item_consumido = prateleira[endereco_remover];
		printf("consumidor %d: vou remover item %d na posição %d\n", *(int *)(pi), item_consumido, endereco_remover);
		endereco_remover++;

		// todos os itens da prateleira ja foram consumidos
		if (endereco_remover >= N) {
			endereco_remover = 0;
		}
		contador_mutex_prateleira--;

		// acorda os produtores
		pthread_cond_broadcast(&prateleira_tem_espaco);

		// libera o lock
		pthread_mutex_unlock(&mutex_prateleira);

		// consome o item
		printf("Consumidor %d: vou consumir item %d\n", *(int *)(pi), item_consumido);
		sleep(1);
	}
	pthread_exit(0);
}
