#ifndef RETOURS_H
#define RETOURS_H
#include "Emprunt.h"
#include "Penalites.h"

typedef struct
{
  int idRetour;
  int idEmprunt;
  char dateR[20];
  int nbr_jourR;
  int mont_penalite;
}RETOUR;

void menuRetours();
void retournerLivre();
int estDateValide(char date[]);
int calculerJoursRetard(char dateRetourPrevue[], char dateRetourReelle[]);
void enregistrerPenalite(EMPRUNT emprunt, int idUtilisateur, int joursRetard, char dateRetour[20]);
void genererRecuRetour(RETOUR retour, User utilisateur, BOOK livre);


#endif