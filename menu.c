#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operation.h"
#include "books.h"
#include "USERS.h"
#include "categories.h"
#include "authors.h"
#include "reservation.h"
#include "statistiques.h"
#include "rapports.h"

void menu_admin()
{
    int choix;
    do
    {
        printf("\nMenu Administrateur :\n");
        printf("1. Gestion des utilisateurs\n");
        printf("2. Gestion des auteurs\n");
        printf("3. Gestion des categories\n");
        printf("4. Gestion des livres\n");
        printf("5. Gestion des emprunts\n");
        printf("6. Gestion des retours\n");
        printf("7. Gestion des reservations\n");
        printf("8. Statistiques\n");
        printf("9. Rapports\n");
        printf("0. Deconnexion\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix)
        {
        case 1:
            menuAdmin();
            break;
        case 2:
            menu_auteurs();
            break;
        case 3:
            menu_categories();
            break;
        case 4:
            menu_livres();
            break;
        case 5:
            menuEmprunts();
            break;
        case 6:
            menuRetours();
            break;
        case 7:
            menu_reservations();
            break;
        case 8:
            menu_statistiques();
            break;
        case 9:
            menu_rapports();
            break;
        case 0:
            printf("Deconnexion...\n");
            break;
        default:
            printf("Choix invalide.\n");
        }
    } while (choix != 0);
}

void menu_user()
{
    int choix;
    do
    {
        printf("\nMenu Utilisateur :\n");
        printf("1. voir les livres\n");
        printf("2. Voir les auteurs\n");
        printf("3. Voir mes reservations\n");
        printf("4. Menu Penalites\n");
        printf("5. Menu Retours\n");
        printf("6. Menu categories\n");
        printf("0. Deconnexion\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix)
        {
        case 1:
            afficher_liste_livres();
            break;
        case 2:
            afficher_liste_auteurs();
            break;
        case 3:
            menu_reservations();
            break;
        case 4:
           menuPenalites();
            break;
        case 5:
            menuRetours();
            break;
        case 6:
            menu_categories();
            break;
        case 0:
            printf("Deconnexion...\n");
            break;
        default:
            printf("Choix invalide.\n");
        }
    } while (choix != 0);
}

void menu_principal_bibliotheque()
{
    int idConnexion = 0;
    int utilisateurConnecte = 0;
    int role = 0;

    do
    {
        if (!utilisateurConnecte)
        {
            idConnexion = connexion();
            if (idConnexion != 0)
            {
                utilisateurConnecte = 1;
                role = verifier_role_utilisateur(idConnexion);
                if (role == 1)
                {
                    menu_admin();
                }
                else if (role == 2)
                {
                    menu_user();
                }
                else
                {
                    printf("\nAcces refuse.\n");
                    utilisateurConnecte = 0;
                }
            }
            else
            {
                printf("\nEchec de connexion.\n");
            }
        }
    } while (!utilisateurConnecte);
}

void menu_principal() {
    int choix;
    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. inscription\n");
        printf("2. Connexion\n");
        printf("0. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                ajouterUtilisateur();
                break;
            case 2:
                menu_principal_bibliotheque();
                break;  
            case 0:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide !\n");
        }
    } while (choix != 0);
    return 0;
}