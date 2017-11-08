#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char **argv)
{
  //création du buffer
  int brserveur = socket(PF_INET, SOCK_DGRAM, 0);
  if(brserveur == -1)
    {
      perror("socket() : error\n");
      return EXIT_FAILURE;
    }
  
  
  //création et configuration de la socket
  struct  sockaddr_in serveur;
  serveur.sin_family = AF_INET;
  serveur.sin_addr.s_addr = INADDR_ANY;
  serveur.sin_port = htons((short)31400);
  
  
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
  char buffer[1024];
  char message[1024];
  
  
  //boucle principale de réception et d'envoie
  while(1)
    {
      // Réception d'un message
      printf("Serveur : Attente réception du message.\n");
      int retourRecv = recvfrom(brserveur, buffer, 1024, 0, (struct sockaddr *) &client, &lgclient);
      if(strncmp(buffer, "quitter", 7) == 0)
	{
	  printf("Serveur : Réception du message '%s'.\n", buffer);
	  int retourSend = sendto(brserveur, "quitter", sizeof("quitter"), 0, (struct sockaddr *)&client, sizeof(client));
	  close(brserveur);
	  return EXIT_SUCCESS;
	}
      else
	{
	  printf("Serveur : Réception du message '%s'.\n", buffer);
	  
	  
	  // Saisie et envoie d'un message
	  printf("Serveur : Veuillez saisir le message à envoyer : \n");
	  fgets(message, 1024, stdin); //0 = stdin 0= entree 1= sortie 2= erreur
	  int retourSend = sendto(brserveur, message, sizeof(message), 0, (struct sockaddr *) &client, sizeof(client));
	  printf("Serveur : Envois du message '%s'.\n", message);
	}
    }
}
