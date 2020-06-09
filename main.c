#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "chan/chan.h"

void echo(void* _msg, void* args) {
  char* msg = (char*) _msg;
  int i = 0;

  printf("Mensagem ecoada em maiúsculo: ");
  while(msg[i]) {
    putchar(toupper(msg[i]));
    i++;
  }
  puts("\n");
}

void* recv_msg(void* args) {
  chan_t* ch = (chan_t*) args;

  chan_for_range(ch, echo, NULL);
  puts("canal fechado, nada mais será ecoado!");

  return NULL;
}

void* send_msg(void* args) {
  chan_t* ch = (chan_t*) args;

  char buf[256] = {0};  
  while(1) {
    fgets(buf, sizeof(buf), stdin);
    if(buf[0] == '\n')
      break;

    buf[strlen(buf) - 1] = '\0';
    chan_send(ch, buf);
  }

  chan_close(ch);
  pthread_exit(0);
}

int main() {

  chan_t ch;

  chan_init(&ch, 0);
  pthread_t prod_id, con1_id;

  pthread_create(&prod_id, NULL, send_msg, &ch);
  pthread_create(&con1_id, NULL, recv_msg, &ch);

  pthread_join(prod_id, NULL);
  pthread_join(con1_id, NULL);

  chan_destroy(&ch);

  return 0;
}