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

  queue_destroy(&q);
  
  return 0;
}