#ifndef PENALITES_H
#define PENALITES_H

typedef struct
{
 int id_pen;
 int idUtilisateur;
 int idEmprunt;
 int nbr_jourR;
 int mont_pen;
 char date[25];
 char statut[15];
}PENALITE;

void menuPenalites();
void afficherPenalites();
void payerPenalite();

#endif