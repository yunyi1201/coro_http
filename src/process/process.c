#include "process.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "common.h"
#include "env.h"
#include "event.h"
#include "net.h"
#include "sched.h"
static int listen_fd;

static inline void request_default_handler(int fd __UNUSED) {}
static worker_init_proc_t worker_init_proc = NULL;
static request_handler_t request_handler = request_default_handler;

static int connection_count = 1; /* if 0, worker exits. */

int shall_stop = 0; /* shall we stop the service? if 0, continue */
int shall_exit = 0; /* shall we force quit the service? */

/* */
static int create_tcp_server(char *ip, int port) {
  int listen_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (listen_fd == -1) {
    printf("Failed to initiate socket: %s\n", strerror(errno));
    exit(0);
  }

  if (set_reuse_addr(listen_fd)) {
    printf("Failed to set reuse listen socket: %s\n", strerror(errno));
    exit(0);
  }

  if (set_nonblock(listen_fd)) {
    printf("Failed to set nonblock listen socket: %s\n", strerror(errno));
    exit(0);
  }

  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = ip_to_nl(ip);

  if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))) {
    printf("Failed to bind socket: %s\n", strerror(errno));
    exit(0);
  }

  if (listen(listen_fd, 1000)) {
    printf("Failed to listen socket: %s\n", strerror(errno));
    exit(0);
  }
  return listen_fd;
}

void tcp_server_init() {
  listen_fd = create_tcp_server(server_addr, server_port);
}

static inline void increase_conn() { connection_count++; }

static inline void decrease_conn_and_check() {
  if (--connection_count == 0) exit(0);
}

static void handle_connection(void *args) {
  int connfd = (int)(intptr_t)args;

  request_handler(connfd);
  close(connfd);
  decrease_conn_and_check();
}

static int worker_accept() {
  struct sockaddr addr;
  socklen_t addrlen;
  int connfd;
  connfd = accept(listen_fd, &addr, &addrlen);
  return connfd;
}

static void worker_accept_cycle(void *args __UNUSED) {
  for (;;) {
    // if (unlikely(shall_stop)) {
    //   decrease_conn_and_check();
    //   break;
    // }

    // if (unlikely(shall_exit))
    //   exit(0);

    int connfd = worker_accept();
    if (likely(connfd > 0)) {
      if (dispatch_coro(handle_connection, (void *)(intptr_t)connfd)) {
        // WARN("system busy to handle request.");
        close(connfd);
        continue;
      }
      increase_conn();
    } else if (connfd == 0) {
      schedule_timeout(200);
      continue;
    }
  }
}

void register_service(worker_init_proc_t worker_proc,
                      request_handler_t handler) {
  worker_init_proc = worker_proc;
  request_handler = handler;
}

void worker_process_cycle() {
  if (worker_init_proc()) {
    printf("Failed to initiate worker process");
    exit(0);
  }

  schedule_init(coro_stack_kbytes, worker_connections);
  event_loop_init(worker_connections);
  dispatch_coro(worker_accept_cycle, NULL);
  //   INFO("worker success running...");
  schedule_cycle();
}