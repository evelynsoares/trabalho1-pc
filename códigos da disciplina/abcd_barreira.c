#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define PAC 10  
#define PBD 5   

pthread_barrier_t barrier;  

void * func_pac(void *arg) {
  int myid = *(int *)(arg);
  int n = 1;
  while(n-- >0){
     printf("A");
    pthread_barrier_wait(&barrier); 
     printf("C"); 
     
  }
}

void * func_pbd(void *arg) {
  int myid = *(int *)(arg);
  int n = 1;
  while(n-- > 0){
   pthread_barrier_wait(&barrier); 
    printf("B");
   
    printf("D");
   
  }
}


int main(int argc, char *argv[]) {
  
  pthread_t pacId[PAC];
  pthread_t pbdId[PBD];

  pthread_barrier_init(&barrier, NULL, PAC+PBD);


  int * id;
  int i;
  for (i = 0; i < PAC; i++){
     id = (int *) malloc(sizeof(int));
     *id = i;
     pthread_create(&pacId[i], NULL, func_pac, (void *) (id));

  }
  for (i = 0; i < PBD; i++){
     id = (int *) malloc(sizeof(int));
     *id = i;
     pthread_create(&pbdId[i], NULL, func_pbd, (void *) (id));

  }


  for (i = 0; i < PAC; i++){
       if(pthread_join(pacId[i], NULL)){
          printf("\n ERROR joining thread");
          exit(1);
       }
  }


  for (i = 0; i < PBD; i++){
       if(pthread_join(pbdId[i], NULL)){
          printf("\n ERROR joining thread");
          exit(1);
       }
  }

  printf("\nBye!\n");
}


