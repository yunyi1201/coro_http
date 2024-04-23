#pragma once

/* associated with configurations */
extern char *log_path;
extern char *log_strlevel;

extern int worker_connections;
extern int coro_stack_kbytes;

extern char *server_addr;
extern int server_port;

void print_env();
void conf_env_init();
