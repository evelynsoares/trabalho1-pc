#include "pthread.h"
#include "stdio.h"

pthread_t threads[2];

void *thread_func(void *arg) { // arg é um ptr generico
	int *n = (int *)arg;       // converte arg pra ptr de int
	printf("pthread_t = %lu, n = %d\n", (unsigned long)pthread_self(), *n);
	return NULL;
}

int main() {
	int i, a = 10;
	for (size_t i = 0; i < 2; i++) {
		// ----------- identificador da thread, atributos(ger no espaço do user/kernel), rotina, arg pra funcao
		pthread_create(&(threads[i]), NULL, thread_func, &a);
		printf("threads[%zu] = %lu, a = %d\n", i, (unsigned long)threads[i], a);
	}

	for (size_t i = 0; i < 2; i++) {
		pthread_join(threads[i], NULL);
	}
}