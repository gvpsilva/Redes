#include "common.h"

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <math.h>

#define BUFSZ 1024
void usage(int argc, char **argv) // USADA PARA QUANDO OCORRE ERRO DE CONEXÃO DEVIDO PROTOCOLO OU PORTA
{
  printf("usage: %s <ipv4|ipv6> <server port>\n", argv[0]);
  printf("example: %s v4 5151\n", argv[0]);
  exit(EXIT_FAILURE);
}
struct client_data {
    int csock;
    struct sockaddr_storage storage;
    char buf[BUFSZ];
    ssize_t count;
};
void *client_tread(void *data) // CRIA UMA CONEXÃO COM O CLIENTE, PODENDO DEPENDENDO DO PROGRAMA UTILIZAR MULTI-THREAD
{
  struct client_data *cdata = (struct client_data *)data; // OBTEM OS DADOS DO CLIENTE E COLOCA NA STRUCT CDATA
  struct sockaddr *caddr = (struct sockaddr *)(&cdata->storage); //ACESSA INFORMAÇÕES DO ENDEREÇO DO CLIENTE
  char caddrstr[BUFSZ];
  addrtostr(caddr, caddrstr, BUFSZ);

  char buf[BUFSZ];
  memset(buf, 0, BUFSZ);
  size_t count = cdata->count; // RECEBE OS DADOS DO CLIENTE
  if (count == -1)
  {
    logexit("recv");
  }
  if (cdata->buf != NULL && strlen(cdata->buf) != 0) {
        if (atoi(cdata->buf) == 1) {
            snprintf(buf, BUFSZ, "%s", "Um anel para a todos governar");
            int sent_count = sendto(cdata->csock, buf, strlen(buf), 0, caddr, sizeof(cdata->storage));
            if (sent_count != strlen(buf)) {
                logexit("send");
            }
            usleep(3000000);
             snprintf(buf, BUFSZ, "%s", "Na terra de Mordor onde as sombras se deitam");
             sent_count = sendto(cdata->csock, buf, strlen(buf), 0, caddr, sizeof(cdata->storage));
            if (sent_count != strlen(buf)) {
                logexit("send");
            }
             usleep(3000000);
             snprintf(buf, BUFSZ, "%s", "Não é o que temos, mas o que fazemos com o que temos");
             sent_count = sendto(cdata->csock, buf, strlen(buf), 0, caddr, sizeof(cdata->storage));
            if (sent_count != strlen(buf)) {
                logexit("send");
            }
             usleep(3000000);
             snprintf(buf, BUFSZ, "%s", "Não há mal que sempre dure");
             sent_count = sendto(cdata->csock, buf, strlen(buf), 0, caddr, sizeof(cdata->storage));
            if (sent_count != strlen(buf)) {
                logexit("send");
            }
             usleep(3000000);
            snprintf(buf, BUFSZ, "%s", "O mundo está mudando, senhor Frodo");
             sent_count = sendto(cdata->csock, buf, strlen(buf), 0, caddr, sizeof(cdata->storage));
            if (sent_count != strlen(buf)) {
                logexit("send");
            }
            snprintf(buf, BUFSZ, "%d", -1);
             sent_count = sendto(cdata->csock, buf, strlen(buf), 0, caddr, sizeof(cdata->storage));
            if (sent_count != strlen(buf)) {
                logexit("send");
            }
        }
        if(atoi(cdata->buf)== 2){
           snprintf(buf, BUFSZ, "%s", "Vou fazer uma oferta que ele não pode recusar");
            int sent_count = sendto(cdata->csock, buf, strlen(buf), 0, caddr, sizeof(cdata->storage));
            if (sent_count != strlen(buf)) {
                logexit("send");
            }
            usleep(3000000);
            snprintf(buf, BUFSZ, "%s", "Mantenha seus amigos por perto e seus inimigos mais perto ainda");
            sent_count = sendto(cdata->csock, buf, strlen(buf), 0, caddr, sizeof(cdata->storage));
            if (sent_count != strlen(buf)) {
                logexit("send");
            }
            usleep(3000000);
            snprintf(buf, BUFSZ, "%s", "É melhor ser temido que amado");
            sent_count = sendto(cdata->csock, buf, strlen(buf), 0, caddr, sizeof(cdata->storage));
            if (sent_count != strlen(buf)) {
                logexit("send");
            }
            usleep(3000000);
            snprintf(buf, BUFSZ, "%s", "A vingança é um prato que se come frio");
            sent_count = sendto(cdata->csock, buf, strlen(buf), 0, caddr, sizeof(cdata->storage));
            if (sent_count != strlen(buf)) {
                logexit("send");
            }
            usleep(3000000);
             snprintf(buf, BUFSZ, "%s","Nunca deixe que ninguém saiba o que você está pensando");
            sent_count = sendto(cdata->csock, buf, strlen(buf), 0, caddr, sizeof(cdata->storage));
            if (sent_count != strlen(buf)) {
                logexit("send");
            }
            snprintf(buf, BUFSZ, "%d", -1);
             sent_count = sendto(cdata->csock, buf, strlen(buf), 0, caddr, sizeof(cdata->storage));
            if (sent_count != strlen(buf)) {
                logexit("send");
            }
            
        }
         if(atoi(cdata->buf)== 3){
           snprintf(buf, BUFSZ, "%s", "Primeira regra do Clube da Luta: você não fala sobre o Clube da Luta");
            int sent_count = sendto(cdata->csock, buf, strlen(buf), 0, caddr, sizeof(cdata->storage));
            if (sent_count != strlen(buf)) {
                logexit("send");
            }
            usleep(3000000);
            snprintf(buf, BUFSZ, "%s","Segunda regra do Clube da Luta: você não fala sobre o Clube da Luta");
            sent_count = sendto(cdata->csock, buf, strlen(buf), 0, caddr, sizeof(cdata->storage));
            if (sent_count != strlen(buf)) {
                logexit("send");
            }
            usleep(3000000);
            snprintf(buf, BUFSZ, "%s","O que você possui acabará possuindo você");
            sent_count = sendto(cdata->csock, buf, strlen(buf), 0, caddr, sizeof(cdata->storage));
            if (sent_count != strlen(buf)) {
                logexit("send");
            }
            usleep(3000000);
            snprintf(buf, BUFSZ, "%s","É apenas depois de perder tudo que somos livres para fazer qualquer coisa");
            sent_count = sendto(cdata->csock, buf, strlen(buf), 0, caddr, sizeof(cdata->storage));
            if (sent_count != strlen(buf)) {
                logexit("send");
            }
            usleep(3000000);
            snprintf(buf, BUFSZ, "%s","Escolha suas lutas com sabedoria");
            sent_count = sendto(cdata->csock, buf, strlen(buf), 0, caddr, sizeof(cdata->storage));
            if (sent_count != strlen(buf)) {
                logexit("send");
            }
            snprintf(buf, BUFSZ, "%d", -1);
             sent_count = sendto(cdata->csock, buf, strlen(buf), 0, caddr, sizeof(cdata->storage));
            if (sent_count != strlen(buf)) {
                logexit("send");
            }
        }
    }

    close(cdata->csock);
    pthread_exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
  if (argc < 3)
  {
    usage(argc, argv);//valida os dados de conexão
  }
  struct sockaddr_storage storage; //ARMAZENA INFORMAÇÕES DE CONEXÃO DO SERVIDOR
  if (0 != server_sockaddr_init(argv[1], argv[2], &storage)) //INICIALIZA O SERVIDOR
  {
    usage(argc, argv); //VALIDA NOVAMENTE
  }
    int s = socket(storage.ss_family, SOCK_DGRAM, 0); //cria o SICKET
  if (s == -1)
  {
    logexit("socket");
  }
  int enable = 1;
  if (0 != setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int))) //DEFINE OPÇÕES PARA UM SOCKET
  {
    logexit("setsockopt");
  }
  struct sockaddr *addr = (struct sockaddr *)(&storage); //CRIA UMA ESTRUTURA PARA ARMAZENA INFORMAÇÕES DE ENDEREÇO
  if (0 != bind(s, addr, sizeof(storage))) //ASSOCIA UM ENDEREÇO LOCAL AO SOCKET
  {
    logexit("bind");
  }

  while (1)
  {
  
     struct client_data *cdata = malloc(sizeof(*cdata)); // Aloca memória para os dados do cliente
        if (!cdata) {
            logexit("malloc");
        }
        cdata->csock = s;
        struct sockaddr_storage cstorage; // Armazena informações do endereço do cliente
        struct sockaddr *caddr = (struct sockaddr *)(&cstorage); // Ponteiro para um sockaddr
        socklen_t caddrlen = sizeof(cstorage); // Tamanho da estrutura

        memset(cdata->buf, 0, BUFSZ);
        cdata->count = recvfrom(s, cdata->buf, BUFSZ, 0, caddr, &caddrlen); // Recebe dados do cliente
        if (cdata->count == -1) {
            logexit("recvfrom");
        }

        memcpy(&(cdata->storage), &cstorage, sizeof(cstorage)); // Copia os dados do endereço do cliente

        pthread_t tid; // Declara um identificador da thread que será criada
        pthread_create(&tid, NULL, client_tread, cdata); // Cria uma thread para tratar a mensagem
        pthread_detach(tid); 
  }
  exit(EXIT_SUCCESS);
}
