#ifndef _CHANNEL_QUEUE_H
#define _CHANNEL_QUEUE_H

#include <stdlib.h>
#include "../errors/errors.h"

struct queue {
  size_t head; // indice em que o primeiro elemento se encontra, não é necessariamente 0
  size_t tail; // indice em que o próximo elemento será armazenado
  size_t length; // número atual de elementos
  size_t capacity; // capacidade máxima da fila (é igual a capacidade do channel)
  void** content; // array de void*, que armazenará 
};
typedef struct queue queue_t;

int queue_init(queue_t* q, size_t capacity); // inicializa uma fila com os valores padrões
void queue_push_back(queue_t* q, void* value); // adiciona nó no final da fila
void queue_pop_front(queue_t* q, void** ret); // remove primeiro nó da fila
void queue_destroy(queue_t* q);

#endif // _CHANNEL_QUEUE_H