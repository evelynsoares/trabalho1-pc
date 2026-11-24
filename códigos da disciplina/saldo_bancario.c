#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

/* Enunciado: a administração de um banco deseja controlar o saldo de
uma conta bancária. Existem duas formas de depósito (caixa eletrônico
e caixa de atendimento) e uma forma de saque (caixa de atendimento).
Construa um protótipo do software a ser usado neste controle. */

int saldo = 0; // variável compartilhada entre as threads
pthread_mutex_t conta = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t tem_saldo = PTHREAD_COND_INITIALIZER;

// uma thread para cada forma de operação
void *deposito_eletronico(void *arg);
void *deposito_atendimento(void *arg);
void *saque_atendimento(void *arg);

int main() {
	pthread_t t_dep_eletronico, t_dep_atendimento, t_saque;

	srand48(time(NULL)); // semente do drand48 (valores e tempos aleatórios)

	pthread_create(&t_dep_eletronico, NULL, deposito_eletronico, NULL);
	pthread_create(&t_dep_atendimento, NULL, deposito_atendimento, NULL);
	pthread_create(&t_saque, NULL, saque_atendimento, NULL);

	pthread_join(t_dep_eletronico, NULL);
	pthread_join(t_dep_atendimento, NULL);
	pthread_join(t_saque, NULL);
	return 0;
}

void *deposito_eletronico(void *arg) {
	while (1) {
		// ... cliente chega no caixa eletrônico e deposita um valor
		int valor = 1 + drand48() * 100; // sorteia de 1 a 100
		sleep(1);                        // tempo de espera do cliente
		pthread_mutex_lock(&conta);
		sleep(1); // tempo de deposito
		saldo += valor;
		pthread_cond_signal(&tem_saldo);
		printf("Cliente está depositando %d pelo caixa eletronico, saldo: %d\n", valor, saldo);
		pthread_mutex_unlock(&conta);
	}
}

void *deposito_atendimento(void *arg) {
	while (1) {
		int valor = 1 + drand48() * 100; // sorteia de 1 a 100
		// ... cliente chega no caixa de atendimento e deposita um valor
		sleep(1);                        // cliente chegando
		pthread_mutex_lock(&conta);
		sleep(1);
		saldo += valor;
		pthread_cond_signal(&tem_saldo);
		printf("Cliente está depositando %d pelo caixa de atendimento, saldo: %d\n", valor, saldo);
		pthread_mutex_unlock(&conta);
	}
}

void *saque_atendimento(void *arg) {
	while (1) {
		int valor = 1 + drand48() * 100; // sorteia de 1 a 100
		// ... cliente chega no caixa de atendimento e saca um valor
		sleep(2);
		pthread_mutex_lock(&conta);
		while (valor > saldo) {
			printf("Cliente não tem saldo suficiente na conta\n");
			pthread_cond_wait(&tem_saldo, &conta);
		}
		// saiu do while, tem saldo, saca
		saldo -= valor;
		printf("Cliente está sacando %d pelo caixa de atendimento, saldo: %d\n", valor, saldo);
		pthread_mutex_unlock(&conta);
	}
}
