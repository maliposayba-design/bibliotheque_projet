#ifndef AUTHORS_H
#define AUTHORS_H
#include "books.h"
typedef struct
{
    int id;
    char nom_auteur[25];
    char nationalite[20];
    char date_naissance[11];
    char biographie[250];
    int nb_livres;

} AUTHORS;
void menu_auteurs();
int generer_id_auteur();
void saisir_auteur(AUTHORS *A);
void enregistrer_auteur(AUTHORS A);
void ajout_auteur();
void afficher_auteur(AUTHORS A);
void afficher_liste_auteurs();
void modifier_auteur();
void supprimer_auteur();
void rechercher_auteur();
#endif
