#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Penalites.h"
#include "Emprunt.h"
#include "books.h"
#include "USERS.h"
#include "structure.h"

void menuPenalites()
{
    int choix;
    do
    {
        printf("\n=== GESTION DES PENALITES ===\n");
        printf("1. Voir mes penalites\n");
        printf("2. Payer une penalite\n");
        printf("3. Retourner au menu principal\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix)
        {
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
    } while (choix != 3);
}

void afficherPenalites()
{
    int idUtilisateur;
    PENALITE penalite;
    FILE *fichier;
    int trouve = 0;

    printf("\n=== CONSULTER LES PENALITES ===\n");
    printf("Entrez votre ID utilisateur : ");
    scanf("%d", &idUtilisateur);

    fichier = fopen("DATABASE/PENALTIES.dat", "rb");
    if (fichier == NULL)
    {
        printf("Aucune penalite enregistree.\n");
        return;
    }

    printf("\n--- Penalites de l'utilisateur ID %d ---\n", idUtilisateur);
    printf("ID | Montant | Date | Statut\n");
    printf("----------------------------------------\n");

    while (fread(&penalite, sizeof(PENALITE), 1, fichier) == 1)
    {
        if (penalite.idUtilisateur == idUtilisateur)
        {
            printf("%d | %d FCFA | %s | %s\n",
                   penalite.id_pen, penalite.mont_pen, penalite.date, penalite.statut);
            trouve = 1;
        }
    }
    fclose(fichier);

    if (!trouve)
    {
        printf("Aucune penalite trouvee pour cet utilisateur.\n");
    }
}

void payerPenalite()
{
    int idPenalite;
    PENALITE penalite;
    FILE *fichier, *fichierTemp;
    int trouve = 0;

    printf("\n=== PAYER UNE PENALITE ===\n");
    printf("Entrez l'ID de la penalite a payer : ");
    scanf("%d", &idPenalite);

    fichier = fopen("DATABASE/PENALTIES.dat", "rb");
    if (fichier == NULL)
    {
        printf("Erreur : Aucune penalite trouvee !\n");
        return;
    }

    fichierTemp = fopen("DATABASE/TEMP_PENALITE.dat", "wb");
    if (fichierTemp == NULL)
    {
        printf("Erreur : Impossible de creer le fichier temporaire !\n");
        fclose(fichier);
        return;
    }

    while (fread(&penalite, sizeof(PENALITE), 1, fichier) == 1)
    {
        if (penalite.id_pen == idPenalite && strcmp(penalite.statut, "NON_PAYEE") == 0)
        {
            trouve = 1;
            strcpy(penalite.statut, "PAYEE");
            printf("\nPenalite ID %d payee avec succes !\n", idPenalite);
            printf("Montant : %d FCFA\n", penalite.mont_pen);
        }
        fwrite(&penalite, sizeof(PENALITE), 1, fichierTemp);
    }

    fclose(fichier);
    fclose(fichierTemp);

    if (!trouve)
    {
        printf("Erreur : Penalite non trouvee ou deja payee !\n");
        remove("DATABASE/TEMP_PENALITE.dat");
        return;
    }

    remove("DATABASE/PENALTIES.dat");
    rename("DATABASE/TEMP_PENALITE.dat", "DATABASE/PENALTIES.dat");
}
