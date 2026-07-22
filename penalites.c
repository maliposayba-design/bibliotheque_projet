#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operation.h"


void menuPenalites() {
    int choix;
    do {
        printf("\n=== GESTION DES PÉNALITÉS ===\n");
        printf("1. Voir mes pénalités\n");
        printf("2. Payer une pénalité\n");
        printf("3. Retourner au menu principal\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch(choix) {
            case 1:
                afficherPenalites();
                break;
            case 2:
                payerPenalite();
                break;
            case 3:
                printf("Retour au menu principal...\n");
                break;
            default:
                printf("Choix invalide !\n");
        }
    } while(choix != 3);
}





void afficherPenalites() {
    int idUtilisateur;
    PENALITE penalite;
    FILE *fichier;
    int trouve = 0;

    printf("\n=== CONSULTER LES PÉNALITÉS ===\n");
    printf("Entrez votre ID utilisateur : ");
    scanf("%d", &idUtilisateur);

    fichier = fopen("DATABASE/PENALTIES.dat", "rb");
    if (fichier == NULL) {
        printf("Aucune pénalité enregistrée.\n");
        return;
    }

    printf("\n--- Pénalités de l'utilisateur ID %d ---\n", idUtilisateur);
    printf("ID | Montant | Date | Statut\n");
    printf("----------------------------------------\n");

    while (fread(&penalite, sizeof(PENALITE), 1, fichier) == 1) {
        if (penalite.idUtilisateur == idUtilisateur) {
            printf("%d | %.0f FCFA | %s | %s\n",
                   penalite.id, penalite.montant, penalite.datePenalite, penalite.statut);
            trouve = 1;
        }
    }

    fclose(fichier);

    if (!trouve) {
        printf("Aucune pénalité trouvée pour cet utilisateur.\n");
    }
}

void payerPenalite() {
    int idPenalite;
    Penalite penalite;
    FILE *fichier, *fichierTemp;
    int trouve = 0;

    printf("\n=== PAYER UNE PÉNALITÉ ===\n");
    printf("Entrez l'ID de la pénalité à payer : ");
    scanf("%d", &idPenalite);

    fichier = fopen("DATABASE/PENALTIES.dat", "rb");
    if (fichier == NULL) {
        printf("Erreur : Aucune pénalité trouvée !\n");
        return;
    }

    fichierTemp = fopen("DATABASE/TEMP_PENALITE.dat", "wb");
    if (fichierTemp == NULL) {
        printf("Erreur : Impossible de créer le fichier temporaire !\n");
        fclose(fichier);
        return;
    }

    while (fread(&penalite, sizeof(PENALITE), 1, fichier) == 1) {
        if (penalite.id == idPenalite && strcmp(penalite.statut, "NON_PAYEE") == 0) {
            trouve = 1;
            strcpy(penalite.statut, "PAYEE");
            printf("\n Pénalité ID %d payée avec succès !\n", idPenalite);
            printf("Montant : %.0f FCFA\n", penalite.montant);
        }
        fwrite(&penalite, sizeof(PENALITE), 1, fichierTemp);
    }

    fclose(fichier);
    fclose(fichierTemp);

    if (!trouve) {
        printf("Erreur : Pénalité non trouvée ou déjà payée !\n");
        remove("DATABASE/TEMP_PENALITE.dat");
        return;
    }

    remove("DATABASE/PENALTIES.dat");
    rename("DATABASE/TEMP_PENALITE.dat", "DATABASE/PENALTIES.dat");
}

