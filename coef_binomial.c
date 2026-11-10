/* Trabalho 1 de Programação Concorrente
 * Evelyn Soares Pereira - 17/0102785
 *
 * Compilar: gcc -Wall -o coef_binomial coef_binomial.c -lpthread
 * Executar: ./coef_binomial
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// valores do problema: pra calcular outro C(n, k) 
// ou usar outro tamanho de buffer, é só mudar aqui
#define VALOR_N 10
#define VALOR_K 5
#define TAM_BUFFER 2

// variáveis compartilhadas
int buffer[TAM_BUFFER];
sem_t posicoes_livres;
sem_t fatores_prontos;
unsigned long long resultado;

void *denominador(void *arg) {
    int posicao_escrita = 0;

    for (int i = 1; i <= VALOR_K; i++) {
        // espera ter posição livre (evita a corrida 2: sobrescrita)
        if (sem_trywait(&posicoes_livres) != 0) {
            printf("Denominador: buffer cheio, vou esperar\n");
            sem_wait(&posicoes_livres);
        }

        sleep(1);  // tempo pra escrever o fator
        buffer[posicao_escrita] = i;
        printf("Denominador: escrevi o fator %d na posição %d\n", i, posicao_escrita);
        posicao_escrita = (posicao_escrita + 1) % TAM_BUFFER;

        // acorda o numerador, avisa que já tem um fator pronto
        sem_post(&fatores_prontos);
    }

    printf("Denominador: terminei\n");
    pthread_exit(NULL);
}

void *numerador(void *arg) {
    int posicao_leitura = 0;
    unsigned long long acumulado = 1;

    for (int i = 1; i <= VALOR_K; i++) {
        unsigned long long fator_num = (unsigned long long) (VALOR_N - i + 1);

        // multiplica pelo i-ésimo fator do numerador, checando se cabe em 64 bits
        if (acumulado > ULLONG_MAX / fator_num) {
            fprintf(stderr, "Erro: o valor intermediário passa de 64 bits no passo %d.\n", i);
            exit(1);
        }
        sleep(2);  // tempo pra multiplicar
        acumulado *= fator_num;
        printf("Numerador: multipliquei por %llu, acumulado = %llu\n", fator_num, acumulado);

        // espera ter fator pronto (evita a corrida 1)
        if (sem_trywait(&fatores_prontos) != 0) {
            printf("Numerador: buffer vazio, vou esperar\n");
            sem_wait(&fatores_prontos);
        }

        int fator_den = buffer[posicao_leitura];
        printf("Numerador: li o fator %d da posição %d\n", fator_den, posicao_leitura);
        posicao_leitura = (posicao_leitura + 1) % TAM_BUFFER;

        // libera a posição que acabou de ler, acorda o denominador
        sem_post(&posicoes_livres);

        // divisão exata pelo lema, acumulado = C(n, i)
        sleep(2);  // tempo pra dividir
        acumulado /= fator_den;
        printf("Numerador: dividi por %d, acumulado = %llu\n", fator_den, acumulado);
    }

    printf("Numerador: terminei\n");
    resultado = acumulado;
    pthread_exit(NULL);
}

int main() {
    printf("Calculando C(%d, %d) com buffer de %d posições\n\n", VALOR_N, VALOR_K, TAM_BUFFER);

    sem_init(&posicoes_livres, 0, TAM_BUFFER);  // o buffer começa todo livre
    sem_init(&fatores_prontos, 0, 0);           // e sem nenhum fator pronto

    pthread_t t_denominador, t_numerador;

    if (pthread_create(&t_denominador, NULL, denominador, NULL) != 0 ||
        pthread_create(&t_numerador, NULL, numerador, NULL) != 0) {
        fprintf(stderr, "Erro ao criar as threads.\n");
        return 1;
    }

    pthread_join(t_denominador, NULL);
    pthread_join(t_numerador, NULL);

    printf("\nResultado: C(%d, %d) = %llu\n", VALOR_N, VALOR_K, resultado);

    sem_destroy(&posicoes_livres);
    sem_destroy(&fatores_prontos);
    return 0;
}
