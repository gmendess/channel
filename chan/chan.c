#include "chan.h"
#include "../queue/queue.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>

int chan_init(chan_t* c, size_t capacity) {
  int status = 0;

  // inicializa o semáforo de leitura e checa por erro
  if((status = pthread_cond_init(&c->cond_read.cond, NULL)) != 0)
    goto ret;
 
  // inicializa o semáforo de escrita e checa por erro
  if((status = pthread_cond_init(&c->cond_write.cond, NULL)) != 0)
    goto error_1;

  // inicializa o mutex que evita race-condition nos membros do canal e checa por erro
  if((status = pthread_mutex_init(&c->mutex, NULL)) != 0)
    goto error_2;

  queue_init(&c->queue);
  c->cond_read.busy = false;
  c->cond_write.busy = false;
  c->closed = false;
  c->capacity = capacity;
  goto ret; // tudo ok, retorna status

  // error handling
  error_2:
    pthread_cond_destroy(&c->cond_write.cond);
  error_1:
    pthread_cond_destroy(&c->cond_read.cond);
  ret:
    return status;
}

int chan_destroy(chan_t* c) {
  pthread_cond_destroy(&c->cond_write);
  pthread_cond_destroy(&c->cond_read);
  pthread_mutex_destroy(&c->mutex);
  queue_destroy(&c->queue);
  c->closed = true;
  c->capacity = 0;

  return 0;
}

int chan_send(chan_t* c, void* value) {
  pthread_mutex_lock(&c->mutex);
  
  // tamanho da fila é igual à capacidade do canal, não é possível inserir mais nada 
  if(c->queue.length == c->capacity) {
    // unlock no mutex e fica esperando um sinal em c->cond_write, indicando que pode inserir na fila
    pthread_cond_wait(&c->cond_write, &c->mutex); 
  }

  queue_push_back(&c->queue, value);

  pthread_cond_signal(&c->cond_read); // informa que um valor foi inserido na fila
  pthread_mutex_unlock(&c->mutex);

  return 0;
}

int chan_recv(chan_t* c, void** ret) {
  pthread_mutex_lock(&c->mutex);

  // tamanho da fila é 0, não há nada pra ser lido 
  if(c->queue.length == 0) {
    // unlock no mutex e fica esperando um sinal em c->cond_read, indicando que um valor pode ser lido
    pthread_cond_wait(&c->cond_read, &c->mutex);
  }

  queue_pop_front(&c->queue, ret);

  pthread_cond_signal(&c->cond_write);
  pthread_mutex_unlock(&c->mutex);

  return 0;
}