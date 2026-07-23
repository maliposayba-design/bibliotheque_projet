#ifndef RETOURS_H
#define RETOURS_H
#include "structure.h"
#include "Emprunt.h"
#include "Penalites.h"



void menuRetours();
void retournerLivre();
int estDateValide(char date[]);
int calculerJoursRetard(char dateRetourPrevue[], char dateRetourReelle[]);
void enregistrerPenalite(EMPRUNT emprunt, int idUtilisateur, int joursRetard, char dateRetour[20]);
void genererRecuRetour(RETOUR retour, User utilisateur, BOOK livre);


#endif
