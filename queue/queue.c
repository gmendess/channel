#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

int queue_init(queue_t* q) {
  q->head = NULL;
  q->tail = NULL;
  q->length = 0;

  return 0;
}

int queue_push_back(queue_t* q, void* value) {
  QNODE_NEW(new_node, value);

  if(q->length == 0)
    q->head = new_node;
  else
    q->tail->next = new_node;
  
  q->tail = new_node;
  ++q->length;

  return 0;
}

int queue_pop_front(queue_t* q, void** ret) {
  if(q->length == 0)
    return 1;

  qnode_t* first = q->head;
  *ret = first->value;
  q->head = first->next;
  
  free(first);
  --q->length;
  
  return 0;
}