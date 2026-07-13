#ifndef CATEGORIES_H
#define CATEGORIES_H
typedef struct
{
    int id_categorie;
    char date_creation[11];
    char description[100];
    char libelle[50];
} CATEGORY;
void menu_categories();
int generer_id_categorie();
void saisir_categorie(CATEGORY *C);
void enregistrer_categorie(CATEGORY C);
void ajout_categorie();
void afficher_categorie(CATEGORY C);
void afficher_liste_categorie();
void modifier_categorie();
void supprimer_categorie();
void rechercher_categorie();
#endif
