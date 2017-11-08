#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h> // Pour inet_ntoa

int main (int argc, char** argv)
{
  //création du buffer
  int brclient = socket(AF_INET, SOCK_STREAM, 0);
  if (brclient == -1)
    {
      perror("socket() : error\n");
      return EXIT_FAILURE;
    }
  else
    printf("Client : La socket est ouverte en mode TCP.\n");
  
  
  //création et configuration de la socket du client
  struct  sockaddr_in client;
  client.sin_family = AF_INET;
  client.sin_addr.s_addr = inet_addr("127.0.01"); // ou INADDR_ANY; ou htonl(INADDR_ANY);
  client.sin_port = htons((short)31400);
  
  
  //le bind = liaison entre le buffer et le socket
  int res = bind(brclient,(struct sockaddr*)&client, sizeof(client));
  if(res < 0)
    {
      perror("bind() : error\n");
      close(brclient);
      return EXIT_FAILURE;
    }
  
  printf("Client : le bind a été effectuer.\n");
  
  
  //création et configuration de la socket du serveur
  struct sockaddr_in Server;
  Server.sin_family = AF_INET;
  Server.sin_addr.s_addr = inet_addr("127.0.01"); // ou INADDR_ANY; ou htonl(INADDR_ANY);
  Server.sin_port = htons(31469);
  
  printf("Client : L'initialisation de la socket distante est terminé.\n");
  
  
  //demande de connection au serveur
  if (connect(brclient, (struct sockaddr*) &Server, sizeof(Server)) == -1)
    {
      perror("connect() : error\n");
      close(brclient);
      return EXIT_FAILURE;
    }
  else
    printf("Client : Connexion établit avec %s sur le port %d.\n",inet_ntoa(Server.sin_addr), htons(Server.sin_port));
  
  
  //initialisation tampon envoie en reception
  char message[1024] = {0};
  char buffer[1024] = {0};
  off_t size; //taille du fichier demande
  int resp=0; //nombre d'octets reçut lors du recv()
  int rcv=0; //nombre d'octets total reçut
  char c;
  

  //boucle principale de réception et d'envoie
  while(1)
    {
      //ouvre le fichier recu
      FILE* fd;


      //recois la taille du fichier
      if(recv(brclient, &size, sizeof(off_t), 0)<=0)
	{
	  perror("Client : erreur reception taille fichier.\n");
	  return EXIT_FAILURE;
	}


      //cree le nouveau fichier
      if((fd = fopen("toto2.jpg", "w+")) == NULL)
	{
	perror("Client : erreur création nouveau fichier.\n");
	return EXIT_FAILURE;
      }

      
      //tant que taille recu n'est pas taille du fichier
      while (rcv!=size)
	{
	  if((resp=recv(brclient, &c, sizeof(c),0))<=0)
	    {
	      perror("Client : erreur reception fichier.\n");
	      return EXIT_FAILURE;
	    }
	  rcv+=resp; //on additionne les octets juste recut et totaux deja recut 
	  fprintf(fd,"%c", c);
	}


      //ferme le fichier recu
      fclose(fd);
      close(brclient);
      return EXIT_SUCCESS;
    }
  close(brclient);
  return EXIT_SUCCESS;
}
