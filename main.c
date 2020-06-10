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

  queue_push_back(&q, &x);
  queue_push_back(&q, &y);
  queue_push_back(&q, &z);

  printf("size: %ld\n", q.length);

  queue_pop_front(&q, &retval);
  printf("%d\n", *(int*) retval);

  queue_pop_front(&q, &retval);
  printf("%d\n", *(int*) retval);
  
  queue_pop_front(&q, &retval);
  printf("%d\n", *(int*) retval);

  queue_destroy(&q);

  return 0;
}