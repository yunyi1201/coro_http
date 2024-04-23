#include <stdio.h>
#include <stdlib.h>

#include "env.h"
#include "process.h"
#include "sched.h"
#include "str.h"

static void usage(const char *prog) {
  printf(
      "Usage: %s [command]\n"
      "command options:\n"
      "  conf  : check and show config file content\n"
      "  start : start the web server\n",
      prog);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    usage(argv[0]);
    return 0;
  }

  /* unsupported command */
  if (!str_equal(argv[1], "start")) {
    fprintf(stderr, "Unknown command: %s\n\n", argv[1]);
    usage(argv[0]);
    return 1;
  }

  /* start server */
  conf_env_init();
  tcp_server_init();

  worker_process_cycle();
  return 0;
}