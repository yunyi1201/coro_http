#include "env.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "conf.h"
#include "str.h"
#include "common.h"



int worker_connections; /* Connection limit of each worker */
int coro_stack_kbytes;  /* stack size of coroutine in KiB */

char *server_addr; /* TCP server address */
int server_port;   /* server port */



static void set_worker_env() {


  /* worker connections */
  char *c = get_conf_entry("worker_connections");
  worker_connections = atoi(c);
  if (worker_connections <= 0) {
    printf("check worker_connections config:%d, should > 0\n",
           worker_connections);
    exit(0);
  }

  /* coroutine stack size */
  c = get_conf_entry("coroutine_stack_kbytes");
  coro_stack_kbytes = str_equal(c, "default")
                            ? 4096 >> 10
                            : ALIGN(atoi(c) * 1024, 4096) >> 10;
  if (coro_stack_kbytes <= 0 || coro_stack_kbytes > 10240) {
    printf("check coroutine_stack_kbytes: %d, should [%dKiB~10MiB]\n",
           coro_stack_kbytes, 4096 >> 10);
    exit(0);
  }
}

static void set_server_env() {

    char *head = get_conf_entry("listen");
    for (char *loop = head; *loop; loop++) {
      if (*loop == ':') {
        server_addr = head;
        *loop = 0;
        head = loop + 1;
      }
    }

    server_port = atoi(head);
    if (server_port <= 0) {
      printf("check listen config: %d, should be positive\n", server_port);
      exit(0);

    }


}

void print_env() {
  printf("Connection of each worker : %d\n", worker_connections);
  printf("Coroutine stack size      : %dKiB\n", coro_stack_kbytes);
  printf("Web server listen port    : %s:%d\n",
         server_addr ? server_addr : "localhost", server_port);
}

void conf_env_init() {
  set_worker_env();
  set_server_env();
}
