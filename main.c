#include <stdlib.h>
#include <stdio.h>
#include "queue/queue.h"

int main() {
  
  queue_t q;
  queue_init(&q);
  int x = 10;
  int y = 20;

  queue_push_back(&q, &x);
  queue_push_back(&q, &y);
  
  void* popped_value;
  queue_pop_front(&q, &popped_value);
  printf("%d\n", *(int*) popped_value);

  queue_pop_front(&q, &popped_value);
  printf("%d\n", *(int*) popped_value);

  queue_push_back(&q, &y);
  queue_pop_front(&q, &popped_value);
  printf("%d\n", *(int*) popped_value);
  
  return 0;
}