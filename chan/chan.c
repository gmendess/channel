#include "chan.h"
#include "../queue/queue.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>

int chan_init(chan_t* c, size_t capacity) {
  int status = SUCCESS;

  // inicializa o semáforo de leitura e checa por erro
  if((status = pthread_cond_init(&c->cond_read.cond, NULL)) != SUCCESS)
    goto ret;
 
  // inicializa o semáforo de escrita e checa por erro
  if((status = pthread_cond_init(&c->cond_write.cond, NULL)) != SUCCESS)
    goto error_1;

  // inicializa o mutex que evita race-condition nos membros do canal e checa por erro
  if((status = pthread_mutex_init(&c->mutex, NULL)) != SUCCESS)
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

void chan_close(chan_t* c) {
  // já fechado
  if(c->closed)
    return;

  c->closed = true;

  // verifica se o canal de leitura está ocupado, ou seja, alguém está esperando por informação
  if(c->cond_read.busy)
    pthread_cond_broadcast(&c->cond_read.cond); // se estiver, informa a todos que o canal foi fechado
  pthread_cond_destroy(&c->cond_read.cond); // destrói a variável condicional de leitura

  // verifica se o canal de escrita está ocupado, ou seja, alguém está tentando enviar uma informação
  if(c->cond_write.busy)
    pthread_cond_broadcast(&c->cond_write.cond); // informa a todos que o canal foi fechado
  pthread_cond_destroy(&c->cond_write.cond); // destrói a variável condicional de escrita
}

void chan_destroy(chan_t* c) {
  pthread_mutex_lock(&c->mutex);

  chan_close(c);

  // destrói a fila.
  // ATENÇÃO: caso algum conteúdo dos nós da fila seja alocado dinamicamente, ele não será
  // destruído, isso é responsabilidade de quem o alocou!
  queue_destroy(&c->queue);
  c->capacity = 0; // valor padrão da capacidade

  pthread_mutex_unlock(&c->mutex); // unlock na mutex
  pthread_mutex_destroy(&c->mutex); // destrói a mutex;
}

static int __unbuffered_chan_send(chan_t* c) {
  if(c->closed)
    return ECLOSED;

  // ninguém está esperando para poder ler
  while(!c->cond_read.busy) {
    chan_cond_wait(&c->cond_write, &c->mutex); // tentando escrever, espera pelo sinal de alguém tentando ler
    if(c->closed)
      return ECLOSED; // canal fechado enquanto esperava
  }

  return SUCCESS;
}

static int __buffered_chan_send(chan_t* c) {
  if(c->closed)
    return ECLOSED;

  // é necessário esperar para escrever, pois a fila está lotada
  while(c->queue.length == c->capacity) {
    // unlock no mutex e fica esperando um sinal em c->cond_write, indicando que pode inserir na fila
    chan_cond_wait(&c->cond_write, &c->mutex);

    // após receber o sinal de escrita, verifica se o canal foi fechado enquanto aguardava
    if(c->closed)
      return ECLOSED;

    // o while é pra verificar novamente se a fila está cheia para evitar spurious wakeup (https://en.wikipedia.org/wiki/Spurious_wakeup)
  }

  return SUCCESS;
}

int chan_send(chan_t* c, void* value) {
  int status = SUCCESS;

  pthread_mutex_lock(&c->mutex);

  if(c->capacity == 0)
    status = __unbuffered_chan_send(c);
  else
    status = __buffered_chan_send(c);

  if(status != ECLOSED)
    status = queue_push_back(&c->queue, value); // insere no final da fila o conteúdo passa em 'value

  pthread_cond_signal(&c->cond_read.cond); // informa que um valor foi inserido na fila
  pthread_mutex_unlock(&c->mutex);

  return status;
}

static int __unbuffered_chan_recv(chan_t* c) {
  if(c->closed)
    return ECLOSED;

  if(c->cond_write.busy)
    pthread_cond_signal(&c->cond_write.cond); // avisa que uma tentativa de leitura está tentando ser feita

  do {
    chan_cond_wait(&c->cond_read, &c->mutex); // dá um unlock na mutex para que a thread que deseja inserir possa continuar a ação e espera pela resposta

    if(c->closed) // canal foi fechado enquanto esperava
      return ECLOSED;

    // o do-while é pra verificar se o item realmente foi inserido na fila e evitar spurious wakeup (https://en.wikipedia.org/wiki/Spurious_wakeup)
  } while(c->queue.length == 0);

  return SUCCESS;
}

static int __buffered_chan_recv(chan_t* c) {
  // tamanho da fila é 0, não há nada pra ser lido 
  while(c->queue.length == 0) {
    // tentativa de ler de um canal fechado e vazio
    if(c->closed)
      return ECLOSED;

    // unlock no mutex e fica esperando um sinal em c->cond_read, indicando que um valor pode ser lido
    chan_cond_wait(&c->cond_read, &c->mutex);

    // o while é pra verificar novamente se o item realmente foi inserido na fila e evitar spurious wakeup (https://en.wikipedia.org/wiki/Spurious_wakeup)
  }

  return SUCCESS;
}

int chan_recv(chan_t* c, void** ret) {
  int status = SUCCESS;

  pthread_mutex_lock(&c->mutex);

  // verifica se o channel é buffered ou unbuffered
  if(c->capacity == 0)
    status = __unbuffered_chan_recv(c);
    // channel unbuffered não avisa que o conteúdo foi lido, pois poderia influenciar em uma segunda escrita
  else {
    status = __buffered_chan_recv(c);
    pthread_cond_signal(&c->cond_write.cond);
  }

  if(status == SUCCESS)
    queue_pop_front(&c->queue, ret); // retira um item da fila e insere o conteúdo em ret.

  pthread_mutex_unlock(&c->mutex);

  return status;
}

void chan_for_range(chan_t* ch, void (*f)(void* chan_value, void* args), void* args) {
  void* value = NULL;
  while(chan_recv(ch, &value) != ECLOSED) {
    f(value, args);
  }
}