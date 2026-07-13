#include <stdio.h>
typedef struct
{
    int id_categorie;
    char date_creation[11];
    char description[100];
    char libelle[50];
} CATEGORY;

int generer_id_categorie()
{
    CATEGORY dernier;
    FILE *f;

    f = fopen("DATABASE/CATEGORIES.dat", "rb");

    if (f == NULL)
        return 1;

    fseek(f, 0, SEEK_END);

    if (ftell(f) == 0)
    {
        fclose(f);
        return 1;
    }

    fseek(f, -sizeof(CATEGORY), SEEK_END);

    fread(&dernier, sizeof(CATEGORY), 1, f);

    fclose(f);

    return dernier.id_categorie + 1;
}

void saisir_categorie(CATEGORY *C)
{
    printf("\n========== SAISIE D'UNE CATEGORIE ==========\n");

    printf("Libelle: ");
    scanf(" %[^\n]", C->libelle);

    printf("Description: ");
    scanf(" %[^\n]", C->description);

    printf("Date de creation(jj/mm/aaaa) : ");
    scanf("%10s", C->date_creation);
}

void afficher_categorie(CATEGORY C)
{
    printf("\n=========================\n");
    printf("ID : %d\n",C.id);
    printf("Libelle : %s\n",C.libelle);
    printf("description : %s\n",C.description);
    printf("Date de creation de la categorie : %s\n",C.date_creation);
    printf("=========================\n");
}

void enregistrer_categorie(CATEGORY C)
{
    FILE *f;

    f = fopen("DATABASE/CATEGORIES.dat", "ab");

    if (f == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    fwrite(&C, sizeof(CATEGORY), 1, f);

    fclose(f);
}

void ajout_categorie()
{
   CATEGORY C;

    C.id_categorie = generer_id_categorie();

    saisir_categorie(&C);

    enregistrer_categorie(C);

    printf("\nCategorie ajoutee avec succes !\n");
}

void afficher_auteur(CATEGORY C)
{
    printf("\n=========================================\n");
    printf("       INFORMATIONS DE LA CATEGORIE\n");
    printf("=========================================\n");
    printf("ID                 : %d\n", C.id_categorie);
    printf("Libelle            : %s\n", C.libelle);
    printf("\nDescription :\n");
    printf("-----------------------------------------\n");
    printf("%s\n", C.description);
    printf("-----------------------------------------\n");
    printf("Date de creation   : %s\n", C.date_creation);
}

void afficher_liste_categorie()
{
    FILE *f;
    CATEGORY C;
    int compteur = 0;

    f = fopen("DATABASE/CATEGORIES.dat", "rb");

    if (f == NULL)
    {
        printf("\nAucune categorie enregistree.\n");
        return;
    }

    printf("\n===========================================\n");
    printf("           LISTE DES CATEGORIES\n");
    printf("===========================================\n");

    printf("%-5s %-30s\n", "ID", "LIBELLE");
    printf("-------------------------------------------\n");

    rewind(f);

    while (fread(&C, sizeof(CATEGORY), 1, f) == 1)
    {
        printf("%-5d %-30s\n", C.id_categorie, C.libelle);
        compteur++;
    }

    printf("-------------------------------------------\n");
    printf("Nombre total de categories: %d\n", compteur);

    fclose(f);
}

void modifier_categorie()
{
    FILE *f;
    CATEGORY C;
    int id, choix;
    int trouve = 0;

    f = fopen("DATABASE/CATEGORIES.dat", "rb+");

    if (f == NULL)
    {
        printf("Erreur d'ouverture du fichier.\n");
        return;
    }

    printf("\nEntrer l'ID de la categorie a modifier : ");
    scanf("%d", &id);

    while (fread(&C, sizeof(CATEGORY), 1, f) == 1)
    {
        if (C.id_categorie == id)
        {
            trouve = 1;

            printf("\nCategorie trouvee :\n");
            afficher_categorie(C);

            printf("\nQue voulez-vous modifier ?\n");
            printf("1. Libelle\n");
            printf("2. Description\n");
            printf("3. Tout modifier\n");

            printf("Votre choix : ");
            scanf("%d", &choix);

            switch (choix)
            {
                case 1:
                    printf("Nouveau Libelle: ");
                    scanf(" %[^\n]", C.libelle);
                    break;

                case 2:
                    printf("Nouvelle description : ");
                    scanf(" %[^\n]", C.description);
                    break;

                case 3:
                    saisir_categorie(&C);
                    break;

                default:
                    printf("Choix invalide.\n");
                    fclose(f);
                    return;
            }

            fseek(f, -sizeof(CATEGORY), SEEK_CUR);

            fwrite(&C, sizeof(CATEGORY), 1, f);

            printf("\nCategorie modifiee avec succes !\n");

            break;
        }
    }

    if (!trouve)
    {
        printf("\nCategorie introuvable.\n");
    }

    fclose(f);
}

void supprimer_categorie()
{
    FILE *f, *temp;
    CATEGORY C;
    int id;
    int trouve = 0;
    int confirmation;

    f = fopen("DATABASE/CATEGORIES.dat", "rb");
    temp = fopen("DATABASE/TEMP.dat", "wb");

    if (f == NULL || temp == NULL)
    {
        printf("Erreur d'ouverture des fichiers.\n");
        return;
    }

    printf("\nEntrer l'ID de la categorie a supprimer : ");
    scanf("%d", &id);

    while (fread(&C, sizeof(CATEGORY), 1, f) == 1)
    {
        if (C.id_categorie == id)
        {
            afficher_categorie(C);

            printf("\nVoulez-vous vraiment supprimer cette categorie ?\n");
            printf("1. Oui\n");
            printf("2. Non\n");
            printf("Votre choix : ");
            scanf("%d", &confirmation);

            if (confirmation == 1)
            {
                trouve = 1;
                printf("\nCategorie supprimee avec succes.\n");
                continue;
            }
        }

        fwrite(&C, sizeof(CATEGORY), 1, temp);
    }

    fclose(f);
    fclose(temp);

    if (trouve)
    {
        remove("DATABASE/CATEGORIES.dat");
        rename("DATABASE/TEMP.dat", "DATABASE/CATEGORIES.dat");
    }
    else
    {
        remove("DATABASE/TEMP.dat");
        printf("\ncategorie introuvable.\n");
    }
}
