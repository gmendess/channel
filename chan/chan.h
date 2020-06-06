#ifndef _CHANNEL_H
#define _CHANNEL_H

#include "../queue/queue.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdlib.h>

struct chan {
  queue_t queue;
  pthread_cond_t cond_read;
  pthread_cond_t cond_write;
  pthread_mutex_t mutex;
  bool closed;
  size_t capacity;
};
typedef struct chan chan_t;

int chan_init(chan_t* c, size_t capacity);
int chan_destroy(chan_t* c);
int chan_send(chan_t* c, void* value);
int chan_recv(chan_t* c, void** ret);

#endif // _CHANNEL_H