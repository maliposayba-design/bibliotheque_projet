#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operation.h"



void menuEmprunts(){
    int choix;
    do {
        printf("\n=== GESTION DES Emprunts ===\n");
        printf("1. Emprunter un livre\n");
        printf("2. Voir historique des retours\n");
        printf("3. Retourner au menu principal\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch(choix) {
            case 1:
                AjouterEmprunt();
                break;
            case 2:
                break;
            case 3:
                printf("Retour au menu principal...\n");
                break;
            default:
                printf("Choix invalide !\n");
        }
    } while(choix != 3);
}














int compterEmprunts(int idUtilisateur) {
    FILE *f = fopen("DATABASE/BORROWS.dat", "rb");
    EMPRUNT emp;
    int compteur=0;
    if (!f){
        return 0;
    }

    Emprunt emp;
    int compteur = 0;
    while (fread(&emp, sizeof(EMPRUNT), 1, f)) {
        if (emp.idUtilisateur == idUtilisateur && strcmp(emp.etat, "EN_COURS") == 0) {
            compteur++;
        }
    }
    fclose(f);
    return compteur;
}


int verifierstock(int isbn_recherche){
        FILE *f_books = fopen("DATABASE/BOOKS.dat", "rb+");
        if (f_books == NULL) {
            printf("[Erreur] Impossible d'ouvrir le registre des livres (BOOKS.dat).\n");
            return;
        }
      while (fread(&livre_courant, sizeof(Livre), 1, f_books) == 1) {
            if (strcmp(livre_courant.isbn, isbn_recherche) == 0) {
                livre_trouve = 1;
                  if (livre_courant.stock <= 0) {
                    printf("[Alerte] Le livre '%s' est épuisé (Stock : 0).\n", livre_courant.titre);
                    fclose(f_books);
                    return;
                }
                 position_livre = ftell(f_books) - sizeof(Livre);
                break;
            }
        }

        if (!livre_trouve) {
            printf("[Erreur] Aucun livre trouvé avec l'ISBN : %s\n", isbn_recherche);
            fclose(f_books);
            return;
        }
        livre_courant.stock--;
        fseek(f_books, position_livre, SEEK_SET);

        fwrite(&livre_courant, sizeof(Livre), 1, f_books);
        fclose(f_books);

        return 1;

}


void AjouterEmprunt(){
    int idUser, idLiv;
    Utilisateur U;
    Livre L;
    FILE *f;

    EMPRUNT E;

    printf("\n========== NOUVEL EMPRUNT ==========\n");

    printf("Entrez l'identifiant de l'utilisateur : ");
    scanf("%d", &E.idUtilisateur);
    printf("Entrez l'identifiant du livre : ");
    scanf("%d", &E.idLivre);

    if (compterEmprunts(idUser) >= 3) {
        printf("Erreur : Cet utilisateur a deja atteint son quota maximum de 3 emprunts.\n");
        return;
    }
    U = trouverUtilisateurParId(idUser);
    if (U.id == -1) {
        printf("Erreur : Utilisateur non trouvé !\n");
        return;
    }
    L = trouverLivreParId(idLiv);
    if (L.id == -1) {
        printf("Erreur : Livre non trouvé !\n");
        return;
    }
    if (strcmp(U.etat, "BLOQUE") == 0) {
        printf("Erreur : Votre compte est bloqué !\n");
        return;
    }
     if (livre.nb_disponible <= 0) {
        printf("Erreur : Aucun exemplaire disponible !\n");
        return;
    }


    DateHeure D;
    printf("Saisie de la date d'emprunt :\n");
    printf("Annee (AAAA) : ");
    scanf("%d", &D.annee);
    printf("Mois (MM) : ");
    scanf("%d", &D.mois);
    printf("Jour (JJ) : ");
    scanf("%d", &D.jour);
    printf("Heure (HH) : ");
    scanf("%d", &D.heure);
    printf("Minute (MM) : ");
    scanf("%d", &D.minute);
    printf("Seconde (SS) : ");
    scanf("%d", &D.seconde);


    EMPRUNT nouvelEmp;
    nouvelEmp.idUtilisateur = idUser;
    nouvelEmp.idLivre = idLiv;
    nouvelEmp.date_emprunt = ;

    nouvelEmp.date_retour = ajouter14Jours(de);
    strcpy(nouvelEmp.etat, "EN_COURS");
    sprintf(nouvelEmp.num_emprunt, "EMP_%04d%02d%02d%02d%02d%02d",D.annee, D.mois, D.jour, D.heure, D.minute, D.seconde);

     FILE *fId = fopen("DATABASE/BORROWS.dat", "rb");
    int nouvelId = 1;
    if (fId) {
        EMPRUNT temp;
        while (fread(&temp, sizeof(EMPRUNT), 1, fId)) {
            nouvelId = temp.id + 1;
        }
        fclose(fId);
    }
    nouvelEmp.id = nouvelId;

    FILE *fSave = fopen("DATABASE/BORROWS.dat", "ab");
    if (fSave) {
        fwrite(&nouvelEmp, sizeof(Emprunt), 1, fSave);
        fclose(fSave);
        printf("Emprunt valide' avec succe's !\n");
        printf("Numero genere' : %s\n", nouvelEmp.num_emprunt);
        printf("Date limite de retour : %02d/%02d/%04d\n",
               nouvelEmp.datePrevueRetour.jour, nouvelEmp.datePrevueRetour.mois, nouvelEmp.datePrevueRetour.annee);
    } else {
        printf("Erreur d'ouverture du fichier de stockage.\n");
    }
}





Utilisateur trouverUtilisateurParId(int id) {
    Utilisateur utilisateur;
    FILE *fichier;

    utilisateur.id = -1;

    fichier = fopen("DATABASE/USERS.dat", "rb");
    if (fichier == NULL) {
        return utilisateur;
    }

    while (fread(&utilisateur, sizeof(Utilisateur), 1, fichier) == 1) {
        if (utilisateur.id == id) {
            fclose(fichier);
            return utilisateur;
        }
    }

    fclose(fichier);
    utilisateur.id = -1;
    return utilisateur;
}

Livre trouverLivreParId(int id) {
    Livre livre;
    FILE *fichier;

    livre.id = -1;

    fichier = fopen("DATABASE/BOOKS.dat", "rb");
    if (fichier == NULL) {
        return livre;
    }

    while (fread(&livre, sizeof(Livre), 1, fichier) == 1) {
        if (livre.id == id) {
            fclose(fichier);
            return livre;
        }
    }

    fclose(fichier);
    livre.id = -1;
    return livre;
}

DateHeure ajouter14Jours(DateHeure depart) {
    int jours[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (depart.annee % 4 == 0) jours[2] = 29;

    DateHeure limite = depart;
    limite.jour += 14;

    if (limite.jour > jours[limite.mois]) {
        limite.jour -= jours[limite.mois];

        if (limite.mois > 12) {
            limite.mois = 1;
            limite.annee++;
        }
    }
    return limite;
}

