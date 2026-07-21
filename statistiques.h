#ifndef STATISTIQUES_H
#define STATISTIQUES_H

#include "USERS.h"
#include "books.h"

typedef struct {
    int id;
    char type[30];
    int valeur;
    char dateCreation[20];
} STATISTIQUE;

void menu_statistiques();
void afficher_statistiques_generales();
void afficher_statistiques_journalieres();
int compter_utilisateurs();
int compter_livres();
int compter_emprunts_en_cours();
int compter_livres_disponibles();
int compter_reservations_totales();
int compter_penalites_impayees();
int compter_emprunts_du_jour();
int compter_retours_du_jour();
int compter_reservations_du_jour();
int compter_nouveaux_utilisateurs_du_jour();
int compter_penalites_du_jour();
float montant_penalites_du_jour();
char* get_livre_le_plus_emprunte();
char* get_utilisateur_le_plus_actif();
float get_montant_total_penalites();

#endif
