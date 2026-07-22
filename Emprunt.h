#ifndef EMPRUNT_H
#define EMPRUNT_H

#include "books.h"
#include "USERS.h"

typedef struct
{
  int idEmprunt;
  char num_emprunt[21];
  int idUtilisateur;
  int idLivre;
  char date_emprunt[25];
  char date_retour[25];
  char etat[20];
}EMPRUNT;

void menuEmprunts();
void AjouterEmprunt();
int compterEmprunts(int idUtilisateur);
void genererRecuEmprunt(EMPRUNT emprunt, User utilisateur, BOOK livre);
User trouverUtilisateurParId(int id);
BOOK trouverLivreParId(int id);
void mettreAJourStock(int idLivre, int quantite);

#endif