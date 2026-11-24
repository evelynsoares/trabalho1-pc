#include "pthread.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "stdbool.h"
#include "stdatomic.h"
#define N 10
#define X 100000

volatile bool wantp = false, wantq = false;
volatile int turn = 1;
int contador = 0;

void *processo_p(void *args) {
    for (int i = 0; i < X; i++) {
        wantp = true;
        atomic_thread_fence(memory_order_seq_cst);
        while (wantq) {
            if (turn == 2) {
                wantp = false;
                while (turn == 2) { }
                wantp = true;
            }
        }

        contador++;

        turn = 2;
        wantp = false;
    }
    pthread_exit(0);
}

void *processo_q(void *args) {
    for (int i = 0; i < X; i++) {
        wantq = true;
        atomic_thread_fence(memory_order_seq_cst);
        while (wantp) {
            if (turn == 1) {
                wantq = false;
                while (turn == 1) { }
                wantq = true;
            }
        }

        contador++;

        turn = 1;
        wantq = false;
    }
    pthread_exit(0);
}

int main() {
    pthread_t p, q;
    printf("O id da pthread main: %lu\n", pthread_self());

    pthread_create(&p, NULL, processo_p, NULL);
    pthread_create(&q, NULL, processo_q, NULL);

    printf("Valor do contador: %d antes do join\n", contador);

    pthread_join(p, NULL);
    pthread_join(q, NULL);

    printf("Valor do contador: %d depois do join\n", contador);
    return 0;
}