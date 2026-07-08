#ifndef USERS


typedef struct{
    int id;
    char nom[50];
    char prenom[50];
    char telephone[20];
    char email[100];
    char login[7];
    char motPasse[100];
    char role[10];
    char etat[10];
    char dateCreation[20];
    char derniereConnexion[20];
}User;

int genererIdUtilisateur();
void ajouterUtilisateur();
void afficherUtilisateurs();
#endif
