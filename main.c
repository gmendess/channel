#include <stdlib.h>
#include <stdio.h>
#include "chan/chan.h"

chan_t ch;

void print_number(void* chan_value, void* args) {
  printf("%s: %d\n", (char*) args, *(int*) chan_value);
  free(chan_value);
}

void* producer(void* args) {

  while(1) {
    int* value = malloc(sizeof(int));
    
    scanf("%d", value);
    if(*value == 0) {
      free(value);
      break;
    }
    
    chan_send(&ch, value);
    puts("Produced!");
  }

  chan_close(&ch);

  pthread_exit(0);
}

void* consumer(void* args) {
  void* value = NULL;

  chan_for_range(&ch, print_number, "Valor impresso: ");
  puts("channel closed!");

  pthread_exit(0);
}

int main() {

  chan_init(&ch, 2);
  pthread_t prod_id, con1_id, con2_id;
  
  pthread_create(&prod_id, NULL, producer, NULL);
  pthread_create(&con1_id, NULL, consumer, NULL);
  pthread_create(&con2_id, NULL, consumer, NULL);

  pthread_join(prod_id, NULL);
  pthread_join(con1_id, NULL);
  pthread_join(con2_id, NULL);

  chan_destroy(&ch);

  return 0;
}