#include <stdlib.h>
#include <stdio.h>
#include "chan/chan.h"

chan_t ch;

void* producer(void* args) {

  int* value = NULL;
  for(int x = 0; x < 10000; x++) {
    value = malloc(sizeof(int));
    *value = x;
    chan_send(&ch, value);
  }

  chan_close(&ch);

  return NULL;
}

void process(void* v, void* args) {
  int* sum = (int*) args;
  *sum += *(int*) v;
  free(v);
}

void* consumer(void* args) {
  int x = 0;

  chan_for_range(&ch, process, &x);

  printf("Soma channel: %d\n", x);

  return NULL;
}

int main() {

  chan_init(&ch, 0);
  pthread_t prod_id, con1_id;
  
  pthread_create(&prod_id, NULL, producer, NULL);
  pthread_create(&con1_id, NULL, consumer, NULL);

  pthread_join(prod_id, NULL);
  pthread_join(con1_id, NULL);

  chan_destroy(&ch);

  size_t soma = 0;
  for(int x = 0; x < 10000; x++)
    soma += x;

  printf("Prova real: %lu\n", soma);

  return 0;
}