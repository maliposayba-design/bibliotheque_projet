#ifndef EMPRUNT_H
#define EMPRUNT_H
#include "structure.h"
#include "books.h"
#include "USERS.h"



void menuEmprunts();
void AjouterEmprunt();
int compterEmprunts(int idUtilisateur);
void genererRecuEmprunt(EMPRUNT emprunt, User utilisateur, BOOK livre);
User trouverUtilisateurParId(int id);
BOOK trouverLivreParId(int id);
void mettreAJourStock(int idLivre, int quantite);

#endif
