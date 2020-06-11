#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "queue/queue.h"

int main() {

  int x = 10;
  int y = 3;
  int z = 77;

  void* retval = NULL;

  queue_t q;
  queue_init(&q, 3);

  printf("\nsize: %ld | head: %ld | tail %ld\n", q.length, q.head, q.tail);

  queue_push_back(&q, &x);
  printf("\nsize: %ld | head: %ld | tail %ld\n", q.length, q.head, q.tail);

  queue_push_back(&q, &y);
  printf("\nsize: %ld | head: %ld | tail %ld\n", q.length, q.head, q.tail);

  queue_push_back(&q, &z);
  printf("\nsize: %ld | head: %ld | tail %ld\n", q.length, q.head, q.tail);


  queue_pop_front(&q, &retval);
  printf("Valor: %d\n", *(int*) retval);

  printf("\nsize: %ld | head: %ld | tail %ld\n", q.length, q.head, q.tail);

  queue_pop_front(&q, &retval);
  printf("Valor: %d\n", *(int*) retval);

  printf("\nsize: %ld | head: %ld | tail %ld\n", q.length, q.head, q.tail);

  queue_pop_front(&q, &retval);
  printf("Valor: %d\n", *(int*) retval);

  printf("\nsize: %ld | head: %ld | tail %ld\n", q.length, q.head, q.tail);

  queue_push_back(&q, &y);
  queue_push_back(&q, &z);

  printf("\nsize: %ld | head: %ld | tail %ld\n", q.length, q.head, q.tail);

  queue_pop_front(&q, &retval);
  printf("Valor: %d\n", *(int*) retval);

  printf("\nsize: %ld | head: %ld | tail %ld\n", q.length, q.head, q.tail);

  queue_push_back(&q, &x);

  printf("\nsize: %ld | head: %ld | tail %ld\n", q.length, q.head, q.tail);
  
  queue_pop_front(&q, &retval);
  printf("Valor: %d\n", *(int*) retval);

  printf("\nsize: %ld | head: %ld | tail %ld\n", q.length, q.head, q.tail);

  queue_pop_front(&q, &retval);
  printf("Valor: %d\n", *(int*) retval);

  printf("\nsize: %ld | head: %ld | tail %ld\n", q.length, q.head, q.tail);
  
  queue_destroy(&q);

  return 0;
}