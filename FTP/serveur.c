#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h> // Pour inet_ntoa
#include <sys/stat.h>

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
  struct stat status; //données du fichier
  int i = 0;
  char c;
  
  listen(brserveur, 5); //file d'attente
  
  
  // accept la connexion
  int acclient = accept(brserveur, (struct sockaddr *)&client, &lgclient);
  
  
  //boucle principale de réception et d'envoie
  while(1)
    {
      //ouvre le fichier a envoyer
      FILE* fd;
      
      
      //ouverture du fichier envoyer si il existe
      if((fd = fopen("toto.jpg", "r+")) == NULL)
	{ 
	  perror("Serveur : erreur de l'ouverture du fichier.\n");
	  close(brserveur);
	  return EXIT_FAILURE;
	}
      
      
      //recupère la taille du fichier
      lstat("toto.jpg", &status);
      off_t size = status.st_size; //off_t est un long pour la taille de fichier en gros
      
      
      //envoie de la taiile du fichier
      if(send(acclient, &size, sizeof(off_t), 0) == -1)
	{
	  perror("Serveur : erreur envoie taille fichier.\n");
	  close(brserveur);
	  return EXIT_FAILURE;
	}
      
      
      //envoie le contenue du fichier
      while(i<(int)size) //tant que l'on a pas envoyer tout le fichier
	{
	  c = fgetc(fd);
	  if(send(acclient, &c, sizeof(char), 0) == -1)
	    {
	      perror("Serveur : erreur envoie du fichier.\n");
	      close(brserveur);
	      return EXIT_FAILURE;
	    }
	  i++;
	}
      close(brserveur);
      return EXIT_SUCCESS;
    }
  close(brserveur);
  return EXIT_SUCCESS;
}
