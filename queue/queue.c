#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

void queue_init(queue_t* q) {
  q->head = NULL;
  q->tail = NULL;
  q->length = 0;
}

int queue_push_back(queue_t* q, void* value) {
  QNODE_NEW(new_node, value);

  if(q->length == 0)
    q->head = new_node;
  else
    q->tail->next = new_node;

  q->tail = new_node;
  ++q->length;

  return SUCCESS;
}

void queue_pop_front(queue_t* q, void** ret) {
  qnode_t* first = q->head;

  // se ret for NULL, não é necessário capturar o valor a ser retirado da fila
  if(ret)
    *ret = first->value;
  q->head = first->next;

  free(first);
  --q->length;
}

void queue_destroy(queue_t* q) {
  qnode_t* node = q->head;

  // Atenção: caso algum nó da fila tiver como conteúdo um ponteiro alocado com malloc, esse ponteiro não será liberado. Qualquer
  // free realizado por essa função é responsável apenas por liberar recursos da própria fila, e não de terceiros!
  while(node != NULL) {
    q->head = q->head->next;
    free(node);
    node = q->head;
  }

  q->length = 0;
  q->head = q->tail = NULL;
}