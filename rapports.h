#ifndef RAPPORTS_H
#define RAPPORTS_H
#include "USERS.h"
#include "books.h"

typedef struct {
    int id;
    char type[30];
    char dateCreation[20];
    char chemin[100];
} RAPPORT;
void menu_rapports();
void ecrire_historique(const char* action, const char* detail);
void afficher_historique();
void generer_recu_emprunt(int idEmprunt);
void generer_recu_retour(int idRetour);
void generer_rapport_journalier();
void afficher_liste_rapports();

#endif
