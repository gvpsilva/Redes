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
#define EARTH_RADIUS_KM 6371.0 // Raio da Terra em quilômetros
#ifndef M_PI
  #define M_PI 3.14159265358979323846 
#endif
double to_radians(double degrees)
{
  return degrees * M_PI / 180.0; // TRANSFORMA A COORDENADA GEOGRAFICA EM DEGRAUS PARA RADIANOS
}
void usage(int argc, char **argv) // USADA PARA QUANDO OCORRE ERRO DE CONEXÃO DEVIDO PROTOCOLO OU PORTA
{
  printf("usage: %s <ipv4|ipv6> <server port>\n", argv[0]);
  printf("example: %s v4 5151\n", argv[0]);
  exit(EXIT_FAILURE);
}
struct client_data //ESTRUTURA QUE ARMAZENA INFORMAÇÕES SOBRE O CLIENTE
{
  int csock;
  struct sockaddr_storage storage;
};
typedef struct //ESTRUTURA PARA ARMAZENAR CORDENADAS GEOGRAFICAS
{
  double latitude;
  double longitude;
} Coordinate;

double haversine_distance(Coordinate coord1, Coordinate coord2) //CALCULA DISTANCIA DE ACORDO COM A FORMULA HAVERSINE
{
  // Converte as coordenadas de graus para radianos
  double lat1 = to_radians(coord1.latitude);
  double lon1 = to_radians(coord1.longitude);
  double lat2 = to_radians(coord2.latitude);
  double lon2 = to_radians(coord2.longitude);

  // Diferença de latitude e longitude
  double dlat = lat2 - lat1;
  double dlon = lon2 - lon1;

  // Aplicação da fórmula de Haversine
  double a = pow(sin(dlat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(dlon / 2), 2);
  double c = 2 * atan2(sqrt(a), sqrt(1 - a));
  double distance = EARTH_RADIUS_KM * c;

  return distance; // Retorna a distância em quilômetros
}
Coordinate coordinate_server = {-19.8696, -43.9645}; // COORDENADAS DO ICEX, PONTO DE PARTIDA DO MOTORISTA AO CLIENTE

void *client_tread(void *data) // CRIA UMA CONEXÃO COM O CLIENTE, PODENDO DEPENDENDO DO PROGRAMA UTILIZAR MULTI-THREAD
{
  struct client_data *cdata = (struct client_data *)data; // OBTEM OS DADOS DO CLIENTE E COLOCA NA STRUCT CDATA
  struct sockaddr *caddr = (struct sockaddr *)(&cdata->storage); //ACESSA INFORMAÇÕES DO ENDEREÇO DO CLIENTE
  char caddrstr[BUFSZ];
  addrtostr(caddr, caddrstr, BUFSZ);

  char buf[BUFSZ];
  memset(buf, 0, BUFSZ);
  size_t count = recv(cdata->csock, buf, BUFSZ, 0);// RECEBE OS DADOS DO CLIENTE
  if (count == -1)
  {
    logexit("recv");
  }
  int option = -1;
  if (buf != NULL && strlen(buf) != 0) //VALIDA SE O CLIENTE REALMENTE MANDOU UM VALOR VALIDO
  {
    while (option != 0 && option != 1) //VALIDA A OPÇÃO A SER DIGITADA A SEGUIR.
    {
      printf("Corrida disponível:\n");
      printf("0-Recusar\n");
      printf("1-Aceitar\n");
      scanf("%d", &option);
    }
  }
  if (option == 0) // 0 CASO RECUSE A CORRIDA, SENDO ASSIM O SERVIDOR VOLTA A ESCUTAR E ESPERA UMA NOVA CONEXÃO
  {
    printf("Aguardando solicitação.\n");
    snprintf(buf, BUFSZ, "%f", -1.0);  //MONTA A MENSAGEM COM -1.0 PARA INDICAR QUE FOI RECUSADA A CORRIDA
    int count = send(cdata->csock, buf, strlen(buf) + 1, 0); //ENVIA A MENSAGEM
    if (count != strlen(buf) + 1)
    {
      logexit("send");
    }
    close(cdata->csock); //fECHA A CONEXÃO
  }
  if (option == 1) //OPÇÃO 1 INDICA QUE O MOTORISTA ACEITOU A CORRIDA
  {
    Coordinate client;
    if (sscanf(buf, "%lf %lf", &client.latitude, &client.longitude) != 2) // VALIDA SE A LATITUDE E A LONGITUDE FORAM INFORMADOS
    {
      printf("Erro ao analisar os dados recebidos do cliente.\n");
    }
    else
    {
      double distancia = haversine_distance(coordinate_server, client);
      while (distancia > 0)
      {
        snprintf(buf, BUFSZ, "%.2f", distancia); //MONTA A MENSAGEM COM A DISTANCIA
        int count = send(cdata->csock, buf, strlen(buf) + 1, 0); // Enviar a distância
        if (count != strlen(buf) + 1)
        {
          logexit("send");
        }
        distancia = distancia - 0.4;
        usleep(2000000); // Aguardar 2000ms  antes de enviar a próxima distância
      }

      printf("O motorista chegou!\n");
      printf("Aguardando solicitação.\n");
      snprintf(buf, BUFSZ, "%d", 0); //Monta a mensagem com zero indicando que o motorista chegou ao encontro com o cliente.
      int count = send(cdata->csock, buf, strlen(buf) + 1, 0);//Envia a mensagem
      if (count != strlen(buf) + 1)
      {
        logexit("send");
      }
    }
  }

  close(cdata->csock);//Fecha a conexão

  pthread_exit(EXIT_SUCCESS);//Encerra a exceção.
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
  int s;
  s = socket(storage.ss_family, SOCK_STREAM, 0); //cria o SICKET
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
  if (0 != listen(s, 10))
  {
    logexit("bind");
  }
  char addrstr[BUFSZ];
  addrtostr(addr, addrstr, BUFSZ);
  printf("Aguardando solicitação.\n");
  while (1)
  {
    struct sockaddr_storage cstorage; //ARMAZENA INFORMAÇÕES DO ENDEREÇO DO CLIENTE
    struct sockaddr *caddr = (struct sockaddr *)(&cstorage); //PONTEIRO PARA UM SOCKADDR
    socklen_t caddrlen = sizeof(cstorage); //TAMANHO DA ESTRUTURA
    int csock = accept(s, caddr, &caddrlen); //CRIADO PARA ACEITAR A CONEXÃO DO CLIENTE COM O SERVIDOR
    if (csock == -1)
    {
      logexit("accept");
    }
    struct client_data *cdata = malloc(sizeof(*cdata));//ALOCA MEMORIA PARA OS DADOS DO CLIENTE
    if (!cdata)
    {
      logexit("malloc");
    }
    cdata->csock = csock;
    memcpy(&(cdata->storage), &cstorage, sizeof(cstorage));//COPIA OS DADOS DO ENDEREÇO DO CLIENTE
    pthread_t tid;// DECLARA UM IDENTIFICADOR DA THREAD QUE SERÁ CRIADA
    pthread_create(&tid, NULL, client_tread, cdata); //CRIA UMA THREAD
  }
  exit(EXIT_SUCCESS);
}
