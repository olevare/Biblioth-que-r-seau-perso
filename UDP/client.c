#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char **argv)
{
  //création du buffer
  int brclient = socket(PF_INET, SOCK_DGRAM, 0);
  if(brclient == -1)
    {
      perror("socket() : error\n");
      return EXIT_FAILURE;
    }
  else
    printf("Client : La socket est ouverte en mode UDP.\n");
  
  
  //création et configuration de la socket du client
  struct  sockaddr_in client;
  client.sin_family = AF_INET;
  client.sin_addr.s_addr = inet_addr("127.0.01"); // ou INADDR_ANY; ou htonl(INADDR_ANY);
  client.sin_port = htons((short)31450);
  
  
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
  struct sockaddr_in serveur;
  serveur.sin_family = AF_INET;
  serveur.sin_addr.s_addr = inet_addr("127.0.0.1");
  serveur.sin_port = htons((short)31400);
  
  printf("Client : L'initialisation de la socket distante est terminé.\n");
  
  
  //initialisation tampon envoie en reception
  char buffer[1024];
  char message[1024];
  
  
  //boucle principale de réception et d'envoie
  while(1)
    {
      // Saisie et envoie d'un message
      printf("Client : Veuillez saisir le message à envoyer : \n");
      fgets(message, 1024, stdin);
      int retourSend = sendto(brclient, message, sizeof(message), 0, (struct sockaddr *)&serveur, sizeof(serveur));
      printf("Client : Envois du message '%s'.\n", message);
      
      
      // Réception d'un message
      printf("Client : Attente réception du message.\n");
      int retourRecv = recvfrom(brclient, buffer, 1024, 0, NULL, NULL);
      printf("Client : Réception du message '%s'.\n", buffer);
      if(strncmp(buffer, "quitter", 7) == 0)
	{
	  int retourSend = sendto(brclient, message, sizeof(message), 0, (struct sockaddr *)&serveur, sizeof(serveur));
	  printf("Client : Fermeture de la socket locale.\n");
	  close(brclient);
	  return EXIT_SUCCESS;;
	}
    }
}
