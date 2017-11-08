#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <netdb.h>

int main(int argc, char **argv)
{
  int i;
  int maxFD=0; //le plus grand des files descripteur 
  int tab_socket[5]; //tableau des files descripteur
  int sock_ouverte=0;
  char buffer[1024] = {0};
  
  
  //création du buffer
  int brserveur = socket(AF_INET, SOCK_STREAM, 0);
  if (brserveur == -1)
    {
      perror("socket() : error\n");
      return EXIT_FAILURE;
    }
  
  maxFD=brserveur;
  
  
  //création et configuration de la socket
  struct  sockaddr_in serveur;
  serveur.sin_family = AF_INET;
  serveur.sin_addr.s_addr = inet_addr("127.0.01");
  serveur.sin_port = htons((short)31469);
  
  
  //le bind = liaison entre le buffer et le socket
  int res = bind(brserveur,(struct sockaddr*)&serveur, sizeof(serveur));
  if(res == -1)
    {
      perror("bind() : error\n");
      close(brserveur);
      return EXIT_FAILURE;
    }
  
  
  //structure du client
  struct sockaddr_in client; 
  socklen_t lgclient = sizeof(client);
  
  
  fd_set surveiller; //tableau de boolean
  listen(brserveur, 5); //file d'attente
  
  
  //boucle principale de receptio, et d'envoie
  while(1)
    {
      FD_ZERO (&surveiller); //remet le tableau a zero
      FD_SET (brserveur, &surveiller); //on rajoute la boite public au tableau
      
      
      //on rajoute toute les boites aux tableau
      for(i=0; i<sock_ouverte; i++)
	FD_SET(tab_socket[i], &surveiller);
      
      
      struct timeval timeout;
      printf("Serveur : attend 3sec avant de cheker les boites réseaux.\n ");
      timeout.tv_sec = 3;
      printf("Serveur : attend.\n");
      if(select(maxFD+1, &surveiller, NULL, NULL, &timeout) == -1)
	{
	  perror("erreur du select() \n");
	  close(brserveur);
	  return -1;
	}
      
      
      //si la boite public a un truc
      if(FD_ISSET(brserveur, &surveiller))
	{
	  tab_socket[sock_ouverte++]=accept(brserveur, (struct sockaddr *)&client, &lgclient);
	  printf("Serveur : nouvelle socket crée.\n");

	  if(tab_socket[sock_ouverte-1] > maxFD)
	    maxFD=tab_socket[sock_ouverte-1];
	}
      
      
      //check les autre boite
      for(i=0; i<sock_ouverte; i++)
	{
	  if(FD_ISSET(tab_socket[i], &surveiller))
	    {
	      //reception du message
	      printf("Serveur : Attente réception d'un message.\n");
	      if (recv(tab_socket[i], buffer, sizeof(buffer), 0) == -1 )
		{
		  perror("recv : error");
		  close(tab_socket[i]);
		  return EXIT_FAILURE;
		}
	      if(strncmp(buffer, "quitter", 7) == 0)
		{
		  printf("Serveur : Réception du message '%s'.\n", buffer);
		  send(tab_socket[i], "quitter", sizeof("quitter"), 0);
		  close(tab_socket[i]);
		  sock_ouverte--;
		}
	      else
		{
		  printf("Serveur : Réception du message '%s'.\n", buffer);
		  
		  
		  //reponse du serveur
		  if (send(tab_socket[i], "compris", sizeof("compris"), 0) == -1)
		    {
		      perror("send : error");
		      close(tab_socket[i]);
		      return EXIT_FAILURE;
		    }
		  printf("Serveur : Envois du message compris.\n");
		}
	    }
	}
    }
  close(brserveur);
  return 0;
}

