#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct {
    int id;
    char numEmprunt[30];
    int idUtilisateur;
    int idLivre;
    char dateEmprunt[20];
    char dateRetourPrevue[20];
    char etat[20];
} EMPRUNT;

typedef struct {
    int id;
    int idEmprunt;
    char dateRetourEffective[20];
    int enRetard;
    float montantPenalite;
} RETOUR;

typedef struct {
    int id;
    int idUtilisateur;
    int idEmprunt;
    float montant;
    char dateCalcul[20];
    int payee;
} PENALITE;

#endif
