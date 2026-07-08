#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "USERS.h"


void ajouterUtilisateur(){
    FILE *f;

    User u;

    f = fopen("DATABASE/USERS.dat","ab");

    if(f == NULL){
        printf("Erreur ouverture fichier.");
        return;
    }

    u.id = genererIdUtilisateur();

    printf("Nom : ");
    scanf("%s",u.nom);

    printf("Prenom : ");
    scanf("%s",u.prenom);

    printf("Telephone : ");
    scanf("%s",u.telephone);

    printf("Email : ");
    scanf("%s",u.email);

    printf("Login : ");
    scanf("%s",u.login);

    strcpy(u.motPasse,"Library123");

    printf("Role (ADMIN/USER) : ");
    scanf("%s",u.role);

    strcpy(u.etat,"ACTIF");

    strcpy(u.dateCreation,"07/07/2026");

    strcpy(u.derniereConnexion,"Aucune");

    fwrite(&u,sizeof(User),1,f);

    fclose(f);

    printf("\nUtilisateur ajoute avec succes.");
}

int genererIdUtilisateur(){
    FILE *f;
    User u;
    int id = 1;

    f = fopen("DATABASE/USERS.dat","rb");

    if(f == NULL){
        return 1;
    }


    while(fread(&u,sizeof(User),1,f)){
        id = u.id + 1;
    }

    fclose(f);

    return id;
}

void afficherUtilisateurs(){
    FILE *f;

    User u;

    f = fopen("DATABASE/USERS.dat","rb");

    if(f == NULL){
        printf("Aucun utilisateur.");
        return;
    }

    while(fread(&u,sizeof(User),1,f)){
        printf("\n----------------------------");

        printf("\nID : %d",u.id);

        printf("\nNom : %s",u.nom);

        printf("\nPrenom : %s",u.prenom);

        printf("\nTelephone : %s",u.telephone);

        printf("\nEmail : %s",u.email);

        printf("\nLogin : %s",u.login);

        printf("\nRole : %s",u.role);

        printf("\nEtat : %s",u.etat);

        printf("\n");
    }

    fclose(f);
}
