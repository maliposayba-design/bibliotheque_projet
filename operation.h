#ifndef OPERATION_H
#define OPERATION_H

typedef struct
{
  int idEmprunt;
  char num_emprunt[21];
  int idUtilisateur;
  int idLivre;
  char date_emprunt[25];
  char date_retour[25];
  char etat[20] ;

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
}PENALITE;

typedef struct {
    int annee, mois, jour;
    int heure, minute, seconde;
} DateHeure;

typedef struct{
    int id;
    char nom[50];
    char prenom[50];
    int telephone;
    char email[100];
    char login[7];
    char motPasse[100];
    char role[10];
    char etat[10];
    char dateCreation[20];
    char derniereConnexion[20];
    int premierconnexion;
}Utilisateur;

typedef struct
{
    int id;
    char isbn[20];
    char titre[100];
    int id_auteur;
    int id_categorie;
    char editeur[50];
    int annee_publication;
    char langue[30];
    int nb_pages;
    int nb_total_exemplaires;
    int nb_disponible;
    char emplacement[50];
    char resume[300];
    char date_ajout[11];
} Livre;



void ActualiserFBinaire();
void EmpruntLivre();
int verifierstock(int isbn_recherche);
void AjouterEmprunt();
int compterEmprunts(int idUtilisateur);

Utilisateur trouverUtilisateurParId(int id);
Livre trouverLivreParId(int id);

void menuPenalites();
void afficherPenalites();
void payerPenalite();
void menuRetours();
void retournerLivre();
void calculerPenalite(Emprunt emprunt, char dateRetour[20]);
void mettreAJourStock(int idLivre, int quantite);
void genererRecuEmprunt(Emprunt emprunt, Utilisateur utilisateur, Livre livre);
Utilisateur trouverUtilisateurParId(int id);
Livre trouverLivreParId(int id);
void menuEmprunts();
int compterEmprunts(int idUtilisateur);
int verifierstock(int isbn_recherche);
void AjouterEmprunt();
DateHeure ajouter14Jours(DateHeure depart);

#endif // OPERATION_H

