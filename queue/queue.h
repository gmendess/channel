#ifndef _CHANNEL_QUEUE_H
#define _CHANNEL_QUEUE_H

#include <stdlib.h>
#include "../errors/errors.h"

struct qnode {
  void* value; // conteúdo armazenado pelo nó
  struct qnode* next; // próximo nó
};
typedef struct qnode qnode_t;

#define QNODE_NEW(name, value) \
  qnode_t* name = malloc(sizeof(qnode_t)); \
  if(!name) \
    return ENOMEM; \
  name->value = value; \
  name->next = NULL \
 

struct queue {
  qnode_t* head; // ponteiro pro primeiro nó da fila
  qnode_t* tail; // ponteiro pro último nó da fila
  size_t length; // número atual de nós
};
typedef struct queue queue_t;

void queue_init(queue_t* q); // inicializa uma fila com os valores padrões
int queue_push_back(queue_t* q, void* value); // adiciona nó no final da fila
void queue_pop_front(queue_t* q, void** ret); // remove primeiro nó da fila
void queue_destroy(queue_t* q);

#endif // _CHANNEL_QUEUE_H