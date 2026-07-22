#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Penalites.h"
#include "Emprunt.h"
#include "Retours.h"
#include "books.h"
#include "USERS.h"

void menuEmprunts()
{
    int choix;
    do
    {
        printf("\n=== GESTION DES EMPRUNTS ===\n");
        printf("1. Emprunter un livre\n");
        printf("2. Retourner au menu principal\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix)
        {
            case 1:
                AjouterEmprunt();
                break;
            case 2:
                printf("Retour au menu principal...\n");
                break;
            default:
                printf("Choix invalide !\n");
        }
    } while (choix != 2);
}

int compterEmprunts(int idUtilisateur)
{
    FILE *f = fopen("DATABASE/BORROWS.dat", "rb");
    EMPRUNT emp;
    int compteur = 0;

    if (f == NULL)
        return 0;

    while (fread(&emp, sizeof(EMPRUNT), 1, f) == 1)
    {
        if (emp.idUtilisateur == idUtilisateur && strcmp(emp.etat, "EN_COURS") == 0)
        {
            compteur++;
        }
    }
    fclose(f);
    return compteur;
}

void genererRecuEmprunt(EMPRUNT emprunt, User utilisateur, BOOK livre)
{
    FILE *fichier;
    char nomFichier[150];
    char dateTemp[25];
    int i;

    strcpy(dateTemp, emprunt.date_emprunt);
    for (i = 0; dateTemp[i] != '\0'; i++)
    {
        if (dateTemp[i] == '/' || dateTemp[i] == ':' || dateTemp[i] == ' ')
            dateTemp[i] = '_';
    }

    sprintf(nomFichier, "REPORTS/BORROWS/BORROW_%s_%s.txt", dateTemp, utilisateur.login);

    fichier = fopen(nomFichier, "w");
    if (fichier == NULL)
    {
        printf("Erreur : Impossible de creer le recu d'emprunt !\n");
        printf("(Verifiez que le dossier REPORTS/BORROWS/ existe bien)\n");
        return;
    }

    fprintf(fichier, "=== RECU D'EMPRUNT ===\n");
    fprintf(fichier, "Numero d'emprunt : %s\n", emprunt.num_emprunt);
    fprintf(fichier, "Lecteur : %s %s\n", utilisateur.nom, utilisateur.prenom);
    fprintf(fichier, "Login : %s\n", utilisateur.login);
    fprintf(fichier, "Date d'emprunt : %s\n", emprunt.date_emprunt);
    fprintf(fichier, "Date prevue de retour : %s\n", emprunt.date_retour);
    fprintf(fichier, "BOOK : %s\n", livre.titre);
    fprintf(fichier, "ID Auteur : %d\n", livre.id_auteur);
    fprintf(fichier, "ISBN : %s\n", livre.isbn);
    fprintf(fichier, "======================\n");

    fclose(fichier);
    printf("Recu d'emprunt genere : %s\n", nomFichier);
}

void AjouterEmprunt()
{
    int idUser, idLiv;
    User U;
    BOOK L;
    EMPRUNT nouvelEmp;
    FILE *f, *fId;
    time_t maintenant, dateLimite;
    struct tm *tInfo;
    int nouvelId;

    printf("\n========== NOUVEL EMPRUNT ==========\n");

    printf("Entrez l'identifiant de l'utilisateur : ");
    scanf("%d", &idUser);

    U = trouverUtilisateurParId(idUser);
    if (U.id == -1)
    {
        printf("Erreur : User non trouve !\n");
        return;
    }

    if (strcmp(U.etat, "BLOQUE") == 0)
    {
        printf("Erreur : Ce compte est bloque !\n");
        return;
    }

    if (compterEmprunts(idUser) >= 3)
    {
        printf("Erreur : Cet utilisateur a deja atteint son quota maximum de 3 emprunts.\n");
        return;
    }

    printf("Entrez l'identifiant du livre : ");
    scanf("%d", &idLiv);

    L = trouverLivreParId(idLiv);
    if (L.id == -1)
    {
        printf("Erreur : BOOK non trouve !\n");
        return;
    }

    if (L.nb_disponible <= 0)
    {
        printf("Erreur : Aucun exemplaire disponible !\n");
        return;
    }

    maintenant = time(NULL);
    tInfo = localtime(&maintenant);

    sprintf(nouvelEmp.date_emprunt, "%02d/%02d/%04d %02d:%02d:%02d",
            tInfo->tm_mday, tInfo->tm_mon + 1, tInfo->tm_year + 1900,
            tInfo->tm_hour, tInfo->tm_min, tInfo->tm_sec);

    sprintf(nouvelEmp.num_emprunt, "EMP_%04d%02d%02d%02d%02d%02d",
            tInfo->tm_year + 1900, tInfo->tm_mon + 1, tInfo->tm_mday,
            tInfo->tm_hour, tInfo->tm_min, tInfo->tm_sec);

    dateLimite = maintenant + (14 * 24 * 60 * 60);
    tInfo = localtime(&dateLimite);

    sprintf(nouvelEmp.date_retour, "%02d/%02d/%04d %02d:%02d:%02d",
            tInfo->tm_mday, tInfo->tm_mon + 1, tInfo->tm_year + 1900,
            tInfo->tm_hour, tInfo->tm_min, tInfo->tm_sec);

    strcpy(nouvelEmp.etat, "EN_COURS");
    nouvelEmp.idUtilisateur = idUser;
    nouvelEmp.idLivre = idLiv;

    fId = fopen("DATABASE/BORROWS.dat", "rb");
    nouvelId = 1;
    if (fId != NULL)
    {
        EMPRUNT temp;
        while (fread(&temp, sizeof(EMPRUNT), 1, fId) == 1)
        {
            nouvelId = temp.idEmprunt + 1;
        }
        fclose(fId);
    }
    nouvelEmp.idEmprunt = nouvelId;

    f = fopen("DATABASE/BORROWS.dat", "ab");
    if (f == NULL)
    {
        printf("Erreur d'ouverture du fichier de stockage.\n");
        return;
    }
    fwrite(&nouvelEmp, sizeof(EMPRUNT), 1, f);
    fclose(f);


    mettreAJourStock(idLiv, -1);

    genererRecuEmprunt(nouvelEmp, U, L);

    printf("\nEmprunt valide avec succes !\n");
    printf("Numero genere : %s\n", nouvelEmp.num_emprunt);
    printf("Date d'emprunt : %s\n", nouvelEmp.date_emprunt);
    printf("Date limite de retour : %s\n", nouvelEmp.date_retour);
}

User trouverUtilisateurParId(int id)
{
    User utilisateur;
    FILE *fichier;

    utilisateur.id = -1;

    fichier = fopen("DATABASE/USERS.dat", "rb");
    if (fichier == NULL)
        return utilisateur;

    while (fread(&utilisateur, sizeof(User), 1, fichier) == 1)
    {
        if (utilisateur.id == id)
        {
            fclose(fichier);
            return utilisateur;
        }
    }

    fclose(fichier);
    utilisateur.id = -1;
    return utilisateur;
}

BOOK trouverLivreParId(int id)
{
    BOOK livre;
    FILE *fichier;

    livre.id = -1;

    fichier = fopen("DATABASE/BOOKS.dat", "rb");
    if (fichier == NULL)
        return livre;

    while (fread(&livre, sizeof(BOOK), 1, fichier) == 1)
    {
        if (livre.id == id)
        {
            fclose(fichier);
            return livre;
        }
    }

    fclose(fichier);
    livre.id = -1;
    return livre;
}