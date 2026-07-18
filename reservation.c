#include "reservations.h"
#include "rapports.h"
#include <stdio.h>
#include <string.h>
#include<time.h>

char* getdatetime(){
    static char buffer[30];
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    sprintf(buffer, "%02d/%02d/%04d %02d:%02d:%02d",
            tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900,
            tm->tm_hour, tm->tm_min, tm->tm_sec);
    return buffer;
}

char* getdate(){
    static char buffer[20];
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    sprintf(buffer, "%02d/%02d/%04d",
            tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900);
    return buffer;
}

int generer_id_reservation(){
    RESERVATION dernier;
    FILE *f;

    f = fopen("DATABASE/RESERVATIONS.dat", "rb");

    if (f == NULL){
        return 1;
    }

    fseek(f, 0, SEEK_END);

    if (ftell(f) == 0){
        fclose(f);
        return 1;
    }

    fseek(f, -sizeof(RESERVATION), SEEK_END);

    fread(&dernier, sizeof(RESERVATION), 1, f);

    fclose(f);

    return dernier.id + 1;
}

void saisir_reservation(RESERVATION *R){
    int idUtilisateur, idLivre;

    printf("\n========== SAISIE D'UNE RESERVATION ==========\n");

    do{
        printf("ID Utilisateur : ");
        scanf("%d", &idUtilisateur);

        if (!utilisateur_existe(idUtilisateur)){
            printf("Utilisateur inexistant. Veuillez reessayer.\n");
        }
    } while (!utilisateur_existe(idUtilisateur));

    do{
        printf("ID Livre : ");
        scanf("%d", &idLivre);

        if (!livre_existe(idLivre)){
            printf("Livre inexistant. Veuillez reessayer.\n");
        }
        else{
            BOOK* livre = get_livre_par_id(idLivre);
            if (livre->nb_disponible > 0){
                printf("Le livre est disponible (%d exemplaires).\n", livre->nb_disponible);
                printf("Veuillez l'emprunter directement.\n");
                free(livre);
                return;
            }
            free(livre);
        }
    }while(!livre_existe(idLivre));

    R->idUtilisateur = idUtilisateur;
    R->idLivre = idLivre;
    strcpy(R->dateReservation, getdatetime());
    strcpy(R->etat, "EN_ATTENTE");
}

void enregistrer_reservation(RESERVATION R){
    FILE *f;

    f = fopen("DATABASE/RESERVATIONS.dat", "ab");

    if (f == NULL){
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }
    fwrite(&R, sizeof(RESERVATION), 1, f);
    fclose(f);
}

void ajouter_reservation(){
    RESERVATION R;
    int idUtilisateur, idLivre;
    printf("\n========== AJOUT D'UNE RESERVATION ==========\n");
    do{
        printf("ID Utilisateur : ");
        scanf("%d", &idUtilisateur);

        if (!utilisateur_existe(idUtilisateur)){
            printf("Utilisateur inexistant. Veuillez reessayer.\n");
        }
    }while(!utilisateur_existe(idUtilisateur));

    do{
        printf("ID Livre : ");
        scanf("%d", &idLivre);

        if (!livre_existe(idLivre)){
            printf("Livre inexistant. Veuillez reessayer.\n");
        }
        else{
            BOOK* livre = get_livre_par_id(idLivre);
            if (livre->nb_disponible > 0){
                printf("Le livre est disponible (%d exemplaires).\n", livre->nb_disponible);
                printf("Veuillez l'emprunter directement.\n");
                free(livre);
                return;
            }
            free(livre);
        }

        if (reservation_existe(idUtilisateur, idLivre)){
            printf("Vous avez deja reserve ce livre !\n");
        }

    }while(!livre_existe(idLivre) || reservation_existe(idUtilisateur, idLivre));

    R.id = generer_id_reservation();
    R.idUtilisateur = idUtilisateur;
    R.idLivre = idLivre;
    strcpy(R.dateReservation, getdatetime());
    strcpy(R.etat, "EN_ATTENTE");
    enregistrer_reservation(R);
    printf("\nReservation ajoutee avec succes !\n");
    printf("   ID Reservation : %d\n", R.id);
    printf("   Date : %s\n", R.dateReservation);
    printf("   Statut : EN ATTENTE\n");
    char detail[200];
    sprintf(detail, "Reservation du livre ID: %d par utilisateur ID: %d", idLivre, idUtilisateur);
    ecrirehistorique("Reservation", detail);
}

void afficher_reservation(RESERVATION R){
    printf("==========INFORMATIONS DE LA RESERVATION==========\n");
    printf("ID: %d\n", R.id);
    printf("ID Utilisateur: %d\n", R.idUtilisateur);
    printf("ID Livre: %d\n", R.idLivre);
    printf("Date de reservation: %s\n", R.dateReservation);
    printf("Etat: %s\n", R.etat);
}

void afficher_liste_reservations(){
    FILE *f;
    RESERVATION R;
    int compteur = 0;
    f = fopen("DATABASE/RESERVATIONS.dat", "rb");
    if (f == NULL){
        printf("\nAucune reservation enregistree.\n");
        return;
    }

    printf("==========LISTE DES RESERVATIONS==========\n");
    printf("%-5s %-15s %-10s %-20s %-10s\n", "ID", "UTILISATEUR", "LIVRE", "DATE", "ETAT");
    printf("----------------------------------------------------------------------\n");
    rewind(f);
    while (fread(&R, sizeof(RESERVATION), 1, f) == 1){
        printf("%-5d %-15d %-10d %-20s %-10s\n",
               R.id,
               R.idUtilisateur,
               R.idLivre,
               R.dateReservation,
               R.etat);
        compteur++;
    }

    printf("\n");
    printf("Nombre total de reservations : %d\n", compteur);

    fclose(f);
}

void afficher_mes_reservations(){
    FILE *f;
    RESERVATION R;
    int idUtilisateur;
    int trouve = 0;

    printf("\n========== MES RESERVATIONS ==========\n");
    printf("Entrez votre ID : ");
    scanf("%d", &idUtilisateur);

    f = fopen("DATABASE/RESERVATIONS.dat", "rb");

    if (f == NULL){
        printf("\nAucune reservation enregistree.\n");
        return;
    }
    printf("===============MES RESERVATIONS===============\n");
    printf("===========================================\n");

    printf("%-5s %-10s %-20s %-10s\n", "ID", "LIVRE", "DATE", "ETAT");
    printf("-------------------------------------------\n");

    rewind(f);

    while (fread(&R, sizeof(RESERVATION), 1, f) == 1){
        if (R.idUtilisateur == idUtilisateur){
            BOOK* livre = get_livre_par_id(R.idLivre);
            char titre[100] = "Inconnu";
            if (livre != NULL){
                strcpy(titre, livre->titre);
                free(livre);
            }
            printf("%-5d %-10s %-20s %-10s\n",R.id,titre,R.dateReservation,R.etat);
            trouve = 1;
        }
    }

    if (!trouve){
        printf("Aucune reservation trouvee pour cet utilisateur.\n");
    }

    fclose(f);
}

void modifier_reservation(){
    FILE *f;
    RESERVATION R;
    int id, choix;
    int trouve = 0;

    f = fopen("DATABASE/RESERVATIONS.dat", "rb+");
    if (f == NULL){
        printf("Erreur d'ouverture du fichier.\n");
        return;
    }
    afficher_liste_reservations();
    printf("\nEntrer l'ID de la reservation a modifier : ");
    scanf("%d", &id);
    while (fread(&R, sizeof(RESERVATION), 1, f) == 1){
        if (R.id == id){
            trouve = 1;

            printf("\nReservation trouvee :\n");
            afficher_reservation(R);

            printf("\nQue voulez-vous modifier ?\n");
            printf("1. Etat\n");
            printf("2. Tout modifier\n");

            printf("Votre choix : ");
            scanf("%d", &choix);

            switch (choix){
                case 1:
                    printf("Nouvel etat (EN_ATTENTE/DISPONIBLE/ANNULEE) : ");
                    scanf(" %[^\n]", R.etat);
                    break;
                case 2:
                    saisir_reservation(&R);
                    break;
                default:
                    printf("Choix invalide.\n");
                    fclose(f);
                    return;
            }

            fseek(f, -sizeof(RESERVATION), SEEK_CUR);
            fwrite(&R, sizeof(RESERVATION), 1, f);
            printf("\nReservation modifiee avec succes !\n");
            break;
        }
    }

    if (!trouve){
        printf("\nReservation introuvable.\n");
    }
    fclose(f);
}

void annuler_reservation(){
    FILE *f;
    RESERVATION R;
    int id;
    int trouve = 0;
    int confirmation;

    f = fopen("DATABASE/RESERVATIONS.dat", "rb+");
    if (f == NULL){
        printf("Erreur d'ouverture du fichier.\n");
        return;
    }

    afficher_liste_reservations();
    printf("\nEntrer l'ID de la reservation a annuler : ");
    scanf("%d", &id);
    while (fread(&R, sizeof(RESERVATION), 1, f) == 1){
        if (R.id == id){
            afficher_reservation(R);
            if (strcmp(R.etat, "EN_ATTENTE") != 0){
                printf("Cette reservation n'est plus en attente.\n");
                printf("   Etat actuel : %s\n", R.etat);
                fclose(f);
                return;
            }
            printf("\nVoulez-vous vraiment annuler cette reservation ?\n");
            printf("1. Oui\n");
            printf("2. Non\n");
            printf("Votre choix : ");
            scanf("%d", &confirmation);

            if (confirmation == 1){
                strcpy(R.etat, "ANNULEE");
                fseek(f, -sizeof(RESERVATION), SEEK_CUR);
                fwrite(&R, sizeof(RESERVATION), 1, f);
                printf("\nReservation annulee avec succes !\n");
                char detail[100];
                sprintf(detail, "Annulation reservation ID: %d", id);
                ecrirehistorique("Annulation reservation", detail);
                trouve = 1;
            }
            break;
        }
    }

    if (!trouve){
        printf("\nReservation introuvable.\n");
    }

    fclose(f);
}

void supprimer_reservation(){
    FILE *f, *temp;
    RESERVATION R;
    int id;
    int trouve = 0;
    int confirmation;

    f = fopen("DATABASE/RESERVATIONS.dat", "rb");
    temp = fopen("DATABASE/TEMP.dat", "wb");

    if (f == NULL || temp == NULL){
        printf("Erreur d'ouverture des fichiers.\n");
        return;
    }

    afficher_liste_reservations();

    printf("\nEntrer l'ID de la reservation a supprimer : ");
    scanf("%d", &id);

    while (fread(&R, sizeof(RESERVATION), 1, f) == 1){
        if (R.id == id){
            afficher_reservation(R);
            printf("\nVoulez-vous vraiment supprimer cette reservation ?\n");
            printf("1. Oui\n");
            printf("2. Non\n");
            printf("Votre choix : ");
            scanf("%d", &confirmation);

            if (confirmation == 1){
                trouve = 1;
                printf("\nReservation supprimee avec succes !\n");
                continue;
            }
        }

        fwrite(&R, sizeof(RESERVATION), 1, temp);
    }

    fclose(f);
    fclose(temp);

    if (trouve){
        remove("DATABASE/RESERVATIONS.dat");
        rename("DATABASE/TEMP.dat", "DATABASE/RESERVATIONS.dat");
    }
    else{
        remove("DATABASE/TEMP.dat");
        printf("\nReservation introuvable.\n");
    }
}

int reservation_existe(int idUtilisateur, int idLivre){
    FILE *f;
    RESERVATION R;

    f = fopen("DATABASE/RESERVATIONS.dat", "rb");

    if (f == NULL)
        return 0;

    while (fread(&R, sizeof(RESERVATION), 1, f) == 1){
        if (R.idUtilisateur == idUtilisateur && R.idLivre == idLivre && strcmp(R.etat, "EN_ATTENTE") == 0){
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}

void verifier_reservations_apres_retour(int idLivre){
    RESERVATION* r = get_premiere_reservation_en_attente(idLivre);

    if (r == NULL){
        printf("Aucune reservation en attente pour ce livre.\n");
        return;
    }

    FILE *f;
    RESERVATION R;
    int trouve = 0;

    f = fopen("DATABASE/RESERVATIONS.dat", "rb+");
    if (f == NULL){
        free(r);
        return;
    }

    while (fread(&R, sizeof(RESERVATION), 1, f) == 1){
        if (R.id == r->id){
            strcpy(R.etat, "DISPONIBLE");
            fseek(f, -sizeof(RESERVATION), SEEK_CUR);
            fwrite(&R, sizeof(RESERVATION), 1, f);
            printf("Reservation ID %d maintenant DISPONIBLE !\n", r->id);
            char detail[150];
            sprintf(detail, "Reservation ID %d devenue disponible (retour livre ID %d)", r->id, idLivre);
            ecrirehistorique("Reservation disponible", detail);
            trouve = 1;
            break;
        }
    }

    fclose(f);
    free(r);

    if (!trouve){
        printf("Erreur : Reservation introuvable.\n");
    }
}

RESERVATION* get_premiere_reservation_en_attente(int idLivre){
    FILE *f;
    RESERVATION R;
    RESERVATION* premiere = NULL;
    char dateMin[30] = "";

    f = fopen("DATABASE/RESERVATIONS.dat", "rb");

    if (f == NULL){
        return NULL;
    }
    rewind(f);

    while (fread(&R, sizeof(RESERVATION), 1, f) == 1){
        if (R.idLivre == idLivre && strcmp(R.etat, "EN_ATTENTE") == 0){
            if (premiere == NULL || strcmp(R.dateReservation, dateMin) < 0){
                premiere = malloc(sizeof(RESERVATION));
                if (premiere != NULL){
                    *premiere = R;
                    strcpy(dateMin, R.dateReservation);
                }
            }
        }
    }

    fclose(f);
    return premiere;
}

void menu_reservations(){
    int choix;

    do{
        printf("\n========== MENU RESERVATIONS ==========\n");
        printf("1. Ajouter une reservation\n");
        printf("2. Afficher la liste des reservations\n");
        printf("3. Afficher mes reservations\n");
        printf("4. Modifier une reservation\n");
        printf("5. Annuler une reservation\n");
        printf("6. Supprimer une reservation\n");
        printf("0. Retour au menu principal\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix){
            case 1: ajouter_reservation(); break;
            case 2: afficher_liste_reservations(); break;
            case 3: afficher_mes_reservations(); break;
            case 4: modifier_reservation(); break;
            case 5: annuler_reservation(); break;
            case 6: supprimer_reservation(); break;
            case 0: printf("Retour...\n"); break;
            default: printf("Choix invalide.\n");
        }

    } while (choix != 0);
}
