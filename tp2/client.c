#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <math.h>

#define BUFSZ 1024


void usage(int argc, char **argv) // Exibe Mensagem caso ocorra erro com relação a conexão.
{
    printf("usage: %s<ipv4|ipv6> <server IP> <server port>\n", argv[0]);
    printf("example: %s 127.0.0.1 5151\n", argv[0]);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        usage(argc, argv);
    }

    struct sockaddr_storage storage;
    if (0 != addrparse(argv[2], argv[3], &storage))
    {
        usage(argc, argv);
    }

    int option = -1;
    while (option != 0)
    {
         int s = socket(storage.ss_family, SOCK_DGRAM, 0); // Cria um novo socket UDP
        if (s == -1)
        {
            logexit("socket");
        }
        struct sockaddr *addr = (struct sockaddr *)(&storage);
         socklen_t addrlen = sizeof(storage);
        fprintf(stderr, "0 - Sair\n1 - Senhor dos Anéis\n2 - O Poderoso Chefão\n3 - Clube da Luta\n");
        scanf("%d", &option);
        if (option == 0)
        {
            break;
        }
        char buf[BUFSZ];
        snprintf(buf, BUFSZ, "%d", option);
        int count = sendto(s, buf, strlen(buf), 0, addr, addrlen);
        if (count != strlen(buf))
        {
            logexit("send");
        }
                    while (1) {
                memset(buf, 0, BUFSZ);
                int count = recvfrom(s, buf, BUFSZ, 0, NULL, NULL);
                if (count == -1) {
                    logexit("recvfrom");
                }

                // Verifica se o servidor indicou fim das mensagens
                if (strcmp(buf, "-1") == 0) {
                    break; // Sai do loop ao receber o sinal de fim
                }

                printf("%s\n", buf);
            }

        close(s);
    }
    // FECHA A COMUNICAÇÃO DO SOCKET
    exit(EXIT_SUCCESS); // ENCERRA A EXECUÇÃO NO TERMINAL
}