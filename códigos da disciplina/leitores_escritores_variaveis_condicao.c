#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"

#define TRUE 1
#define NE 10 // numero de escritores
#define NL 20 // numero de leitores

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t turno = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t pode_ler = PTHREAD_COND_INITIALIZER;
pthread_cond_t pode_escrever = PTHREAD_COND_INITIALIZER;

int leitores = 0;
int escritores = 0;

void *reader(void *arg);
void *writer(void *arg);
void read_data_base();
void use_data_read();
void think_up_data();
void write_data_base();

int main() {

	pthread_t r[NL], w[NE];

	int i;
	int *id;
	/* criando leitores */
	for (i = 0; i < NL; i++) {
		id = (int *)malloc(sizeof(int));
		*id = i;
		pthread_create(&r[i], NULL, reader, (void *)(id));
	}

	/* criando escritores */
	for (i = 0; i < NE; i++) {
		id = (int *)malloc(sizeof(int));
		*id = i;
		pthread_create(&w[i], NULL, writer, (void *)(id));
	}

	pthread_join(r[0], NULL);
	return 0;
}

void *reader(void *arg) {
	int i = *((int *)arg);
	while (TRUE) { /* repere para sempre */
		pthread_mutex_lock(&turno);
		pthread_mutex_lock(&lock);
		while (escritores > 0) {
			pthread_cond_wait(&pode_ler, &lock);
		}
		leitores++;
		pthread_mutex_unlock(&lock);
		pthread_mutex_unlock(&turno);

		read_data_base(i);

		pthread_mutex_lock(&lock);
		leitores--;
		if (leitores == 0) {
			pthread_cond_signal(&pode_escrever);
		}
		pthread_mutex_unlock(&lock);

		use_data_read(i); /* região não crítica */
	}
	pthread_exit(0);
}

void *writer(void *arg) {
	int i = *((int *)arg);

	while (TRUE) {        /* repete para sempre */
		think_up_data(i); /* região não crítica */
		pthread_mutex_lock(&turno);
		pthread_mutex_lock(&lock);
		while (escritores > 0 || leitores > 0) {
			pthread_cond_wait(&pode_escrever, &lock);
		}
		escritores++;
		pthread_mutex_unlock(&lock);
		pthread_mutex_unlock(&turno);

		write_data_base(i); /* atualiza os dados */

		pthread_mutex_lock(&lock);
		escritores--;
		pthread_cond_signal(&pode_escrever);
		pthread_cond_broadcast(&pode_ler);
		pthread_mutex_unlock(&lock);
	}
	pthread_exit(0);
}

void read_data_base(int i) {
	printf("Leitor %d está lendo os dados! Número de leitores = %d \n", i, leitores);
	sleep(rand() % 5);
}

void use_data_read(int i) {
	printf("Leitor %d está usando os dados lidos!\n", i);
	sleep(rand() % 5);
}

void think_up_data(int i) {
	printf("Escritor %d está pensando no que pode_escrever!\n", i);
	sleep(rand() % 5 + 5);
}

void write_data_base(int i) {
	printf("Escritor %d está escrevendo os dados!\n", i);
	sleep(rand() % 5);
}
