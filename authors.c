#include <stdio.h>
#include "books.h"
#include "authors.h"

int generer_id_auteur()
{
    AUTHORS dernier;
    FILE *f;

    f = fopen("DATABASE/AUTHORS.dat", "rb");

    if (f == NULL)
        return 1;

    fseek(f, 0, SEEK_END);

    if (ftell(f) == 0)
    {
        fclose(f);
        return 1;
    }

    fseek(f, -sizeof(AUTHORS), SEEK_END);

    fread(&dernier, sizeof(AUTHORS), 1, f);

    fclose(f);

    return dernier.id + 1;
}

void saisir_auteur(AUTHORS *A)
{
    printf("\n========== SAISIE D'UN AUTEUR ==========\n");

    printf("Nom complet : ");
    scanf(" %[^\n]", A->nom_auteur);

    printf("Nationalite : ");
    scanf(" %[^\n]", A->nationalite);

    printf("Date de naissance (jj/mm/aaaa) : ");
    scanf("%10s", A->date_naissance);

    printf("Biographie : ");
    scanf(" %[^\n]", A->biographie);
 do
{
    printf("Nombre de livres publies : ");
    scanf("%d", &A->nb_livres);

    if (A->nb_livres < 0)
        printf("Le nombre de livres ne peut pas etre negatif. Veuillez reessayer.\n");

} while (A->nb_livres < 0);
}

void enregistrer_auteur(AUTHORS A)
{
    FILE *f;

    f = fopen("DATABASE/AUTHORS.dat", "ab");

    if (f == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    fwrite(&A, sizeof(AUTHORS), 1, f);

    fclose(f);
}

void ajout_auteur()
{
    AUTHORS A;

    A.id = generer_id_auteur();

    saisir_auteur(&A);

    enregistrer_auteur(A);

    printf("\nAuteur ajoute avec succes !\n");
}

void afficher_auteur(AUTHORS A)
{
    printf("\n=========================================\n");
    printf("       INFORMATIONS DE L'AUTEUR\n");
    printf("=========================================\n");

    printf("ID                 : %d\n", A.id);
    printf("Nom                : %s\n", A.nom_auteur);
    printf("Nationalite        : %s\n", A.nationalite);
    printf("Date de naissance  : %s\n", A.date_naissance);
    printf("Nombre de livres   : %d\n", A.nb_livres);

    printf("\nBiographie :\n");
    printf("-----------------------------------------\n");
    printf("%s\n", A.biographie);
    printf("-----------------------------------------\n");
}

void afficher_liste_auteurs()
{
    FILE *f;
    AUTHORS A;
    int compteur = 0;

    f = fopen("DATABASE/AUTHORS.dat", "rb");

    if (f == NULL)
    {
        printf("\nAucun auteur enregistre.\n");
        return;
    }

    printf("\n===========================================\n");
    printf("           LISTE DES AUTEURS\n");
    printf("===========================================\n");

    printf("%-5s %-30s\n", "ID", "NOM");
    printf("-------------------------------------------\n");

    rewind(f);

    while (fread(&A, sizeof(AUTHORS), 1, f) == 1)
    {
        printf("%-5d %-30s\n", A.id, A.nom_auteur);
        compteur++;
    }

    printf("-------------------------------------------\n");
    printf("Nombre total d'auteurs : %d\n", compteur);

    fclose(f);
}

void modifier_auteur()
{
    FILE *f;
    AUTHORS A;
    int id, choix;
    int trouve = 0;

    f = fopen("DATABASE/AUTHORS.dat", "rb+");

    if (f == NULL)
    {
        printf("Erreur d'ouverture du fichier.\n");
        return;
    }
    afficher_liste_auteurs();
    printf("\nEntrer l'ID de l'auteur a modifier : ");
    scanf("%d", &id);

    while (fread(&A, sizeof(AUTHORS), 1, f) == 1)
    {
        if (A.id == id)
        {
            trouve = 1;

            printf("\nAuteur trouve :\n");
            afficher_auteur(A);

            printf("\nQue voulez-vous modifier ?\n");
            printf("1. Nom\n");
            printf("2. Nationalite\n");
            printf("3. Date de naissance\n");
            printf("4. Biographie\n");
            printf("5. Nombre de livres\n");
            printf("6. Tout modifier\n");

            printf("Votre choix : ");
            scanf("%d", &choix);

            switch (choix)
            {
                case 1:
                    printf("Nouveau nom : ");
                    scanf(" %[^\n]", A.nom_auteur);
                    break;

                case 2:
                    printf("Nouvelle nationalite : ");
                    scanf(" %[^\n]", A.nationalite);
                    break;

                case 3:
                    printf("Nouvelle date : ");
                    scanf("%10s", A.date_naissance);
                    break;

                case 4:
                    printf("Nouvelle biographie : ");
                    scanf(" %[^\n]", A.biographie);
                    break;

               case 5:
                    do
                      {
                       printf("Nouveau nombre de livres : ");
                       scanf("%d",&A.nb_livres);

                      if(A.nb_livres<0)
                      printf("Le nombre de livres ne peut pas etre negatif.\n");

                     }while(A.nb_livres<0);

                    break;

                case 6:
                    saisir_auteur(&A);
                    break;

                default:
                    printf("Choix invalide.\n");
                    fclose(f);
                    return;
            }

            fseek(f, -sizeof(AUTHORS), SEEK_CUR);

            fwrite(&A, sizeof(AUTHORS), 1, f);

            printf("\nAuteur modifie avec succes !\n");

            break;
        }
    }

    if (!trouve)
    {
        printf("\nAuteur introuvable.\n");
    }

    fclose(f);
}

void supprimer_auteur()
{
    FILE *f, *temp;
    AUTHORS A;
    int id;
    int trouve = 0;
    int confirmation;

    f = fopen("DATABASE/AUTHORS.dat", "rb");
    temp = fopen("DATABASE/TEMP.dat", "wb");

    if (f == NULL || temp == NULL)
    {
        printf("Erreur d'ouverture des fichiers.\n");
        return;
    }
    afficher_liste_auteurs();

    printf("\nEntrer l'ID de l'auteur a supprimer : ");
    scanf("%d", &id);

    while (fread(&A, sizeof(AUTHORS), 1, f) == 1)
    {
        if (A.id == id)
{
    if (auteur_a_des_livres(A.id))
    {
        printf("\nImpossible de supprimer cet auteur.\n");
        printf("Des livres lui sont encore associes.\n");

        fclose(f);
        fclose(temp);
        remove("DATABASE/TEMP.dat");
        return;
    }

    afficher_auteur(A);

    printf("\nVoulez-vous vraiment supprimer cet auteur ?\n");
            printf("1. Oui\n");
            printf("2. Non\n");
            printf("Votre choix : ");
            scanf("%d", &confirmation);

            if (confirmation == 1)
            {
                trouve = 1;
                printf("\nAuteur supprime avec succes.\n");
                continue;   // Ne pas copier cet auteur
            }
        }

        fwrite(&A, sizeof(AUTHORS), 1, temp);
    }

    fclose(f);
    fclose(temp);

    if (trouve)
    {
        remove("DATABASE/AUTHORS.dat");
        rename("DATABASE/TEMP.dat", "DATABASE/AUTHORS.dat");
    }
    else
    {
        remove("DATABASE/TEMP.dat");
        printf("\nAuteur introuvable.\n");
    }
}

void menu_auteurs()
{
    int choix;

    do
    {
        printf("\n========== MENU AUTEURS ==========\n");
        printf("1. Ajouter un auteur\n");
        printf("2. Afficher la liste des auteurs\n");
        printf("3. Afficher les details d'un auteur\n");
        printf("4. Modifier un auteur\n");
        printf("5. Supprimer un auteur\n");
        printf("0. Retour au menu principal\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix)
        {
            case 1: ajout_auteur(); break;
            case 2: afficher_liste_auteurs(); break;
            case 3: rechercher_auteur(); break;
            case 4: modifier_auteur(); break;
            case 5: supprimer_auteur(); break;
            case 0: printf("Retour...\n"); break;
            default: printf("Choix invalide.\n");
        }

    } while (choix != 0);
}

void rechercher_auteur()
{
    FILE *f;
    AUTHORS A;
    int id;
    int trouve = 0;

    f = fopen("DATABASE/AUTHORS.dat", "rb");

    if (f == NULL)
    {
        printf("\nAucun auteur enregistre.\n");
        return;
    }

    afficher_liste_auteurs();

    printf("\nEntrer l'ID de l'auteur a afficher : ");
    scanf("%d", &id);

    while (fread(&A, sizeof(AUTHORS), 1, f) == 1)
    {
        if (A.id == id)
        {
            trouve = 1;
            afficher_auteur(A);
            break;
        }
    }

    if (!trouve)
    {
        printf("\nAuteur introuvable.\n");
    }

    fclose(f);
}
