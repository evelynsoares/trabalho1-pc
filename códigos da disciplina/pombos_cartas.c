#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "unistd.h"

#define N 10 //número de usuários

#define CARTAS 20 //quantidade de cartas na mochila

pthread_mutex_t mutex_mochila = PTHREAD_MUTEX_INITIALIZER; // protege contador_cartas
pthread_cond_t cond_usuarios = PTHREAD_COND_INITIALIZER;   // usuários dormem aqui
pthread_cond_t cond_pombo = PTHREAD_COND_INITIALIZER;      // pombo dorme aqui

int contador_cartas = 0; // quantas cartas tem na mochila agora

void * f_usuario(void *arg);
void * f_pombo(void *arg);

int main(int argc, char **argv){
    int i;

    pthread_t usuario[N];
    int *id;
    for(i = 0; i < N; i++){
         id = (int *) malloc(sizeof(int));
          *id = i;
	pthread_create(&(usuario[i]),NULL,f_usuario,  (void *) (id));
    }
     pthread_t pombo;
     id = (int *) malloc(sizeof(int));
     *id = 0;
     pthread_create(&(pombo),NULL,f_pombo, (void*) (id));

     pthread_join(pombo,NULL);
   
}


void * f_pombo(void *arg){

    while(1){
        //Inicialmente está em A, aguardar/dorme a mochila ficar cheia (20 cartas)
        // trava mutex_mochila
        // enquanto contador_cartas < CARTAS:
        //     printf("Pombo: mochila com %d cartas, vou dormir\n", contador_cartas);
        //     espera em cond_pombo (com mutex_mochila)

        //Leva as cartas para B e volta para A
        printf("Pombo: voando de A para B com %d cartas\n", contador_cartas);
        sleep(1);
        printf("Pombo: voltando de B para A\n");
        sleep(1);
        // zera contador_cartas (entregou tudo em B)

        //Acordar os usuários
        // broadcast em cond_usuarios (vários usuários podem estar dormindo)
        // destrava mutex_mochila
    }
}

void * f_usuario(void *arg){
    int id = *((int *)arg);

    while(1){
        //Escreve uma carta
        printf("Usuario %d: vou escrever uma carta\n", id);
        sleep(1);

        //Caso o pombo não esteja em A ou a mochila estiver cheia, então dorme
        // trava mutex_mochila
        // enquanto contador_cartas == CARTAS:
        //     printf("Usuario %d: mochila cheia, vou esperar o pombo\n", id);
        //     espera em cond_usuarios (com mutex_mochila)

        //Posta sua carta na mochila do pombo
        // incrementa contador_cartas
        printf("Usuario %d: colei minha carta, mochila com %d cartas\n", id, contador_cartas);

        //Caso a mochila fique cheia, acorda o pombo
        // se contador_cartas == CARTAS:
        //     printf("Usuario %d: mochila cheia, vou acordar o pombo\n", id);
        //     signal em cond_pombo (só existe 1 pombo)
        // destrava mutex_mochila
    }
}
