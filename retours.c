#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operation.h"





void menuRetours() {
    int choix;
    do {
        printf("\n=== GESTION DES RETOURS ===\n");
        printf("1. Retourner un livre\n");
        printf("2. Voir historique des retours\n");
        printf("3. Retourner au menu principal\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch(choix) {
            case 1:
                retournerLivre();
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













void retournerLivre() {
    int idUtilisateur, idLivre;
    char dateRetour[20];
    EMPRUNT empruntTrouve;
    Retour nouveauRetour;
    Livre livre;
    Utilisateur utilisateur;
    FILE *fichierEmprunts, *fichierTemp, *fichierRetours;
    int trouve = 0;
    Emprunt emprunt;

    printf("\n=== RETOURNER UN LIVRE ===\n");

    printf("Entrez votre ID utilisateur : ");
    scanf("%d", &idUtilisateur);
    printf("Entrez l'ID du livre à retourner : ");
    scanf("%d", &idLivre);

    utilisateur = trouverUtilisateurParId(idUtilisateur);
    if (utilisateur.id == -1) {
        printf("Erreur : Utilisateur non trouvé !\n");
        return;
    }

    livre = trouverLivreParId(idLivre);
    if (livre.id == -1) {
        printf("Erreur : Livre non trouvé !\n");
        return;
    }

    printf("Entrez la date de retour (format JJ/MM/AAAA) : ");
    scanf("%s", dateRetour);
    if (!estDateValide(dateRetour)) {
        printf("Erreur : Date invalide !\n");
        return;
    }

    fichierEmprunts = fopen("DATABASE/BORROWS.dat", "rb");
    if (fichierEmprunts == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier des emprunts !\n");
        return;
    }

    fichierTemp = fopen("DATABASE/TEMP.dat", "wb");
    if (fichierTemp == NULL) {
        printf("Erreur : Impossible de créer le fichier temporaire !\n");
        fclose(fichierEmprunts);
        return;
    }

    while (fread(&EMPRUNT, sizeof(EMPRUNT), 1, fichierEmprunts) == 1) {
        if (EMPRUNT.idUtilisateur == idUtilisateur &&
            EMPRUNT.idLivre == idLivre &&
            strcmp(EMPRUNT.etat, "EN_COURS") == 0) {
            trouve = 1;
            empruntTrouve = emprunt;
            strcpy(EMPRUNT.etat, "RETOURNE");

            nouveauRetour.id = 1;
            nouveauRetour.idUtilisateur = idUtilisateur;
            nouveauRetour.idLivre = idLivre;
            strcpy(nouveauRetour.dateRetour, dateRetour);

            calculerPenalite(EMPRUNT, dateRetour);

            fichierRetours = fopen("DATABASE/RETURNS.dat", "ab");
            if (fichierRetours != NULL) {
                fwrite(&nouveauRetour, sizeof(Retour), 1, fichierRetours);
                fclose(fichierRetours);
            }
        }
        fwrite(&emprunt, sizeof(Emprunt), 1, fichierTemp);
    }

    fclose(fichierEmprunts);
    fclose(fichierTemp);

    if (!trouve) {
        printf("Erreur : Aucun emprunt en cours trouvé pour ce livre !\n");
        remove("DATABASE/TEMP.dat");
        return;
    }

    remove("DATABASE/BORROWS.dat");
    rename("DATABASE/TEMP.dat", "DATABASE/BORROWS.dat");

    mettreAJourStock(idLivre, 1);

    genererRecuRetour(nouveauRetour, utilisateur, livre);

    printf("\n Retour effectué avec succès !\n");
    if (nouveauRetour.joursRetard > 0) {
        printf("Attention : %d jours de retard - Pénalité de %.0f FCFA\n",
               nouveauRetour.joursRetard, nouveauRetour.montantPenalite);
    }
}






void calculerPenalite(Emprunt emprunt, char dateRetour[20]) {
    int jourRetour, moisRetour, anneeRetour;
    int jourPrevu, moisPrevu, anneePrevu;
    int joursRetard = 0;
    double montant = 0;
    Penalite nouvellePenalite;
    FILE *fichier;

    sscanf(dateRetour, "%d/%d/%d", &jourRetour, &moisRetour, &anneeRetour);
    sscanf(emprunt.dateRetourPrevue, "%d/%d/%d", &jourPrevu, &moisPrevu, &anneePrevu);

    if (anneeRetour > anneePrevu ||
        (anneeRetour == anneePrevu && moisRetour > moisPrevu) ||
        (anneeRetour == anneePrevu && moisRetour == moisPrevu && jourRetour > jourPrevu)) {

        int jourCourant = jourPrevu;
        int moisCourant = moisPrevu;
        int anneeCourant = anneePrevu;

        while (anneeCourant < anneeRetour ||
               (anneeCourant == anneeRetour && moisCourant < moisRetour) ||
               (anneeCourant == anneeRetour && moisCourant == moisRetour && jourCourant < jourRetour)) {

            joursRetard++;
            jourCourant++;
            if (jourCourant > getNombreJoursMois(moisCourant, anneeCourant)) {
                jourCourant = 1;
                moisCourant++;
                if (moisCourant > 12) {
                    moisCourant = 1;
                    anneeCourant++;
                }
            }
        }
    }

    montant = joursRetard * 500.0;

    if (joursRetard > 0) {
        nouvellePenalite.id = 1;
        nouvellePenalite.idEmprunt = emprunt.id;
        nouvellePenalite.idUtilisateur = emprunt.idUtilisateur;
        nouvellePenalite.montant = montant;
        strcpy(nouvellePenalite.datePenalite, dateRetour);
        strcpy(nouvellePenalite.statut, "NON_PAYEE");

        fichier = fopen("DATABASE/PENALTIES.dat", "ab");
        if (fichier != NULL) {
            fwrite(&nouvellePenalite, sizeof(Penalite), 1, fichier);
            fclose(fichier);
        }
    }
}




void mettreAJourStock(int idLivre, int quantite) {
    Livre livre;
    FILE *fichier, *fichierTemp;
    int trouve = 0;

    fichier = fopen("DATABASE/BOOKS.dat", "rb");
    if (fichier == NULL) {
        return;
    }

    fichierTemp = fopen("DATABASE/TEMP_BOOKS.dat", "wb");
    if (fichierTemp == NULL) {
        fclose(fichier);
        return;
    }

    while (fread(&livre, sizeof(Livre), 1, fichier) == 1) {
        if (livre.id == idLivre) {
            trouve = 1;
            livre.exemplairesDisponibles += quantite;
            if (livre.exemplairesDisponibles < 0) {
                livre.exemplairesDisponibles = 0;
            }
        }
        fwrite(&livre, sizeof(Livre), 1, fichierTemp);
    }

    fclose(fichier);
    fclose(fichierTemp);

    if (trouve) {
        remove("DATABASE/BOOKS.dat");
        rename("DATABASE/TEMP_BOOKS.dat", "DATABASE/BOOKS.dat");
    } else {
        remove("DATABASE/TEMP_BOOKS.dat");
    }
}



void genererRecuEmprunt(Emprunt emprunt, Utilisateur utilisateur, Livre livre) {
    FILE *fichier;
    char nomFichier[100];
    char dateTemp[20];

    strcpy(dateTemp, emprunt.dateEmprunt);
    for (int i = 0; i < strlen(dateTemp); i++) {
        if (dateTemp[i] == '/') {
            dateTemp[i] = '_';
        }
    }
    sprintf(nomFichier, "REPORTS/BORROWS/BORROW_%s_%s.txt", dateTemp, utilisateur.login);

    fichier = fopen(nomFichier, "w");
    if (fichier == NULL) {
        printf("Erreur : Impossible de créer le reçu d'emprunt !\n");
        return;
    }

    fprintf(fichier, "=== REÇU D'EMPRUNT ===\n");
    fprintf(fichier, "Numéro d'emprunt : %s\n", emprunt.numeroEmprunt);
    fprintf(fichier, "Lecteur : %s %s\n", utilisateur.nom, utilisateur.prenom);
    fprintf(fichier, "Login : %s\n", utilisateur.login);
    fprintf(fichier, "Date d'emprunt : %s\n", emprunt.dateEmprunt);
    fprintf(fichier, "Date prévue de retour : %s\n", emprunt.dateRetourPrevue);
    fprintf(fichier, "Livre : %s\n", livre.titre);
    fprintf(fichier, "Auteur : ID %d\n", livre.idAuteur);
    fprintf(fichier, "ISBN : %s\n", livre.isbn);
    fprintf(fichier, "=====================\n");

    fclose(fichier);
    printf("Reçu d'emprunt généré : %s\n", nomFichier);
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


