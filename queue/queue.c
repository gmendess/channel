#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

int queue_init(queue_t* q, size_t capacity) {
  // se a capacidade do channel for 0, significa que ele é unbuffered, contudo mesmo assim é necessário
  // alocar espaço para ao menos um elemento, por isso a verificação
  q->capacity = capacity ? capacity : 1;
  
  q->content = calloc(q->capacity, sizeof(void*));
  if(q->content == NULL)
    return ENOMEM;
  
  q->head   = 0;
  q->tail   = 0;
  q->length = 0;

  return SUCCESS;
}


// Ao adicionar um elemento na fila, coloca ele na posição delimitada por q->tail e incrementa esse contador. Caso q->tail chegue na capacidade
// máxima da fila, ele vai para a posição 0, simulando uma circularidade. Note que não é feita nenhuma verificação se a fila está cheia, pois o
// channel já faz isso pra saber se ele deve, ou não, ficar bloqueado, então gastaria recursos para fazer a mesma coisa duas vezes.
void queue_push_back(queue_t* q, void* value) {
  q->content[q->tail] = value;
  q->tail = (q->tail + 1) % q->capacity; // cria efeito de circularidade na fila
  q->length++;
}

// Ao remover um elemento da fila, não é necessário mover todos os outros, apenas incrementar q->head. Caso q->head chegue na capacidade
// máxima da fila, ele retorna para a posição 0, simulando uma circularidade
void queue_pop_front(queue_t* q, void** ret) {  
  // se ret for NULL, não é necessário capturar o valor a ser retirado da fila
  if(ret)
    *ret = q->content[q->head];

  q->head = (q->head + 1) % q->capacity;
  q->length--;
}

void queue_destroy(queue_t* q) {
  free(q->content);

  q->content  = NULL;
  q->capacity = 0;
  q->length   = 0;
  q->head     = 0;
  q->tail     = 0;
}