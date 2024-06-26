#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
void logexit(const char *msg)
{
  perror(msg);
  exit(EXIT_FAILURE);
}

int addrparse(const char *addstr, const char *portstr, struct sockaddr_storage *storage)
{
  if (addstr == NULL || portstr == NULL)
  {
    return -1;
  }
  uint16_t port = (uint16_t)atoi(portstr); // unsigned short
  if (port == 0)
  {
    return -1;
  }
  port = htons(port); // host to network short

  struct in_addr inaddr4; // 32-bit IP address
  if (inet_pton(AF_INET, addstr, &inaddr4))
  {
    struct sockaddr_in *addr4 = (struct sockaddr_in *)storage;
    addr4->sin_family = AF_INET;
    addr4->sin_port = port;
    addr4->sin_addr = inaddr4;
    return 0;
  }
  struct in6_addr inaddr6; // 128-bit IPv6 address
  if (inet_pton(AF_INET6, addstr, &inaddr6))
  {
    struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)storage;
    addr6->sin6_family = AF_INET6;
    addr6->sin6_port = port;
    memcpy(&(addr6->sin6_addr), &inaddr6, sizeof(inaddr6));
    // addr6->sin6_addr= inaddr6;
    return 0;
  }
  return -1;
}

void addrtostr(const struct sockaddr *addr, char *str, size_t strsize)
{
  int version;
  char addstr[INET6_ADDRSTRLEN + 1] = "";
  uint16_t port;
  if (addr->sa_family == AF_INET)
  {
    version = 4;
    struct sockaddr_in *addr4 = (struct sockaddr_in *)addr;
    if (!inet_ntop(AF_INET, &(addr4->sin_addr), addstr, INET6_ADDRSTRLEN + 1))
    {
      logexit("ntop");
    }
    port = ntohs(addr4->sin_port);
  }
  else if (addr->sa_family == AF_INET6)
  {
    version = 6;
    struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)addr;
    if (!inet_ntop(AF_INET6, &(addr6->sin6_addr), addstr, INET6_ADDRSTRLEN + 1))
    {
      logexit("ntop");
    }
    port = ntohs(addr6->sin6_port);
  }
  else
  {
    logexit("unknown protocol family.");
  }
  if (str)
  {
    snprintf(str, strsize, "IPv%d %s %hu", version, addstr, port);
  }
}

int server_sockaddr_init(const char *proto, const char *portstr, struct sockaddr_storage *storage)
{
  uint16_t port = (uint16_t)atoi(portstr); // unsigned short
  if (port == 0)
  {
    return -1;
  }
  port = htons(port); // host to network short
  memset(storage, 0, sizeof(*storage));
  if (0 == strcmp(proto, "ipv4"))
  {
    struct sockaddr_in *addr4 = (struct sockaddr_in *)storage;
    addr4->sin_family = AF_INET;
    addr4->sin_port = port;
    addr4->sin_addr.s_addr = INADDR_ANY;
    return 0;
  }
  else if (0 == strcmp(proto, "ipv6"))
  {
    struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)storage;
    addr6->sin6_family = AF_INET6;
    addr6->sin6_addr = in6addr_any;
    addr6->sin6_port = port;
  }
  else
  {
    return -1;
  }
  return 0;
}