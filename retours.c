#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Retours.h"
#include "Emprunt.h"
#include "books.h"
#include "USERS.h"
#include "Penalites.h"

void menuRetours()
{
    int choix;
    do
    {
        printf("\n=== GESTION DES RETOURS ===\n");
        printf("1. Retourner un livre\n");
        printf("2. Retourner au menu principal\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix)
        {
            case 1:
                retournerLivre();
                break;
            case 2:
                printf("Retour au menu principal...\n");
                break;
            default:
                printf("Choix invalide !\n");
        }
    } while (choix != 2);
}

int estDateValide(char date[])
{
    int j, m, a;

    if (sscanf(date, "%d/%d/%d", &j, &m, &a) != 3)
        return 0;

    if (m < 1 || m > 12) return 0;
    if (j < 1 || j > 31) return 0;
    if (a < 2000 || a > 2100) return 0;

    return 1;
}

int calculerJoursRetard(char dateRetourPrevue[], char dateRetourReelle[])
{
    int jP, mP, aP, jR, mR, aR;
    struct tm tPrevue = {0}, tReelle = {0};
    time_t tp, tr;
    double diffSecondes;
    int jours;

    sscanf(dateRetourPrevue, "%d/%d/%d", &jP, &mP, &aP);
    sscanf(dateRetourReelle, "%d/%d/%d", &jR, &mR, &aR);

    tPrevue.tm_mday = jP;
    tPrevue.tm_mon  = mP - 1;
    tPrevue.tm_year = aP - 1900;
    tPrevue.tm_hour = 12;
    tReelle.tm_mday = jR;
    tReelle.tm_mon  = mR - 1;
    tReelle.tm_year = aR - 1900;
    tReelle.tm_hour = 12;

    tp = mktime(&tPrevue);
    tr = mktime(&tReelle);

    diffSecondes = difftime(tr, tp);
    jours = (int)(diffSecondes / (60 * 60 * 24));

    return (jours > 0) ? jours : 0;
}

void enregistrerPenalite(EMPRUNT emprunt, int idUtilisateur, int joursRetard, char dateRetour[20])
{
    PENALITE nouvellePenalite;
    FILE *fichier, *fId;
    int nouvelId;

    nouvellePenalite.idUtilisateur = idUtilisateur;
    nouvellePenalite.idEmprunt = emprunt.idEmprunt;
    nouvellePenalite.nbr_jourR = joursRetard;
    nouvellePenalite.mont_pen = joursRetard * 500;
    strcpy(nouvellePenalite.date, dateRetour);
    strcpy(nouvellePenalite.statut, "NON_PAYEE");

    fId = fopen("DATABASE/PENALTIES.dat", "rb");
    nouvelId = 1;
    if (fId != NULL)
    {
        PENALITE temp;
        while (fread(&temp, sizeof(PENALITE), 1, fId) == 1)
        {
            nouvelId = temp.id_pen + 1;
        }
        fclose(fId);
    }
    nouvellePenalite.id_pen = nouvelId;

    fichier = fopen("DATABASE/PENALTIES.dat", "ab");
    if (fichier != NULL)
    {
        fwrite(&nouvellePenalite, sizeof(PENALITE), 1, fichier);
        fclose(fichier);
    }
}

void mettreAJourStock(int idLivre, int quantite)
{
    BOOK livre;
    FILE *fichier, *fichierTemp;
    int trouve = 0;

    fichier = fopen("DATABASE/BOOKS.dat", "rb");
    if (fichier == NULL)
        return;

    fichierTemp = fopen("DATABASE/TEMP_BOOKS.dat", "wb");
    if (fichierTemp == NULL)
    {
        fclose(fichier);
        return;
    }

    while (fread(&livre, sizeof(BOOK), 1, fichier) == 1)
    {
        if (livre.id == idLivre)
        {
            trouve = 1;
            livre.nb_disponible += quantite;

            if (livre.nb_disponible < 0)
                livre.nb_disponible = 0;

            if (livre.nb_disponible > livre.nb_total_exemplaires)
                livre.nb_disponible = livre.nb_total_exemplaires;
        }
        fwrite(&livre, sizeof(BOOK), 1, fichierTemp);
    }

    fclose(fichier);
    fclose(fichierTemp);

    if (trouve)
    {
        remove("DATABASE/BOOKS.dat");
        rename("DATABASE/TEMP_BOOKS.dat", "DATABASE/BOOKS.dat");
    }
    else
    {
        remove("DATABASE/TEMP_BOOKS.dat");
    }
}

void genererRecuRetour(RETOUR retour, User utilisateur, BOOK livre)
{
    FILE *fichier;
    char nomFichier[150];
    char dateTemp[20];
    int i;

    strcpy(dateTemp, retour.dateR);
    for (i = 0; dateTemp[i] != '\0'; i++)
    {
        if (dateTemp[i] == '/')
            dateTemp[i] = '_';
    }

    sprintf(nomFichier, "REPORTS/RETURNS/RETURN_%s_%s.txt", dateTemp, utilisateur.login);

    fichier = fopen(nomFichier, "w");
    if (fichier == NULL)
    {
        printf("Erreur : Impossible de creer le recu de retour !\n");
        printf("(Verifiez que le dossier REPORTS/RETURNS/ existe bien)\n");
        return;
    }

    fprintf(fichier, "=== RECU DE RETOUR ===\n");
    fprintf(fichier, "ID Retour : %d\n", retour.idRetour);
    fprintf(fichier, "ID Emprunt : %d\n", retour.idEmprunt);
    fprintf(fichier, "Lecteur : %s %s\n", utilisateur.nom, utilisateur.prenom);
    fprintf(fichier, "BOOK : %s\n", livre.titre);
    fprintf(fichier, "Date de retour : %s\n", retour.dateR);
    fprintf(fichier, "Jours de retard : %d\n", retour.nbr_jourR);
    fprintf(fichier, "Montant penalite : %d FCFA\n", retour.mont_penalite);
    fprintf(fichier, "======================\n");

    fclose(fichier);
    printf("Recu de retour genere : %s\n", nomFichier);
}

void retournerLivre()
{
    int idUtilisateur, idLivre;
    char dateRetour[20];
    EMPRUNT emprunt;
    RETOUR nouveauRetour;
    BOOK livre;
    User utilisateur;
    FILE *fichierEmprunts, *fichierTemp, *fichierRetours, *fIdR;
    int trouve = 0;
    int joursRetard = 0;
    int nouvelIdR;

    printf("\n=== RETOURNER UN LIVRE ===\n");

    printf("Entrez votre ID utilisateur : ");
    scanf("%d", &idUtilisateur);
    printf("Entrez l'ID du livre a retourner : ");
    scanf("%d", &idLivre);

    utilisateur = trouverUtilisateurParId(idUtilisateur);
    if (utilisateur.id == -1)
    {
        printf("Erreur : User non trouve !\n");
        return;
    }

    livre = trouverLivreParId(idLivre);
    if (livre.id == -1)
    {
        printf("Erreur : BOOK non trouve !\n");
        return;
    }

    printf("Entrez la date de retour (format JJ/MM/AAAA) : ");
    scanf("%s", dateRetour);
    if (!estDateValide(dateRetour))
    {
        printf("Erreur : Date invalide !\n");
        return;
    }

    fichierEmprunts = fopen("DATABASE/BORROWS.dat", "rb");
    if (fichierEmprunts == NULL)
    {
        printf("Erreur : Impossible d'ouvrir le fichier des emprunts !\n");
        return;
    }

    fichierTemp = fopen("DATABASE/TEMP.dat", "wb");
    if (fichierTemp == NULL)
    {
        printf("Erreur : Impossible de creer le fichier temporaire !\n");
        fclose(fichierEmprunts);
        return;
    }

    while (fread(&emprunt, sizeof(EMPRUNT), 1, fichierEmprunts) == 1)
    {
        if (!trouve &&
            emprunt.idUtilisateur == idUtilisateur &&
            emprunt.idLivre == idLivre &&
            strcmp(emprunt.etat, "EN_COURS") == 0)
        {
            trouve = 1;
            strcpy(emprunt.etat, "RETOURNE");

            joursRetard = calculerJoursRetard(emprunt.date_retour, dateRetour);

            nouveauRetour.idEmprunt = emprunt.idEmprunt;
            strcpy(nouveauRetour.dateR, dateRetour);
            nouveauRetour.nbr_jourR = joursRetard;
            nouveauRetour.mont_penalite = joursRetard * 500;

            nouvelIdR = 1;
            if (fIdR != NULL)
            {
                RETOUR tempR;
                while (fread(&tempR, sizeof(RETOUR), 1, fIdR) == 1)
                {
                    nouvelIdR = tempR.idRetour + 1;
                }
                fclose(fIdR);
            }
            nouveauRetour.idRetour = nouvelIdR;

            fichierRetours = fopen("DATABASE/RETURNS.dat", "ab");
            if (fichierRetours != NULL)
            {
                fwrite(&nouveauRetour, sizeof(RETOUR), 1, fichierRetours);
                fclose(fichierRetours);
            }

            if (joursRetard > 0)
            {
                enregistrerPenalite(emprunt, idUtilisateur, joursRetard, dateRetour);
            }
        }
        fwrite(&emprunt, sizeof(EMPRUNT), 1, fichierTemp);
    }

    fclose(fichierEmprunts);
    fclose(fichierTemp);

    if (!trouve)
    {
        printf("Erreur : Aucun emprunt en cours trouve pour ce livre !\n");
        remove("DATABASE/TEMP.dat");
        return;
    }

    remove("DATABASE/BORROWS.dat");
    rename("DATABASE/TEMP.dat", "DATABASE/BORROWS.dat");


    mettreAJourStock(idLivre, 1);

    genererRecuRetour(nouveauRetour, utilisateur, livre);

    printf("\nRetour effectue avec succes !\n");
    if (joursRetard > 0)
    {
        printf("Attention : %d jours de retard - Penalite de %d FCFA\n",
               joursRetard, nouveauRetour.mont_penalite);
    }
}