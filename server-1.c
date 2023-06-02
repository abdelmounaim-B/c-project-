#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <locale.h>

// Definition des structures
typedef struct { 
    char ville[30];
    char pays[30];
    char rue[30];
}adresse_du_contact;
typedef struct{
    
    long        GSM;
    char  email[30];
    char    nom[30];
    char prenom[30];
    adresse_du_contact  adresse;
}donees_contact;



// Fonction pour ajouter un contact

void ajouter_un_contact(char *nom_fich,donees_contact a){
    FILE *f=fopen(nom_fich,"a");
    if(f!=NULL){
        fprintf(f,"%s %s %ld %s %s %s %s\n",a.nom,a.prenom,a.GSM,a.email,a.adresse.rue,a.adresse.ville,a.adresse.pays);
        printf("\n------------------------ un Contact a ete ajoute avec succes !--------------------------------\n\n");
        fclose(f);
    }
    else
        printf("\n--------- Echec, l'ouverture du fichier n'a pas abouti !----------\n\n");

}


// Fonction pour chercher un contact

int  rechercher_un_contact(char *nom_fichier,long gsm) {
    FILE *f=fopen(nom_fichier,"r");

    donees_contact c;
    int rep=0;
       if(f!=NULL){
           while(!feof(f)){

              if (fscanf(f,"%s %s %ld %s %s %s %s",c.nom,c.prenom,&c.GSM,c.email,c.adresse.rue,c.adresse.ville,c.adresse.pays)==7){
                 if (gsm==c.GSM) {
                       rep=1;
                        printf("\n--------Ce contact a ete trouve avec succes !-----------------------\n");
               break;                  }
              }
            }
    fclose(f);

    if (rep == 0) {
        printf("\n-----------Ce contact n'a pas ete trouve !---------\n");
    }

}

else {printf("L'ouverture du fichier n'a pas abouti ! \n"); }

  return rep;
 
}


//Fonction pour supprimer un contact

int supprimer_un_contact(char *nom_f,long gsm) {
    char nom_fichier[]="Lescontactes.txt";
  donees_contact c;
    int rep=0;
    FILE *f_in=fopen(nom_f,"r");
    FILE *f_out=fopen(nom_fichier,"w");
    while (fscanf(f_in,"%s %s %ld %s %s %s %s",c.nom,c.prenom,&c.GSM,c.email,c.adresse.rue,c.adresse.ville,c.adresse.pays)!=-1) {
 
        if (gsm == c.GSM) {
            rep=1;
              printf("\n------La Suppression a ete faite avec succes ---------\n");
         
        }
        else {
            fprintf(f_out,"%s %s %ld %s %s %s %s\n",c.nom,c.prenom,c.GSM,c.email,c.adresse.rue,c.adresse.ville,c.adresse.pays);
        }
    }
    fclose(f_out);
    fclose(f_in);
    remove(nom_f);
    rename(nom_fichier,nom_f);
    return rep;
}

//Fonction pour modifier un contact


int modifier_un_contact(char *nom_f,long gsm, donees_contact d) {
   donees_contact a;
    int rep=0;
    char nom_fichier[]="Lescontactes.txt";
    FILE *f_in=fopen(nom_f,"r");
    FILE *f_out=fopen(nom_fichier,"w");
    while(fscanf(f_in,"%s %s %ld %s %s %s %s",a.nom,a.prenom,&a.GSM,a.email,a.adresse.rue,a.adresse.ville,a.adresse.pays)!=-1) {

        if (gsm == a.GSM) {
            fprintf(f_out,"%s %s %ld %s %s %s %s\n",d.nom,d.prenom,d.GSM,d.email,d.adresse.rue,d.adresse.ville,d.adresse.pays);
            rep=1;
       printf("\n--------------- Le contact a ete modifie avec  succes !!!!------------\n");
           
        }
        else {
            fprintf(f_out,"%s %s %ld %s %s %s %s\n",a.nom,a.prenom,a.GSM,a.email,a.adresse.rue,a.adresse.ville,a.adresse.pays);

        }
    }
  fclose(f_out);
    fclose(f_in);
 

    remove(nom_f);
    rename(nom_fichier,nom_f);
    return rep;

}

// Debut de la fonction Main

int main()
{
setlocale(LC_ALL, "fr_FR.UTF-8");

    char server_message[256] = "vous avez  ete  connecte  au serveur avec succes";
    // Definition du socket
    int socket_serveur;
    socket_serveur=socket(AF_INET,SOCK_STREAM,0);
    
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8006);
    server_address.sin_addr.s_addr = INADDR_ANY;

  
    bind(socket_serveur,(struct sockaddr *) &server_address, sizeof(server_address));
    listen(socket_serveur,8);

     int socket_client;
     socket_client = accept(socket_serveur, NULL, NULL);


    
    send(socket_client,server_message,sizeof(server_message),0);

   
    
	int  resultat=0;
    char mdp[30];
    char nom[30];
    
    char login [30];
    char password[30];
    char type[30];
    int essaie=0;


     FILE *f=fopen("comptes.txt","r");
    if(f==NULL){
       printf(" Echec,  l''ouverture du fichier comptes.txt n'a pas aboutit ! \n");
       exit(1);
    }
    else{
    printf("verification d'identite : \n ");
   
    recv( socket_client ,nom, sizeof(nom),0);
    printf("%s \n ",nom);
    while(!feof(f)){
    fscanf(f,"%s %s %s",login,password,type);
    if(strcmp(nom,login)==0){
         while( essaie <=3){
            bzero(mdp,30);
            recv( socket_client , mdp, sizeof(mdp),0);
            printf("%s \n ",mdp);
               if(strcmp(mdp,password)==0) {
                      if(strcmp(type,"admin")==0){
                          resultat=1;
                          send(socket_client , &resultat , sizeof(resultat) , 0 );
                          break;

                      }
                     else if(strcmp(type,"invite")==0){
                           resultat=2;
                           send(socket_client , &resultat , sizeof(resultat) , 0 );
                           break;
                     }
               }
         resultat=0;
         send(socket_client , &resultat , sizeof(resultat) , 0 );
         essaie++;
       }

    }
    else{
    resultat=-1;
    }
       }
       send(socket_client , &resultat , sizeof(resultat) , 0 );
    }


 

int choix=0;

int reponse=0;

  do {
        recv(socket_client , &choix , sizeof(choix),0);
                      switch (choix){
                        case 1 : {
						
                        	donees_contact a;
                                 recv( socket_client , &a , sizeof(a),0);
                                 ajouter_un_contact("contacts.txt",a);
                               

                                  break;
                              }

                         case 2: {
                                long d, gsm;
                                recv(socket_client, &d, sizeof(d), 0);
                                gsm = *(&d); // dereference the pointer before assigning to gsm
                                reponse = rechercher_un_contact("contacts.txt", gsm);
                               // char *mssg = (char*)malloc(150*sizeof(char));
                                if (reponse) {
                               
                                    //char mssg[100] = "------Contact trouvable!!!!-------";
                                    char mssg[100]="*** Contact Trouve ! ***" ;
                                    send(socket_client, mssg, sizeof(mssg), 0);
                                } else if (!reponse) {
                                //strcpy(mssg, "*** Contact non Trouve ! ***");
                                   char mssg[100]="*** Contact non Trouve ! ***" ;
                                    send(socket_client, mssg, sizeof(mssg), 0);
                                }
                                //free(mssg);
                                fflush(stdin);
                                break;
                            }


                         case 3 : {
							long gsm;
                              recv(socket_client , &gsm, sizeof(gsm), 0);
                              reponse=supprimer_un_contact("contacts.txt",gsm);
                              send(socket_client , &reponse , sizeof(reponse), 0);
                              break;
                          }

                          case 4 :  {
						
                          		donees_contact a; long gsm;
                              recv(socket_client , &gsm, sizeof(gsm), 0);
                              recv(socket_client , &a , sizeof(a), 0);
                              reponse=modifier_un_contact("contacts.txt",gsm,a);
                              send(socket_client , &reponse , sizeof(reponse), 0);
                              break;
							}
                             

                        case 5 : {

						
						FILE *f;
				
						donees_contact a;
						
					    
					   
					    printf("\n\nLe fichier contient %d lignes\n", nb_lignes);
					    
					    // Ouverture du fichier 
					    f = fopen("contacts.txt", "r");
					    if (f == NULL) {
					        printf("Erreur d'ouverture du fichier !");
					        return -1;
					    }
					    while(!feof(f))  {
					    
						if(fscanf(f,"%s %s %ld %s %s %s %s",a.nom,a.prenom,&a.GSM,a.email,a.adresse.rue,a.adresse.ville,a.adresse.pays)!=-1)  {
						  send(socket_client , &a , sizeof(a), 0);	
							
						}
						
						
						}
						fclose(f);
						
						 break;
					}
                        case 6 :     close(socket_serveur); break;
                       
                      }
        } while(choix !=6);

    close(socket_serveur);
    return 0;
}

