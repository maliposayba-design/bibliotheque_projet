#include <stdio.h>
<<<<<<< HEAD
#include "authors.h"
#include "categories.h"
#include "books.h"

int main()
{
    int choix;

    do
    {
        printf("\n=====================================\n");
        printf("     GESTION DE BIBLIOTHEQUE\n");
        printf("=====================================\n");
        printf("1. Gerer les auteurs\n");
        printf("2. Gerer les categories\n");
        printf("3. Gerer les livres\n");
        printf("0. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix)
        {
            case 1: menu_auteurs(); break;
            case 2: menu_categories(); break;
            case 3: menu_livres(); break;
            case 0: printf("\nAu revoir !\n"); break;
            default: printf("Choix invalide.\n");
        }

    } while (choix != 0);
=======
#include <stdlib.h>
#include "USERS.h"
int main()
{
    printf("Hello world!\n");
     //ajouterUtilisateur();
    afficherUtilisateurs();
    supprimer_utilisateur();
    //connexion();
>>>>>>> laye

    return 0;
}
