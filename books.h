#ifndef BOOKS_H
#define BOOKS_H
#include "authors.h"
#include "categories.h"
typedef struct
{
    int id;
    char isbn[20];
    char titre[100];
    int id_auteur;
    int id_categorie;
    char editeur[50];
    int annee_publication;
    char langue[30];
    int nb_pages;
    int nb_total_exemplaires;
    int nb_disponible;
    char emplacement[50];
    char resume[300];
    char date_ajout[11];
} BOOK;
void menu_livres();
int generer_id_livre();
int isbn_existe(char isbn[]);
int auteur_existe(int id);
int categorie_existe(int id);
void saisir_livre(BOOK *L);
void enregistrer_livre(BOOK L);
void ajout_livre();
void afficher_livre(BOOK L);
void afficher_liste_livres();
void modifier_livre();
void supprimer_livre();
int auteur_a_des_livres(int IdAuteur);
void rechercher_livre();
#endif
