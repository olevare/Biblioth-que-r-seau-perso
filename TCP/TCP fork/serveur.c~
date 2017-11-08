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
  int brserveur = socket(AF_INET, SOCK_STREAM, 0);
  if (brserveur == -1)
    {
      perror("socket() : error\n");
      return EXIT_FAILURE;
    }
  
  
  //création et configuration de la socket
  struct  sockaddr_in serveur;
  serveur.sin_family = AF_INET;
  serveur.sin_addr.s_addr = INADDR_ANY;
  serveur.sin_port = htons((short)31469);
  
  
  //le bind = liaison entre le buffer et le socket
  int res = bind(brserveur,(struct sockaddr*)&serveur, sizeof(serveur));
  if(res == -1)
    {
      perror("bind() : error\n");
      close(brserveur);
      return EXIT_FAILURE;
    }
  
  printf("Serveur : le bind a été effectuer.\n");
  
  
  //structure du client
  struct sockaddr_in client; 
  socklen_t lgclient = sizeof(client);
  
  
  //initialisation tampon envoie en reception
  char message[1024] = {0};
  char buffer[1024] = {0};
  
  
  listen(brserveur, 5); //file d'attente
  
  
  // accept la connexion
  int acclient = accept(brserveur, (struct sockaddr *)&client, &lgclient);
  
  
  //boucle principale de réception et d'envoie
  while(1)
    {
      // Réception d'un message
      printf("Serveur : Attente réception du message.\n");
      if (recv(acclient, buffer, sizeof(buffer), 0) == -1 )
	{
	  perror("Serveur : erreur du recv.\n");
	  close(brserveur);
	  return EXIT_FAILURE;
	}
      if(strncmp(buffer, "quitter", 7) == 0)
	{
	  printf("Serveur : Réception du message '%s'.\n", buffer);
	  send(acclient, "quitter", sizeof(message), 0);
	  close(brserveur);
	  return EXIT_SUCCESS;
	}
      else
	{
	  printf("Serveur : Réception du message '%s'.\n", buffer);
	  
	  
	  // Saisie et envoie d'un message
	  printf("Serveur : Veuillez saisir le message à envoyer : \n");
	  fgets(message, sizeof(message), stdin);
	  
	  if (send(acclient, message, sizeof(message), 0) == -1)
	    {
	      perror("send : error");
	      close(brserveur);
	      return EXIT_FAILURE;
	    }
	  printf("Serveur : Envois du message '%s'.\n", message);
	}
    }
}
