#include <stdio.h>
#include <string.h>
#include "books.h"
int generer_id_livre()
{
    BOOK dernier;
    FILE *f;

    f = fopen("DATABASE/BOOKS.dat", "rb");

    if (f == NULL)
        return 1;

    fseek(f, 0, SEEK_END);

    if (ftell(f) == 0)
    {
        fclose(f);
        return 1;
    }

    fseek(f, -sizeof(BOOK), SEEK_END);

    fread(&dernier, sizeof(BOOK), 1, f);

    fclose(f);

    return dernier.id + 1;
}

int isbn_existe(char isbn[])
{
    FILE *f;
    BOOK L;

    f = fopen("DATABASE/BOOKS.dat", "rb");

    if (f == NULL)
        return 0;

    while (fread(&L, sizeof(BOOK), 1, f) == 1)
    {
        if (strcmp(L.isbn, isbn) == 0)
        {
            fclose(f);
            return 1;
        }
    }

    fclose(f);

    return 0;
}

int auteur_existe(int id)
{
    FILE *f;
    AUTHORS A;

    f = fopen("DATABASE/AUTHORS.dat", "rb");

    if (f == NULL)
        return 0;

    while (fread(&A, sizeof(AUTHORS), 1, f) == 1)
    {
        if (A.id == id)
        {
            fclose(f);
            return 1;
        }
    }

    fclose(f);

    return 0;
}

int categorie_existe(int id)
{
    FILE *f;
    CATEGORY C;

    f = fopen("DATABASE/CATEGORIES.dat", "rb");

    if (f == NULL)
        return 0;

    while (fread(&C, sizeof(CATEGORY), 1, f) == 1)
    {
        if (C.id_categorie == id)
        {
            fclose(f);
            return 1;
        }
    }

    fclose(f);

    return 0;
}

void saisir_livre(BOOK *L)
{
    int existe;

    printf("\n========== SAISIE D'UN LIVRE ==========\n");

    printf("ISBN : ");
    scanf("%19s", L->isbn);

    while (isbn_existe(L->isbn))
    {
        printf("\nErreur : cet ISBN existe deja.\n");
        printf("Veuillez saisir un autre ISBN : ");
        scanf("%19s", L->isbn);
    }

    printf("Titre : ");
    scanf(" %[^\n]", L->titre);

    do
    {
        afficher_liste_auteurs();

        printf("\nEntrer l'ID de l'auteur : ");
        scanf("%d", &L->id_auteur);

        existe = auteur_existe(L->id_auteur);

        if (!existe)
        {
            printf("\nAuteur introuvable. Veuillez reessayer.\n");
        }

    } while (!existe);

    do
    {
        afficher_liste_categorie();

        printf("\nEntrer l'ID de la categorie : ");
        scanf("%d", &L->id_categorie);

        existe = categorie_existe(L->id_categorie);

        if (!existe)
        {
            printf("\nCategorie introuvable. Veuillez reessayer.\n");
        }

    } while (!existe);

    printf("Editeur : ");
    scanf(" %[^\n]", L->editeur);

    do
    {
        printf("Annee de publication : ");
        scanf("%d", &L->annee_publication);

        if (L->annee_publication <= 0)
        {
            printf("Annee invalide.\n");
        }

    } while (L->annee_publication <= 0);

    printf("Langue : ");
    scanf(" %[^\n]", L->langue);

    do
    {
        printf("Nombre total d'exemplaires : ");
        scanf("%d", &L->nb_total_exemplaires);

        if (L->nb_total_exemplaires <= 0)
        {
            printf("Le nombre doit etre superieur a 0.\n");
        }

    } while (L->nb_total_exemplaires <= 0);

    L->nb_disponible = L->nb_total_exemplaires;

    do
{
    printf("Nombre de pages : ");
    scanf("%d", &L->nb_pages);

    if (L->nb_pages <= 0)
        printf("Le nombre de pages doit etre superieur a 0.\n");

} while (L->nb_pages <= 0);

    printf("Emplacement dans la bibliotheque : ");
    scanf(" %[^\n]", L->emplacement);

    printf("Resume : ");
    scanf(" %[^\n]", L->resume);


    printf("Date d'ajout (jj/mm/aaaa) : ");
    scanf("%10s", L->date_ajout);
}

void enregistrer_livre(BOOK L)
{
    FILE *f;

    f = fopen("DATABASE/BOOKS.dat", "ab");

    if (f == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    fwrite(&L, sizeof(BOOK), 1, f);

    fclose(f);
}

void ajout_livre()
{
    BOOK L;
    FILE *f;
    AUTHORS A_test;
    CATEGORY C_test;

    f = fopen("DATABASE/AUTHORS.dat", "rb");
    if (f == NULL || fread(&A_test, sizeof(AUTHORS), 1, f) != 1)
    {
        printf("\nAucun auteur enregistre. Veuillez d'abord ajouter un auteur.\n");
        if (f != NULL) fclose(f);
        return;
    }
    fclose(f);

    f = fopen("DATABASE/CATEGORIES.dat", "rb");
    if (f == NULL || fread(&C_test, sizeof(CATEGORY), 1, f) != 1)
    {
        printf("\nAucune categorie enregistree. Veuillez d'abord ajouter une categorie.\n");
        if (f != NULL) fclose(f);
        return;
    }
    fclose(f);

    L.id = generer_id_livre();
    saisir_livre(&L);
    enregistrer_livre(L);

    printf("\nLivre ajoute avec succes !\n");
}

void afficher_livre(BOOK L)
{
    printf("\n====================================================\n");
    printf("               INFORMATIONS DU LIVRE\n");
    printf("====================================================\n");

    printf("ID                         : %d\n", L.id);
    printf("ISBN                       : %s\n", L.isbn);
    printf("Titre                      : %s\n", L.titre);
    printf("ID Auteur                  : %d\n", L.id_auteur);
    printf("ID Categorie               : %d\n", L.id_categorie);
    printf("Editeur                    : %s\n", L.editeur);
    printf("Annee de publication       : %d\n", L.annee_publication);
    printf("Langue                     : %s\n", L.langue);
    printf("Nombre total               : %d\n", L.nb_total_exemplaires);
    printf("Nombre disponible          : %d\n", L.nb_disponible);
    printf("Emplacement                : %s\n", L.emplacement);
    printf("Date d'ajout               : %s\n", L.date_ajout);

    printf("\nResume :\n");
    printf("----------------------------------------------------\n");
    printf("%s\n", L.resume);
    printf("----------------------------------------------------\n");
}

void afficher_liste_livres()
{
    FILE *f;
    BOOK L;
    int compteur = 0;

    f = fopen("DATABASE/BOOKS.dat", "rb");

    if (f == NULL)
    {
        printf("\nAucun livre enregistre.\n");
        return;
    }

    printf("\n=====================================================================\n");
    printf("                        LISTE DES LIVRES\n");
    printf("=====================================================================\n");

    printf("%-5s %-20s %-20s\n", "ID", "ISBN", "TITRE");
    printf("---------------------------------------------------------------------\n");

    rewind(f);

    while (fread(&L, sizeof(BOOK), 1, f) == 1)
    {
        printf("%-5d %-20s %-20s\n",
               L.id,
               L.isbn,
               L.titre);

        compteur++;
    }

    printf("---------------------------------------------------------------------\n");
    printf("Nombre total de livres : %d\n", compteur);

    fclose(f);
}

void modifier_livre()
{
    FILE *f;
    BOOK L;
    int id, choix;
    int trouve = 0;

    f = fopen("DATABASE/BOOKS.dat", "rb+");

    if (f == NULL)
    {
        printf("Erreur d'ouverture du fichier.\n");
        return;
    }

    afficher_liste_livres();

    printf("\nEntrer l'ID du livre a modifier : ");
    scanf("%d", &id);

    while (fread(&L, sizeof(BOOK), 1, f) == 1)
    {
        if (L.id == id)
        {
            trouve = 1;

            printf("\nLivre trouve :\n");
            afficher_livre(L);

            printf("\nQue voulez-vous modifier ?\n");
            printf("1. ISBN\n");
            printf("2. Titre\n");
            printf("3. Auteur\n");
            printf("4. Categorie\n");
            printf("5. Editeur\n");
            printf("6. Annee de publication\n");
            printf("7. Langue\n");
            printf("8. Nombre total d'exemplaires\n");
            printf("9. Nombre de pages\n");
            printf("10. Emplacement\n");
            printf("11. Resume\n");
            printf("12. Tout modifier\n");

            printf("\nVotre choix : ");
            scanf("%d", &choix);

            switch (choix)
            {
                case 1:
                {
                    char nouvel_isbn[20];

                    do
                    {
                        printf("Nouvel ISBN : ");
                        scanf("%s", nouvel_isbn);

                        if (strcmp(nouvel_isbn, L.isbn) == 0)
                            break;

                    } while (isbn_existe(nouvel_isbn));

                    strcpy(L.isbn, nouvel_isbn);
                    break;
                }

                case 2:
                    printf("Nouveau titre : ");
                    scanf(" %[^\n]", L.titre);
                    break;

                case 3:
                {
                    int idAuteur;

                    do
                    {
                        afficher_liste_auteurs();
                        printf("Nouvel ID Auteur : ");
                        scanf("%d", &idAuteur);

                    } while (!auteur_existe(idAuteur));

                    L.id_auteur = idAuteur;
                    break;
                }

                case 4:
                {
                    int idCategorie;

                    do
                    {
                        afficher_liste_categorie();
                        printf("Nouvel ID Categorie : ");
                        scanf("%d", &idCategorie);

                    } while (!categorie_existe(idCategorie));

                    L.id_categorie = idCategorie;
                    break;
                }

                case 5:
                    printf("Nouvel editeur : ");
                    scanf(" %[^\n]", L.editeur);
                    break;

                case 6:
                    printf("Nouvelle annee : ");
                    scanf("%d", &L.annee_publication);
                    break;

                case 7:
                    printf("Nouvelle langue : ");
                    scanf(" %[^\n]", L.langue);
                    break;

                case 8:
                    printf("Nouveau nombre total d'exemplaires : ");
                    scanf("%d", &L.nb_total_exemplaires);

                    if(L.nb_disponible > L.nb_total_exemplaires)
                        L.nb_disponible = L.nb_total_exemplaires;

                    break;

                case 9:
                    do
                      {
                        printf("Nombre de pages : ");
                        scanf("%d", &L.nb_pages);

                        if (L.nb_pages <= 0)
                        printf("Le nombre de pages doit etre superieur a 0.\n");

                        } while (L.nb_pages <= 0);
                      break;
                case 10:
                    printf("Nouvel emplacement : ");
                    scanf(" %[^\n]", L.emplacement);
                    break;

                case 11:
                    printf("Nouveau resume : ");
                    scanf(" %[^\n]", L.resume);
                    break;

                case 12:
                    saisir_livre(&L);
                    break;

                default:
                    printf("Choix invalide.\n");
                    fclose(f);
                    return;
            }

            fseek(f, -sizeof(BOOK), SEEK_CUR);

            fwrite(&L, sizeof(BOOK), 1, f);

            printf("\nLivre modifie avec succes !\n");

            break;
        }
    }

    if (!trouve)
    {
        printf("\nLivre introuvable.\n");
    }

    fclose(f);
}

void supprimer_livre()
{
    FILE *f, *temp;
    BOOK L;
    int id;
    int trouve = 0;
    int confirmation;

    f = fopen("DATABASE/BOOKS.dat", "rb");
    temp = fopen("DATABASE/TEMP.dat", "wb");

    if (f == NULL || temp == NULL)
    {
        printf("Erreur d'ouverture des fichiers.\n");
        return;
    }

    afficher_liste_livres();

    printf("\nEntrer l'ID du livre a supprimer : ");
    scanf("%d", &id);

    while (fread(&L, sizeof(BOOK), 1, f) == 1)
    {
        if (L.id == id)
        {
            afficher_livre(L);

            printf("\nVoulez-vous vraiment supprimer ce livre ?\n");
            printf("1. Oui\n");
            printf("2. Non\n");
            printf("Votre choix : ");
            scanf("%d", &confirmation);

            if (confirmation == 1)
            {
                trouve = 1;
                printf("\nLivre supprime avec succes !\n");
                continue;       // on ne recopie pas ce livre
            }
        }

        fwrite(&L, sizeof(BOOK), 1, temp);
    }

    fclose(f);
    fclose(temp);

    if (trouve)
    {
        remove("DATABASE/BOOKS.dat");
        rename("DATABASE/TEMP.dat", "DATABASE/BOOKS.dat");
    }
    else
    {
        remove("DATABASE/TEMP.dat");
        printf("\nLivre introuvable.\n");
    }
}

void menu_livres()
{
    int choix;

    do
    {
        printf("\n========== MENU LIVRES ==========\n");
        printf("1. Ajouter un livre\n");
        printf("2. Afficher la liste des livres\n");
        printf("3. Afficher les details d'un livre\n");   // <-- nouvelle ligne
        printf("4. Modifier un livre\n");
        printf("5. Supprimer un livre\n");
        printf("0. Retour au menu principal\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix)
        {
            case 1: ajout_livre(); break;
            case 2: afficher_liste_livres(); break;
            case 3: rechercher_livre(); break;   // <-- nouvelle ligne
            case 4: modifier_livre(); break;
            case 5: supprimer_livre(); break;
            case 0: printf("Retour...\n"); break;
            default: printf("Choix invalide.\n");
        }

    } while (choix != 0);
}

int auteur_a_des_livres(int idAuteur)
{
    FILE *f;
    BOOK L;

    f = fopen("DATABASE/BOOKS.dat", "rb");

    if (f == NULL)
        return 0;

    while (fread(&L, sizeof(BOOK), 1, f) == 1)
    {
        if (L.id_auteur == idAuteur)
        {
            fclose(f);
            return 1;
        }
    }

    fclose(f);

    return 0;
}

void rechercher_livre()
{
    FILE *f;
    BOOK L;
    int id;
    int trouve = 0;

    f = fopen("DATABASE/BOOKS.dat", "rb");

    if (f == NULL)
    {
        printf("\nAucun livre enregistre.\n");
        return;
    }

    afficher_liste_livres();

    printf("\nEntrer l'ID du livre a afficher : ");
    scanf("%d", &id);

    while (fread(&L, sizeof(BOOK), 1, f) == 1)
    {
        if (L.id == id)
        {
            trouve = 1;
            afficher_livre(L);
            break;
        }
    }

    if (!trouve)
    {
        printf("\nLivre introuvable.\n");
    }

    fclose(f);
}
