#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MA 10 //macacos que andam de A para B
#define MB 10 //macacos que andam de B para A

pthread_mutex_t turn = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_count_ab = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_count_ba = PTHREAD_MUTEX_INITIALIZER;
// só um contadr faz com que um macaco A -> B 
// espere mesmo quando um macaco B -> A está apenas atualizando outro contador
pthread_mutex_t corda = PTHREAD_MUTEX_INITIALIZER;

int count_ab = 0, count_ba = 0;

void * macacoAB(void * a) {
	int i = *((int *) a);    
	while(1){
		pthread_mutex_lock(&turn); // tenta pegar a turn
		pthread_mutex_lock(&mutex_count_ab); // dou lock na mutex_count_ab, evita que 2 macacos alterem count_ab ao mesmo tempo
		count_ab++;

		// se for o primeiro macaco, pego a corda
		if (count_ab == 1) {
			pthread_mutex_lock(&corda);
		}

		// libera turn pq turn não representa a corda ocupada, representa a entrada no processo de decisão
		// Se o gorila tentar ele conseguirá turno, mas ficará parado em corda
		pthread_mutex_unlock(&turn); 
		// liberar mutex_count_ab porque outro macaco A -> B precisa conseguir fazer incremetno
		pthread_mutex_unlock(&mutex_count_ab);

		printf("Macaco %d passando de A para B\n", i); // regiao critica conceitual
		// sleep(1);
		
		// ermina de atravessar, ele protege o contador AB antes de diminuí-lo
        pthread_mutex_lock(&mutex_count_ab);
        count_ab--;
        printf("Macaco %d terminou de passar de A para B. Num: %d\n" , i, count_ab);
		if(count_ab == 0){ // ultimo macaco da mutex_count_ab ab passou
			pthread_mutex_unlock(&corda); // libera corda: um grupo B -> A pode assumir; um gorila pode entrar
		}
		pthread_mutex_unlock(&mutex_count_ab); // libera mutex_count_ab que protege o contador
	}
	pthread_exit(0);
}

void * macacoBA(void * a) {
	int i = *((int *) a);    
	while(1){
		pthread_mutex_lock(&turn); // tenta pegar a turn
		pthread_mutex_lock(&mutex_count_ba); // dou lock na mutex_count_ab, evita que 2 macacos alterem count_ab ao mesmo tempo
		count_ba++;

		// se for o primeiro macaco, pega a corda
		if (count_ba == 1) {
			pthread_mutex_lock(&corda);
		}

		// libera turn pq turn não representa a corda ocupada, representa a entrada no processo de decisão
		// Se o gorila tentar ele conseguirá turno, mas ficará parado em corda
		pthread_mutex_unlock(&turn); 
		// liberar mutex_count_ba porque outro macaco A -> B precisa conseguir fazer incremetno
		pthread_mutex_unlock(&mutex_count_ba);

		printf("Macaco %d passando de B para A\n", i); // regiao critica conceitual
		// sleep(1);
		
		// ermina de atravessar, ele protege o contador AB antes de diminuí-lo
        pthread_mutex_lock(&mutex_count_ba);
        count_ba--;
        printf("Macaco %d terminou de passar de B para A. Num: %d\n" , i, count_ba);
		if(count_ba == 0){ // ultimo macaco da mutex_count_ba ab passou
			pthread_mutex_unlock(&corda); // libera corda: um grupo B -> A pode assumir; um gorila pode entrar
		}
		pthread_mutex_unlock(&mutex_count_ba); // libera mutex_count_ba que protege o contador
	}
	pthread_exit(0);
}

void * gorilaAB(void * a){
	while(1){
		pthread_mutex_lock(&turn); // novos macacos nao podem entrar, mas os que ja estao na corda continuam
		pthread_mutex_lock(&corda); // gorila tenta pegar corda -> blocked se tiver macaco passando
		printf("Gorila passando de A para B\n"); // regiao critica conceitual
		// sleep(1); // gorila está na corda, novos macacos esperam em turno, outro gorila espera em turno
		pthread_mutex_unlock(&corda);
		pthread_mutex_unlock(&turn);
	}
	pthread_exit(0);
}

void * gorilaBA(void * a){
	while(1){
		//Procedimentos para acessar a corda
        pthread_mutex_lock(&turn);
        pthread_mutex_lock(&corda);
		printf("Gorila passando de B para A \n");
		// sleep(1);
        pthread_mutex_unlock(&corda);
        pthread_mutex_unlock(&turn);
	}
	pthread_exit(0);
}

int main(int argc, char * argv[]) {
	pthread_t macacos[MA+MB];
	int *id;
	int i = 0;

	for(i = 0; i < MA+MB; i++){
		id = (int *) malloc(sizeof(int));
		*id = i;
		if(i % 2 == 0) {
			if(pthread_create(&macacos[i], NULL, &macacoAB, (void*)id)){
				printf("Não pode criar a thread %d\n", i);
				return -1;
			}
		} else {
			if(pthread_create(&macacos[i], NULL, &macacoBA, (void*)id)){
				printf("Não pode criar a thread %d\n", i);
				return -1;
			}
		}
	}
	pthread_t g1, g2;
	pthread_create(&g1, NULL, &gorilaAB, NULL);
	pthread_create(&g2, NULL, &gorilaBA, NULL);

	pthread_join(macacos[0], NULL);
	return 0;
}
