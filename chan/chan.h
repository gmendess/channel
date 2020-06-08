#ifndef _CHANNEL_H
#define _CHANNEL_H

#include "../queue/queue.h"
#include "../errors/errors.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdlib.h>

// Esse tipo contém uma pthread_cond_t e um booleano indicando se a variável condicional está ou não
// em estado de waiting. A documentação diz que ao tentar destruir uma pthread_cond_t que ainda tem um
// pthread_cond_wait pendente, o comportamente depende da implementação, podendo retornar EBUSY ou ficar
// bloqueado até o sinal ser enviado (ref. https://linux.die.net/man/3/pthread_cond_destroy - vá em Errors). 
// Como não posso depender da implementação, vou fazer isso eu mesmo
struct chan_cond {
  pthread_cond_t cond;
  bool busy;
};
typedef struct chan_cond chan_cond_t;

// syntax-sugar 
// c = ponteiro para chan_cond_t
// mutex = ponteiro para pthread_mutex_t
#define chan_cond_wait(c, mutex) \
  (*c).busy = true; \
  pthread_cond_wait(c.cond, mutex); \
  (*c).busy = false;

struct chan {
  queue_t queue;
  chan_cond_t cond_read;
  chan_cond_t cond_write;
  pthread_mutex_t mutex;
  bool closed;
  size_t capacity;
};
typedef struct chan chan_t;

int chan_init(chan_t* c, size_t capacity);
void chan_close(chan_t* c);
void chan_destroy(chan_t* c);
int chan_send(chan_t* c, void* value);
int chan_recv(chan_t* c, void** ret);

#endif // _CHANNEL_H