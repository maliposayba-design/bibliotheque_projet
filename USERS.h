#ifndef USERS
#define USERS

typedef struct{
    int id;
    char nom[50];
    char prenom[50];
    int telephone;
    char email[100];
    char login[7];
    char motPasse[100];
    char role[10];
    char etat[10];
    char dateCreation[20];
    char derniereConnexion[20];
    int premierconnexion;
}User;

int id_utilisateur();
void ajouterUtilisateur();
void afficherUtilisateurs();
int login_existe(char login[]);
void supprimer_utilisateur();
int recherche_id(int);
int connexion();
void menuAdmin();
void menuUser();
#endif
