#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct{
  int idEmprunt;
  char num_emprunt[21];
  int idUtilisateur;
  int idLivre;
  char date_emprunt[25];
  char date_retour[25];
  char etat[20];
}EMPRUNT;

typedef struct
{
  int idRetour;
  int idEmprunt;
  char dateR[20];
  int nbr_jourR;
  int mont_penalite;
}RETOUR;

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


#endif
