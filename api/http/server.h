#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "util/defs.h"
#include "util/error.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>

//  Status flags
#define HTTP_SERVER_INITIALIZED 0x01
#define HTTP_SERVER_RUNNING 0x02
#define HTTP_SERVER_ERROR 0x04

struct HTTPServer {
  uint32_t server_fd;
  uint32_t status;

  struct {
    uint16_t port;
    const char *address;
  } configuration;

  pthread_t thread_id;
};

Error http_server_init(IN OUT struct HTTPServer *server,
                       IN const char *bind_address, IN uint16_t port);

Error http_server_start(IN OUT struct HTTPServer *server);

bool http_server_closed(IN struct HTTPServer *server);

Error http_server_destroy(IN OUT struct HTTPServer *server);

#endif /** HTTP_SERVER_H */
