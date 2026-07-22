#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "USERS.h"
#include "authors.h"
#include "books.h"
#include "categories.h"
#include "reservation.h"
#include "statistiques.h"
#include "rapports.h"
#include "menu.h"


int main() {
    int choix;
    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. ajouter un utilisateur\n");
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

