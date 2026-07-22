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
    scanf("%d",&u.telephone);

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
            printf("Le login doit contenir exactement 6 caracteres\n");
        }
        log = login_existe(u.login);
        if (log == 1) {
            printf("Ce login existe deja, veuillez en choisir un autre.\n");
        }
    }while(minuscule != 0 || strlen(u.login) != 6 || log != 0);

    strcpy(u.motPasse,"Library123");

    do{
    printf("Role (ADMIN/USER) : ");
    scanf("%s", u.role);

    if(strcmp(u.role, "ADMIN") != 0 && strcmp(u.role, "USER") != 0){
        printf("Le r�le doit �tre ADMIN ou USER uniquement.\n");
    }
    }while(strcmp(u.role, "ADMIN") != 0 && strcmp(u.role, "USER") != 0);


    strcpy(u.etat,"ACTIF");

    printf("Entre la date de creation : ");
    scanf("%s",u.dateCreation);

    strcpy(u.derniereConnexion,"Aucune");

    u.premierconnexion = 1;

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

        printf("\nTelephone : %d",u.telephone);

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

int connexion(){
    FILE *f;
    FILE *temp;
    User u;
    int id = 0;
     int trouve = 0;
    char login[15];
    char motPasse[100];




    f = fopen("DATABASE/USERS.dat", "rb");
    temp = fopen("DATABASE/TEMP.dat", "wb");

    if(f == NULL || temp == NULL)
    {
        printf("Impossible d'ouvrir le fichier.\n");
        fclose(f);
        fclose(temp);
        return 0;
    }

    printf("CONNEXION\n");

        printf("Login : ");
        scanf("%s", login);
        printf("Mot de passe : ");
        scanf("%s", motPasse);

    while(fread(&u, sizeof(User), 1, f))
    {
        if(strcmp(login, u.login) == 0 && strcmp(motPasse, u.motPasse) == 0){

            trouve = 1;
            id = u.id;
            if(u.premierconnexion == 1)
            {
                printf("\nC'est votre premiere connexion, veuillez changer votre mot de passe.\n");
                printf("Nouveau mot de passe : ");
                scanf("%s", u.motPasse);
                u.premierconnexion = 0;
            }

            printf("\nConnexion reussie.\n");
            printf("Bienvenue %s %s\n", u.prenom, u.nom);

            if(strcmp(u.role, "ADMIN") == 0)
            {
                printf("Vous etes connecte en tant qu'ADMIN.\n");
            }
            else
            {
                printf("Vous etes connecte en tant qu'UTILISATEUR.\n");
            }

        }

        fwrite(&u, sizeof(User), 1, temp);
    }

    fclose(f);
    fclose(temp);
    
    if (trouve){
        remove("DATABASE/USERS.dat");
        rename("DATABASE/TEMP.dat", "DATABASE/USERS.dat");
    }else{
        remove("DATABASE/TEMP.dat");
        printf("\nLogin ou mot de passe incorrect.\n");
    }
    return id;

}


void menuAdmin()
{
    int choix;

    do{
        
        printf("\nMenu Admin :\n");
        printf("1. Ajouter un utilisateur\n");
        printf("2. Afficher les utilisateurs\n");
        printf("3. Supprimer un utilisateur\n");
        printf("4. Modifier un utilisateur\n");
        printf("5. Deconnexion\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch(choix)
        {
            case 1:
                ajouterUtilisateur();
                break;
            case 2:
                afficherUtilisateurs();
                break;
            case 3:
                supprimer_utilisateur();
                break;
            case 4:
                modifier_utilisateur();
                break;
            case 5:
                printf("Deconnexion...\n");
                break;
            default:
                printf("Choix invalide.\n");
        }
    }while(choix != 5);
}

void menuUser(){
    int choix;
    do{
        printf("\nMenu Utilisateur :\n");
        printf("1. connexion\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch(choix)
        {
            case 1:
                connexion();
                break;
            default:
                printf("Choix invalide.\n");
        }
    }while(choix != 1);
}

void supprimer_utilisateur(){
    FILE *f;
    FILE *temp;
    User u;
    int id;
    int trouve = 0;
    int a;
    int confirmation;

    f=fopen("DATABASE/USERS.dat","rb");
    temp=fopen("DATABASE/TEMP.dat","wb");
    if(f== NULL){
        printf("erreur lors de l'ouverture du fichier U");
        return;
    }
    if(temp == NULL){
        printf("erreur lors de l'ouverture du fichier T");
        return;
    }
    do{
        printf("\nEntre l'id de l'utilisateur que vous voulez supprimer:");
        scanf("%d",&id);

        a= recherche_id(id);

        if(a==0){
            printf("\nl'id que vous avez entrer n'existe pas");
        }
    }while(a==0);

     while(fread(&u, sizeof(User), 1, f) == 1){
        if (u.id == id){

            printf("\nID : %d",u.id);
            printf("\nNom : %s",u.nom);
            printf("\nPrenom : %s",u.prenom);

            printf("\nVoulez-vous supprimer cette utilisateur?\n");
            printf("1. Oui\n");
            printf("2. Non\n");
            do{
                 printf("Votre choix :");
                scanf("%d", &confirmation);
            }while(confirmation != 1 && confirmation != 2);


            if (confirmation == 1)
            {
                trouve = 1;
                printf("\nUtilisateur supprime avec succes.\n");
                continue;
            }
        }

        fwrite(&u, sizeof(User), 1, temp);
    }
    fclose(f);
    fclose(temp);
    if (trouve){
        remove("DATABASE/USERS.dat");
        rename("DATABASE/TEMP.dat","DATABASE/USERS.dat");
    }else{
        remove("DATABASE/TEMP.dat");
    }

}

int recherche_id(int id){
    FILE *f;
    User u;


    f=fopen("DATABASE/USERS.dat","rb");

    if(f== NULL){
        printf("erreur lors de l'ouverture du fichier U");
        return 0;
    }
    while(fread(&u,sizeof(User),1,f)){
        if(u.id == id){
            fclose(f);
            return u.id;
          }
    }
    fclose(f);
    return 0;
}

void modifier_utilisateur(){
    FILE *f;
    FILE *temp;
    int id;
    User u;
    int trouve = 0;
    int confirmation;

    f=fopen("DATABASE/USERS.dat","rb");
    temp=fopen("DATABASE/TEMP.dat","wb");
    if(f== NULL){
        printf("erreur lors de l'ouverture du fichier U");
        return 0;
    }
    if(temp == NULL){
        printf("erreur lors de l'ouverture du fichier T");
        return 0;
    }
    printf("\nEntre l'id de l'utilisateur que vous voulez modifier:");
    scanf("%d",&id);

     while(fread(&u, sizeof(User), 1, f) == 1){
        if (u.id == id){

            printf("\nID : %d",u.id);
            printf("\nNom : %s",u.nom);
            printf("\nPrenom : %s",u.prenom);

            printf("\nVoulez-vous modifier cette utilisateur?\n");
            printf("1. Oui\n");
            printf("2. Non\n");
            do{
                 printf("Votre choix :");
                scanf("%d", &confirmation);
            }while(confirmation != 1 && confirmation != 2);
            if(confirmation == 1)
            {
                trouve = 1;
                printf("\nModification de l'utilisateur.\n");
                printf("Nom : ");
                scanf("%s",u.nom);

                printf("Prenom : ");
                scanf("%s",u.prenom);

                printf("Telephone : ");
                scanf("%d",&u.telephone);

                printf("Email : ");
                scanf("%s",u.email);

                printf("Role (ADMIN/USER) : ");
                scanf("%s", u.role);

                printf("Etat (ACTIF/INACTIF) : ");
                scanf("%s", u.etat);

            }
            fwrite(&u, sizeof(User), 1, temp);
        }
     }
     fclose(f);
     fclose(temp);

     if (trouve){
        remove("DATABASE/USERS.dat");
        rename("DATABASE/TEMP.dat", "DATABASE/USERS.dat");
        printf("\nUtilisateur modifie avec succes.\n");
    }else{
        remove("DATABASE/TEMP.dat");
    }   
}