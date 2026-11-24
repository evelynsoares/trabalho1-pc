#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

pthread_mutex_t catraca = PTHREAD_MUTEX_INITIALIZER;
int pessoas_dentro = 0; // varaivel compartilhada entre as threads

// duas threads, uma pra entrada e uma pra saida
void *entrada(void *arg);
void *saida(void *arg);

int main() {
	pthread_t t_entrada, t_saida; // um "handle" para cada thread

	srand48(time(NULL)); // semente do drand48 (tempos aleatórios das catracas)

	pthread_create(&t_entrada, NULL, entrada, NULL);
	pthread_create(&t_saida, NULL, saida, NULL);

	pthread_join(t_entrada, NULL); // a main espera as threads terminarem
	pthread_join(t_saida, NULL);
	return 0;
}


void *entrada(void *arg) {
	while (1) {
		// pessoa chegando e passando pela catraca: FORA do lock,
		// senão a saída ficaria bloqueada à toa durante o sleep
		sleep(drand48() * 3);

		pthread_mutex_lock(&catraca);
		// ++ é ler, somar e gravar: sem lock, uma atualização pode se perder
		pessoas_dentro++;
		// o printf LÊ o contador, então também fica dentro do lock
		printf("Entrou 1 pessoa -> %d no parque\n", pessoas_dentro);
		pthread_mutex_unlock(&catraca);
	}
}

void *saida(void *arg) {
	while (1) {
		sleep(drand48() * 3);

		pthread_mutex_lock(&catraca);
		// o teste e o decremento ficam no MESMO lock: se o if ficasse
		// fora, o valor poderia mudar entre o teste e o --
		if (pessoas_dentro > 0) {
			pessoas_dentro--;
			printf("Saiu 1 pessoa   -> %d no parque\n", pessoas_dentro);
		} else {
			// só com lock não dá pra "esperar alguém entrar": ignora o giro
			printf("Catraca de saida girou com o parque vazio: ignorado\n");
		}
		pthread_mutex_unlock(&catraca);
	}
}