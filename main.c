#include <stdlib.h>
#include <stdio.h>
#include "chan/chan.h"

chan_t ch;

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

  printf("thread %ld >>> ", pthread_self());
  if(chan_recv(&ch, &value) != ECLOSED) {
    printf("%d\n", *(int*) value);
    free(value);
  }
  else {
    puts("Channel closed!");
  }

  pthread_exit(0);
}

int main() {

  chan_init(&ch, 2);
  pthread_t prod_id, con1_id, con2_id, con3_id;
  
  pthread_create(&prod_id, NULL, producer, NULL);
  pthread_join(prod_id, NULL);

  pthread_create(&con1_id, NULL, consumer, NULL);
  pthread_create(&con2_id, NULL, consumer, NULL);
  pthread_create(&con3_id, NULL, consumer, NULL);

  pthread_join(con1_id, NULL);
  pthread_join(con2_id, NULL);
  pthread_join(con3_id, NULL);

  chan_destroy(&ch);

  return 0;
}