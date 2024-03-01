#include "server.h"
#include "util/defs.h"
#include "util/error.h"
#include "util/logger.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

internal void *http_server_thread_entrypoint(IN void *handle) {
  uint64_t result;
  uint32_t client_fd;

  struct HTTPServer *server = (struct HTTPServer *)handle;
  bool graceful_shutdown = true;

  while (server->status & HTTP_SERVER_RUNNING) {
  }

  return (void *)graceful_shutdown;
}

Error http_server_init(IN OUT struct HTTPServer *server,
                       IN const char *bind_address, IN uint16_t port) {
  uint32_t result = 0;
  struct sockaddr_in bind_addr;

  if (server == NULL) {
    return ERR("invalid server handle given", NULL);
  }

  if (server->status & HTTP_SERVER_INITIALIZED) {
    return ERR("the server is already initialized", NULL);
  }

  //  Clean all old state if there is old state
  memset(server, 0, sizeof(*server));

  server->server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (server->server_fd == -1) {
    return ERR("couldn't create socket", strerror(errno));
  }

  // Clean the address space
  memset(&bind_addr, 0, sizeof(bind_addr));

  bind_addr.sin_addr.s_addr = inet_addr(bind_address);
  bind_addr.sin_port = htons(port);
  bind_addr.sin_family = AF_INET;

  result = bind(server->server_fd, (const struct sockaddr *)&bind_addr,
                sizeof(struct sockaddr_in));

  if (result != 0) {
    return ERR("couldn't bind socket", strerror(errno));
  }

  server->status |= HTTP_SERVER_INITIALIZED;
  server->configuration.address = bind_address;
  server->configuration.port = port;

  return SUCCESS();
}

Error http_server_start(IN OUT struct HTTPServer *server) {
  uint64_t result;

  if (!server) {
    return ERR("invalid server handle given", NULL);
  }

  if (http_server_closed(server)) {
    return ERR("http server is closed", NULL);
  }

  server->status |= HTTP_SERVER_RUNNING;

  result = pthread_create(&server->thread_id, NULL,
                          http_server_thread_entrypoint, (void *)server);

  if (result != 0) {
    server->status ^= HTTP_SERVER_ERROR | HTTP_SERVER_RUNNING;

    return ERR("couldn't create server thread", strerror(result));
  }

  return SUCCESS();
}

bool http_server_closed(IN struct HTTPServer *server) {
  fd_set descriptors;
  struct timeval timeout = {0, 0};

  uint64_t result;
  char chr;

  if (server == NULL) {
    return true;
  }

  if (server->server_fd == -1) {
    return true;
  }

  FD_ZERO(&descriptors);
  FD_SET(server->server_fd, &descriptors);

  result = select(server->server_fd + 1, &descriptors, NULL, NULL, &timeout);

  if (result == -1) {
    return true;
  }

  if (result != 0) {
    return true;
  }

  result = recv(server->server_fd, &chr, 0, MSG_PEEK | MSG_WAITALL);

  if (result == -1 && (errno == EWOULDBLOCK || errno == EAGAIN)) {
    return false;
  }

  if (result <= 0)
    return true;

  return false;
}

Error http_server_destroy(IN OUT struct HTTPServer *server) {
  if (!server) {
    return ERR("invalid server handle given", NULL);
  }

  //  Stop the server and wait the server thread exiting
  server->status &= ~(HTTP_SERVER_RUNNING);
  pthread_join(server->thread_id, NULL);

  //  Close the socket
  close(server->server_fd);
  server->server_fd = -1;

  return SUCCESS();
}
