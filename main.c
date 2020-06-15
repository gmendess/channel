#include <stdlib.h>
#include <stdio.h>
#include "chan/chan.h"

chan_t ch;

void* producer(void* args) {

  unsigned long long* value = NULL;
  for(unsigned long long x = 0; x < 10000000; x++) {
    value = malloc(sizeof(unsigned long long));
    *value = x;
    chan_send(&ch, value);
  }

  chan_close(&ch);

  return NULL;
}

void process(void* v, void* args) {
  unsigned long long* sum = (unsigned long long*) args;
  *sum += *(unsigned long long*) v;
  free(v);
}

void* consumer(void* args) {
  unsigned long long x = 0;

  chan_for_range(&ch, process, &x);

  printf("Soma channel: %llu\n", x);

  return NULL;
}

int main() {

  chan_init(&ch, 1000);
  pthread_t prod_id, con1_id;
  
  pthread_create(&prod_id, NULL, producer, NULL);
  pthread_create(&con1_id, NULL, consumer, NULL);

  pthread_join(prod_id, NULL);
  pthread_join(con1_id, NULL);

  chan_destroy(&ch);

  unsigned long long soma = 0;
  for(unsigned long long x = 0; x < 10000000; x++)
    soma += x;

  printf("Prova real: %llu\n", soma);

  return 0;
}