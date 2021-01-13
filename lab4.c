/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 4 */
/* Codigo: Uso de variáveis de condição e suas operações básicas para sincronização por condição */

/***** Condicao logica da aplicacao: as threads A so podem imprimir "Bye" depois que a thread B imprimir  "Hello"  ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  4

/* Variaveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

/* Thread A */
void *A (void *t) {
  long int id = (long int) t;

  if(id == 0) printf("tudo bem ?\n");
  if(id == 1) printf("bom dia!\n");

  pthread_mutex_lock(&x_mutex);
  x++;
  if(x == 2) pthread_cond_broadcast(&x_cond);
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}

/* Thread B */
void *B (void *t) {
  long int id = (long int) t;

  pthread_mutex_lock(&x_mutex);
  if (x < 2) { 
     pthread_cond_wait(&x_cond, &x_mutex);
  }
  pthread_mutex_unlock(&x_mutex);
  if(id == 2) printf("até mais!\n");
  if(id == 3) printf("boa tarde!\n"); 
  pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

  /* Cria as threads */
  pthread_create(&threads[0], NULL, A, (void *) 0);
  pthread_create(&threads[1], NULL, A, (void *) 1);
  pthread_create(&threads[2], NULL, B, (void *) 2);
  pthread_create(&threads[3], NULL, B, (void *) 3);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("\nFIM\n");

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
  return 0;
}
