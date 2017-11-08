#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

char** afficheIP(char *name)
{
    struct hostent *hello = gethostbyname(name); //envoi
    printf("%d.%d.%d.%d\n",(unsigned char)hello->h_addr_list[0][0],(unsigned char)hello->h_addr_list[0][1],(unsigned char)hello->h_addr_list[0][2],(unsigned char)hello->h_addr_list[0][3]);
    return hello->h_addr_list;
}

void afficheNom(char *IP)
{
	struct in_addr addr;
    inet_aton(IP, &addr);
    struct hostent *hello = gethostbyaddr(&addr, sizeof(addr), AF_INET); //envoi
    printf("%s\n",hello->h_name);
}

void affichePort(int descbrloc) // utile si port est a 0
{
  struct sockaddr_in info;
  socklen_t lgname = sizeof(&info);
  int rest= getsockname(descbrloc, (struct sockaddr*) &info, &lgname);
  printf("\n %d\n", ntohs(info.sin_port));
}

struct sockaddr_in creationBR(char *IP, int port, int *descbrloc)
{
  //création du buffer
  *descbrloc = socket(PF_INET, SOCK_DGRAM, 0); //DGRAM = UDP  STREAM = TCP
  if(*descbrloc == -1)
    printf("erreur ^^ \n");

  //création et configuration de la socket
  struct  sockaddr_in ad;
  ad.sin_family = AF_INET; //représente la famille de protocole (TCP, etc...)
  ad.sin_addr.s_addr = inet_addr(IP); //INADDR_ANY adresse IP local
  ad.sin_port = htons(port); //le port le 0 représente un port libre quelconque sinon "htons(numéro port)"

  //le bind = liaison entre le buffer et le socket
  int res= bind(*descbrloc,(struct sockaddr*)&ad, sizeof(ad));
  if(res < 0)
    printf("erreur again :( \n");

   return ad;
}