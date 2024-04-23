#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "env.h"
// #include "conf.h"
// #include "util/str.h"
// #include "system.h"

char *log_path; /* log file path */
char *log_strlevel;

int worker_connections; /* Connection limit of each worker */
int coro_stack_kbytes;  /* stack size of coroutine in KiB */

char *server_addr; /* TCP server address */
int server_port;   /* server port */

// static void set_log_env() {
//   g_log_path = get_conf_entry("log_path");
//   g_log_strlevel = get_conf_entry("log_level");
// }

// static void set_worker_env() {
//   /* worker processes */
//   char *c = get_conf_entry("worker_processes");
//   g_worker_processes = str_equal(c, "default") ? get_ncpu() : atoi(c);
//   if (g_worker_processes < 0 || g_worker_processes > MAX_WORKER_PROCESS) {
//     printf("check worker_processes config:%d, should default or [0~%d]\n",
//            g_worker_processes, MAX_WORKER_PROCESS);
//     exit(0);
//   }

//   /* worker connections */
//   c = get_conf_entry("worker_connections");
//   g_worker_connections = atoi(c);
//   if (g_worker_connections <= 0) {
//     printf("check worker_connections config:%d, should > 0\n",
//            g_worker_connections);
//     exit(0);
//   }

//   /* coroutine stack size */
//   c = get_conf_entry("coroutine_stack_kbytes");
//   g_coro_stack_kbytes = str_equal(c, "default")
//                             ? get_page_size() >> 10
//                             : ALIGN(atoi(c) * 1024, get_page_size()) >> 10;
//   if (g_coro_stack_kbytes <= 0 || g_coro_stack_kbytes > 10240) {
//     printf("check coroutine_stack_kbytes: %d, should [%dKiB~10MiB]\n",
//            g_coro_stack_kbytes, get_page_size() >> 10);
//     exit(0);
//   }
// }

static void set_server_env() {
  server_addr = NULL;

  //   char *head = get_conf_entry("listen");
  //   for (char *loop = head; *loop; loop++) {
  //     if (*loop == ':') {
  //       g_server_addr = head;
  //       *loop = 0;
  //       head = loop + 1;
  //     }
  //   }

  //   g_server_port = atoi(head);
  //   if (g_server_port <= 0) {
  //     printf("check listen config: %d, should be positive\n", g_server_port);
  //     exit(0);

  //   }

  server_addr = "127.0.0.1";
  server_port = 8080;
}

// void print_env() {
//   printf("Number of processor(s)    : %d\n", get_ncpu());
//   printf("Log file path             : %s\n", g_log_path);
//   printf("Logging level             : %s\n", g_log_strlevel);
//   printf("Number of work processes  : %d\n", g_worker_processes);
//   printf("Connection of each worker : %d\n", g_worker_connections);
//   printf("Coroutine stack size      : %dKiB\n", g_coro_stack_kbytes);
//   printf("Web server listen port    : %s:%d\n",
//          g_server_addr ? g_server_addr : "localhost", g_server_port);
// }

void conf_env_init() {
  //   set_log_env();
  //   set_worker_env();
  worker_connections = 1024;
  coro_stack_kbytes = 4096;
  set_server_env();
}
