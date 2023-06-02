#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <locale.h>


// Definition des structures

typedef struct {
    char ville[30];
    char pays[30];
    char rue[50];
}adresse_du_contact;
typedef struct Contact{
    char    nom[50];
    char prenom[50];
    long      GSM;
    char  email[50];
    adresse_du_contact  adresse;
}donees_contact;



// Fonction pour saisir un contact

donees_contact saisir_un_contact(){
   donees_contact a;
 
   printf("Donner le nom du contact : ");
   scanf("%s",a.nom);

   printf("Entrez le prenom du contact: ");
   scanf("%s",a.prenom);

   printf("Entrez le GSM : ");
   scanf("%ld",&a.GSM);

   printf("Entrez l'email' : ");
   scanf("%s",a.email);

   printf("  \n Saisie de l'adresse:\n\n");
   printf("veuillez entrer le nom de la rue:  ");
   scanf("%s",a.adresse.rue);

   printf("veuillez entrer la ville du contact:  ");
   scanf("%s",a.adresse.ville);

   printf("veuillez entrer le pays du contact:  ");
   scanf("%s",a.adresse.pays);

   printf("\n\nContact ajoute avec succes !");
return a;
}






// Menu de de l'utulisateur ou invite

int Menu_invite(){
int choix;
printf("\n*************************************MENU******************************************\n");
printf("         1-Rechercher un contact \n");
printf("         2-Afficher tous les contacts \n");

printf("         3-Quitter \n");
printf("\n\n   Enrez votre choix : ");
scanf("%d",&choix);
return choix;
}

// Menu de l'administrateur

int Menu_Admin(){
int choix;
printf("\n*************************************MENU******************************************\n");
printf("         1-Ajouter un contact \n");
printf("         2-Rechercher un contact \n");
printf("         3-Supprimer un contact \n");
printf("         4-Modifier un contact \n");
printf("         5-Afficher tous les contacts \n");
printf("         6-Quitter \n");
printf("\n\n   Entrez votre choix : ");
scanf("%d",&choix);
return choix;
}


// Debut de la fonction main 
int main()
{
//setlocale(LC_ALL, "fr_FR.UTF-8");

    // Definitio du socket
    int Socket;
    Socket=socket(AF_INET,SOCK_STREAM,0);
    // definir une adresse pour socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8006);
    server_address.sin_addr.s_addr = INADDR_ANY;
 
 /////////////////////////////////////////////////////////////////////////////////////////////////////////  
//     if (inet_pton(AF_INET, "100.82.200.22", &server_address.sin_addr) <= 0)
//  {
//    error("ADDRESS NOT SUPPORTED");
//  }
   
    // conexion au socket
    int connection_status = connect(Socket,(struct sockaddr *) &server_address, sizeof(server_address));
    // tester la connexion
    if (connection_status == -1){
        printf("Erreur ! Echec de connexion au socket \n\n");
    }

    // reception des donnnees
    char server_response[256]; // pour stocker le 1er message reçu du serevur
    recv(Socket,&server_response,sizeof(server_response),0);
    // printout les donn es
    printf("le serveur a envoyer : %s \n",server_response);



    char login[30];
    char mdp[30];
    int n=0;
    int resultat=0;
    
          printf("\n\n************************************* BIENVENUE ******************************************\n\n");
         printf("RQ : vous avez le droit d'enter comme -Admin ou -Invite seulement \n");
         printf("S'il vous plait entrer votre login:\n ");
         scanf("%s",login);
         send(Socket ,login , strlen(login), 0 );
    do{
        printf("s'il vous plait entrer votre password (vouz avez %d essai!) : ",3-n);
        scanf("%s",mdp);
        send(Socket , mdp , strlen(mdp), 0 );
        recv( Socket , &resultat, sizeof(resultat),0);
        n++;
    }while(resultat==0 && n<3 );
    int choix;
    int reponse;

    donees_contact c;
   
     if (resultat==1) {
               printf("\n\n-----------------Vous etes Administrateur -----------------\n\n");
               do {

                      choix=Menu_Admin();

                      send(Socket , &choix ,sizeof(choix), 0 );
                      switch (choix){
                        case 1 : {
						
								donees_contact a;	
								a=saisir_un_contact();
                                 send(Socket , &a , sizeof(a) , 0 );
                               
                                  
                                 break;
                             }

                           

                        case 2: {
                          char mssg[100];  long gsm;
                                  //char *mssg=(char*)malloc(150*sizeof(char)); //str[200]; // declare a fixed-size array
                                  printf("entrez le gsm : ");
                                  scanf("%ld", &gsm);
                                  send(Socket, &gsm, sizeof(gsm), 0);
                                  recv(Socket, mssg, sizeof(mssg), 0); // receive message into the array
                                 
                                 
                                  printf("\n%s\n", mssg);
                                  //free(mssg);
                                  fflush(stdin);
                                  break;
                              }

                       

                        case 3 : {
						
									 long gsm;
									 printf("entrez le gsm du contact a supprimer: ");
                                   scanf("%ld",&gsm);
                                   send(Socket , &gsm, sizeof(gsm) , 0 );
                                   recv(Socket , &reponse , sizeof(reponse) , 0 );
                                    if(reponse){
                                      printf("La suppression a ete faite avec succes \n ");
                                     
                                    }
                                    if(!reponse) {printf("le contact n'existe pas \n ");}
                                     
                                    break;
                                }

                        case 4 :  {
					
									donees_contact a;   long gsm;
									printf("entrez le gsm  de contact a modifier: ");
                                   scanf("%ld",&gsm);
                                   send(Socket , &gsm, sizeof(gsm) , 0 );
                                   printf(" Saise les nouvelles donnes du contatcs  \n");
                                   a = saisir_un_contact();
                                   send(Socket , &a, sizeof(a) , 0 );
                                   recv(Socket , &reponse , sizeof(reponse) , 0 );
                                    if(reponse){
                                      printf(" La modificatin a ete faite avec succes \n ");
                                    break;
                                    }
                                    if (!reponse){ printf("Erreur, ce contacct n'existe pas \n ");}
                                   
                                       
                                    break;
                                }
                     
                        case 5 :{
                        	
                        	donees_contact a;
                        
                        	 	printf("\n\n La liste des contacts : \n"); 
                        	 
                        	recv(Socket , &a , sizeof(a) , 0 );
                        	printf("\n %s %s %ld %s %s %s %s \n",a.nom,a.prenom,&a.GSM,a.email,a.adresse.rue,a.adresse.ville,a.adresse.pays);
                        	
							 
                        	
	
                                break;
                   }
                           
                           case 6 : printf("\nAurevoir ! \n");
                                 exit;

                           break;
                        default :
                                 printf("votre choix %d  est invalide\n\n ",choix);
                                 break;
                      }
               } while(choix !=6);}
               
                else if (resultat==2){
               printf("--------------VOUS ETES UTULISATEUR ----------------------\n");
              do {
                      choix=Menu_invite();
                      if (choix==1) { choix=2;}
                      else if(choix==2) { choix=5;}
                      else if(choix==3) { choix=6;}
                      send(Socket , &choix ,sizeof(choix), 0 );
                     
                      switch (choix){
                     
                     
                     
                        case 5 :  
							donees_contact a;
                        
                        	 	printf("\n\n La liste des contacts : \n"); 
                        	 
                        	recv(Socket , &a , sizeof(a) , 0 );
                        	printf("\n %s %s %ld %s %s %s %s \n",a.nom,a.prenom,&a.GSM,a.email,a.adresse.rue,a.adresse.ville,a.adresse.pays);
                        	
						break;
                       
                        case 2: {   char mssg[100]; long gsm;
                                  //char *mssg=(char*)malloc(150*sizeof(char)); //str[200]; // declare a fixed-size array
                                  printf("entrez le gsm : ");
                                  scanf("%ld", &gsm);
                                  send(Socket, &gsm, sizeof(gsm), 0);
                                  recv(Socket, mssg, sizeof(mssg), 0); // receive message into the array
                                 
                                 
                                  printf("\n%s\n", mssg);
                                  //free(mssg);
                                  fflush(stdin);
                                    break;
                        }
                       
                       
                        case 6 : printf("\nAurevoir ! \n");
                                 exit;

                           break;
                        default :
                                 printf("votre choix %d  est invalide\n\n ",choix);
                                 break;
                       
                      }
               } while(choix !=6);
     }
     else{
        printf(" mode non specifie \n ");
        exit(1);
     }
               close(Socket);
               return 0;
     }

