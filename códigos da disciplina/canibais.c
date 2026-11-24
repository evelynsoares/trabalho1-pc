#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAXCANIBAIS 20

void *canibal(void*meuid);
void *cozinheiro(int m);

pthread_mutex_t mutex_caldeirao = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t pode_servir = PTHREAD_COND_INITIALIZER;
pthread_cond_t pode_cozinhar = PTHREAD_COND_INITIALIZER;
int quantidade_comida = 0;

void main(argc, argv) int argc; char *argv[]; {
    int erro;
    int i, n, m;
    int *id;

    pthread_t tid[MAXCANIBAIS];

    if(argc != 3){
        printf("erro na chamada do programa: jantar <#canibais> <#comida>\n");
        exit(1);
    }

    n = atoi (argv[1]); //número de canibais
    m = atoi (argv[2]); // quantidade de porções que o cozinheiro consegue preparar por vez
    printf("numero de canibais: %d -- quantidade de comida: %d\n", n, m);

    if(n > MAXCANIBAIS){
        printf("o numero de canibais é maior que o maximo permitido: %d\n", MAXCANIBAIS);
        exit(1);
    }

    for (i = 0; i < n; i++)  {
        id = (int *) malloc(sizeof(int));
        *id = i;
        erro = pthread_create(&tid[i], NULL, canibal, (void *) (id));

        if(erro){
        printf("erro na criacao do thread %d\n", i);
        exit(1);
        }
    }

    cozinheiro(m);
}

void * canibal (void* pi){
    while(1) {
        //pegar uma porção de comida e acordar o cozinheiro se as porções acabaram
        pthread_mutex_lock(&mutex_caldeirao);
        while (quantidade_comida == 0) { // ta vazio
            // canibais esperam ter porcao
            printf("%d: caldeirao vazio, vou esperar\n", *(int *)(pi));
            pthread_cond_wait(&pode_servir, &mutex_caldeirao);
        } // saiu do while, tem comida
        printf("sou %d: vou me servir, tem %d porcoes\n", *(int *)(pi),quantidade_comida);
        quantidade_comida--;

        // se for a ultima porcao, acordar o cozinheiro
        if (quantidade_comida == 0) {
            printf("%d: vou acordar o cozinheiro\n", *(int *)(pi));
            pthread_cond_signal(&pode_cozinhar);
        }

        pthread_mutex_unlock(&(mutex_caldeirao)); 
        printf("%d: vou comer a porcao que peguei\n", *(int *)(pi));
        sleep(2);
    }
}

void *cozinheiro (int m){
    int contador;
    while(1) {
        pthread_mutex_lock(&mutex_caldeirao); 
        
        while (quantidade_comida > 0) { // tem porcao
            printf("vou aguardar, tem %d porcoes\n", quantidade_comida);
            pthread_cond_wait(&pode_cozinhar, &mutex_caldeirao); // aguarda ficar vazio
        } // saiu do while, ficou vazio
        printf("vou cozinhar\n");
        sleep(1);
        quantidade_comida += m;
        pthread_cond_broadcast(&pode_servir);    
        pthread_mutex_unlock(&mutex_caldeirao);   
    }

}


/*
# O problema dos canibais

## Descrição
Uma tribo de N canibais selvagens está jantando 
os missionários capturados. Eles ficam ao redor de 
um grande caldeirão contendo X porções de ensopado 
de missionário cozido. Quando um selvagem tem fome, 
ele se serve no caldeirão (pega 1 porção), a menos 
que esteja vazio. Nesse caso, o canibal deve 
aguardar/dormir até que o cozinheiro forneça mais porções.

Quando um canibal pegar a última porção, o mesmo 
deve acordar o cozinheiro para que ele prepare mais 
X porções de ensopado e encha o caldeirão. Após encher 
a tigela, o cozinheiro acorda os canibais selvagens 
que estavam esperando o ensopado ficar pronto e volta a dormir.

## Restrições
- Canibais não podem se servir ao mesmo tempo.
- Canibais podem comer ao mesmo tempo.
- Canibais não podem se servir se o caldeirão estiver vazio.
- Canibais só podem se servir depois que o cozinheiro terminar de preparar o jantar (as porções).
- Se o caldeirão estiver vazio, os canibais esperam dormindo.
- O cozinheiro só pode encher o caldeirão quando ele estiver vazio.

*/