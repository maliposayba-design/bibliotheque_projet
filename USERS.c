#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "USERS.h"


void ajouterUtilisateur(){
    FILE *f;

    User u;

    f = fopen("DATABASE/USERS.dat","ab");

    if(f == NULL){
        printf("Erreur ouverture fichier.");
        return;
    }

    u.id = id_utilisateur();

    printf("Nom : ");
    scanf("%s",u.nom);

    printf("Prenom : ");
    scanf("%s",u.prenom);

    printf("Telephone : ");
    scanf("%s",u.telephone);

    printf("Email : ");
    scanf("%s",u.email);

    int minuscule, log;
    do{

        minuscule=0;
        printf("Login : ");
        scanf("%s",u.login);
        for (int i = 0; u.login[i] != '\0'; i++) {
            if(u.login[i] < 'A' || u.login[i] > 'Z') {
                minuscule = 1;
                break;
            }
        }
        if(minuscule !=0){
            printf("Le login doit contenir uniquement des lettre en majuscule\n");
        }

        if (strlen(u.login) != 6) {
            printf("Le login doit contenir exactement 6 caractères\n");
        }
        log = login_existe(u.login);
        if (log == 1) {
            printf("Ce login existe deja, veuillez en choisir un autre.\n");
        }
    }while(minuscule != 0 || strlen(u.login) != 6 || log != 0);

    strcpy(u.motPasse,"Library123");

    printf("Role (ADMIN/USER) : ");
    scanf("%s",u.role);

    strcpy(u.etat,"ACTIF");

    printf("Entre la date de creation : ");
    scanf("%s",u.dateCreation);

    strcpy(u.derniereConnexion,"Aucune");

    fwrite(&u,sizeof(User),1,f);

    fclose(f);

    printf("\nUtilisateur ajoute avec succes.");
}

int id_utilisateur(){
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
        printf("erreur");
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

int login_existe(char login[]){
    FILE *f;

    User tester;

    f = fopen("DATABASE/USERS.dat","rb");

    if(f == NULL){
        printf("erreur");
        return 0;
    }
    while (fread(&tester, sizeof(User), 1, f)) {
        if (strcmp(login, tester.login) == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;

}

