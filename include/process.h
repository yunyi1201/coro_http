#pragma once

extern int shall_stop, shall_exit;

typedef void (*request_handler_t)(int fd);
typedef int (*worker_init_proc_t)();

void register_service(worker_init_proc_t worker_proc,
                      request_handler_t handler);

void worker_process_cycle();
void worker_exit_handler(int pid);

void tcp_server_init();
