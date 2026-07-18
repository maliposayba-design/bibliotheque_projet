#ifndef RESERVATIONS_H
#define RESERVATIONS_H
#include "USERS.h"
#include "books.h"

typedef struct{
    int id;
    int idUtilisateur;
    int idLivre;
    char dateReservation[20];
    char etat[20];
} RESERVATION;

char* getdatetime();
char* getdate();
void menu_reservations();
int generer_id_reservation();
void saisir_reservation(RESERVATION *R);
void enregistrer_reservation(RESERVATION R);
void ajouter_reservation();
void afficher_reservation(RESERVATION R);
void afficher_liste_reservations();
void afficher_mes_reservations();
void modifier_reservation();
void annuler_reservation();
void supprimer_reservation();
int reservation_existe(int idUtilisateur, int idLivre);
void verifier_reservations_apres_retour(int idLivre);
RESERVATION* get_premiere_reservation_en_attente(int idLivre);

#endif
