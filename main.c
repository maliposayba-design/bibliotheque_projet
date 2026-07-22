#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "USERS.h"
<<<<<<< HEAD
int main()
{
    printf("Hello world!\n");
     //ajouterUtilisateur();
    afficherUtilisateurs();
    supprimer_utilisateur();
    //connexion();
=======
#include "authors.h"
#include "books.h"
#include "categories.h"
#include "reservation.h"
#include "statistiques.h"
#include "rapports.h"

void afficher_menu_principal() {
    printf("==================BIBLIOTHEQUE E-LIBRARY ISI DAKAR==================\n");
    printf("1. Gestion des utilisateurs\n");
    printf("2. Gestion des auteurs\n");
    printf("3. Gestion des categories\n");
    printf("4. Gestion des livres\n");
    printf("5. Gestion des emprunts\n");
    printf("6. Gestion des retours\n");
    printf("7. Gestion des reservations\n");
    printf("8. Statistiques\n");
    printf("9. Rapports\n");
    printf("0. Quitter\n");
    printf("Votre choix : ");
}

int main() {
    system("title Bibliotheque E-LIBRARY ISI DAKAR");

    printf("========================BIENVENUE A LA BIBLIOTHEQUE E-LIBRARY ISI========================\n");
    printf("\n");

    int choix;
    do {
        afficher_menu_principal();
        printf("entrez votre choix : ");
        scanf("%d", &choix);
        switch(choix) {
            case 1:
                printf("\n[Module Utilisateurs] En developpement...\n");
                break;

            case 2:
                printf("\n[Module Auteurs] En developpement...\n");
                break;

            case 3:
                printf("\n[Module Categories] En developpement...\n");
                break;

            case 4:
                printf("\n[Module Livres] En developpement...\n");
                break;

            case 5:
                printf("\n[Module Emprunts] En developpement...\n");
                break;

            case 6:
                printf("\n[Module Retours] En developpement...\n");
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
                printf("\nAu revoir et merci d'avoir utilise la bibliotheque !\n");
                break;

            default:
                printf("\nChoix invalide ! Veuillez reessayer.\n");
        }
    } while (choix != 0);
>>>>>>> feature/rapport

    return 0;
}
