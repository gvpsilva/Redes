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

void usage(int argc, char **argv) //Exibe Mensagem caso ocorra erro com relação a conexão.
{
  printf("usage: %s <server IP> <server port>\n", argv[0]);
  printf("example: %s 127.0.0.1 5151\n", argv[0]);
  exit(EXIT_FAILURE);
}

typedef struct //struct de coordenadas geograficas
{
  double latitude;
  double longitude;
} Coordinate;

int main(int argc, char **argv)
{
  if (argc < 3)
  {
    usage(argc, argv);
  }

  struct sockaddr_storage storage;
  if (0 != addrparse(argv[1], argv[2], &storage))
  {
    usage(argc, argv);
  }

  int s = socket(storage.ss_family, SOCK_STREAM, 0); //cria o socket
  if (s == -1)
  {
    logexit("socket");
  }

  struct sockaddr *addr = (struct sockaddr *)(&storage);

  if (0 != connect(s, addr, sizeof(storage))) //busca estabelecer conexão com o servidor
  {
    fprintf(stderr, "Não foi encontrado um motorista\n");
    close(s);
    exit(EXIT_SUCCESS);
  }
  int option = -1;
  while (option != 0 && option != 1) //VALIDA SE UMA DAS OPÇÕES FOI SELECIONADA, SENDO O VALOR INICIAL -1, PARA CASO OCORRA ALGUMA RECUSA MOSTRE NOVAMENTE 
  {
    //MOSTRA OPÇÕES PARA O USUÁRIO SELECIONAR
    fprintf(stderr, "0-Sair\n");
    fprintf(stderr, "1-Solicitar Corrida\n");

    scanf("%d", &option);
    if (option == 1)
    {
     
      Coordinate coordCli = {-19.8642, -43.9620}; //Coordenadas Escola de Bela Artes

      char buf[BUFSZ];
      snprintf(buf, BUFSZ, "%lf %lf", coordCli.latitude, coordCli.longitude);//MONTA MENSAGEM A SER ENVIADA PARA O SERVIDOR

      int count = send(s, buf, strlen(buf) + 1, 0);//ENVIA A MENSAGEM PARA O SERVIDOR, SOLICITANDO A CORRIDA
      if (count != strlen(buf) + 1) //VALIDA SE A MENSAGEM NÃO ESTÁ NULA
      {
        logexit("send");
      }
      while (1)
      {
        memset(buf, 0, BUFSZ);
        count = recv(s, buf, BUFSZ, 0); //RECEBE DADOS DO SERVIDOR
        if (atof(buf) > 0)
        {
          double distance = atof(buf);//PEGA O VALOR DA DISTANCIA DE STRING E CONVERTE PARA DISTANCIA
          int distance_meters = (int)(distance * 1000); // Convertendo para metros
          if (distance_meters != 0)//VERIFICA SE A DISTANCIA É ZERO
          {
            printf("Motorista a %dm\n", distance_meters); //IMPRIMIE A DISTANCIA PARA O CLIENTE
          }
          
        }
        else if (atoi(buf) == -1)//CASO A CORRIDA SEJA RECUSADA PELO SERVIDOR
        {
          printf("Não foi encontrado um motorista.\n"); //MENSAGEM REOTRNADA PARA O USUÁRIO.
          option=-1; // OPTION RECEBE VALOR -1, PARA INDICAR QUE DEVE EXIBIR O MENU NOVAMENTE, PARA O USUÁRIO SOLICITAR NOVAMENTE O UBER OU CANCELAR A SOLICITAÇÃO.
          break;
        }
        else if (fabs(atof(buf)) < 0.01){
            printf("O motorista chegou!\n"); // INDICA QUE O MOTORISTA CHEGOU DEVIDO A DISTANCIA SER ZERO
            break;
        }

      }
    }
    else if (option == 0) //OPÇÃO 0 INDICA QUE O CLIENTE DIGITOU ZERO PARA SAIR
    {
      printf("Programa encerrado\n");
    }
  }
  close(s);//FECHA A COMUNICAÇÃO DO SOCKET
  exit(EXIT_SUCCESS); //ENCERRA A EXECUÇÃO NO TERMINAL
}
