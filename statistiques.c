#include "statistiques.h"
#include "reservation.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void* lire_fichier_stats(const char* nom, size_t taille, int* nb) {
    FILE* f = fopen(nom, "rb");
    if (f == NULL){
        *nb = 0;
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long tailleFichier = ftell(f);
    *nb = tailleFichier / taille;
    rewind(f);

    void* data = malloc(tailleFichier);
    if (data == NULL) {
        fclose(f);
        *nb = 0;
        return NULL;
    }

    fread(data, taille, *nb, f);
    fclose(f);
    return data;
}

int compter_utilisateurs() {
    int nb;
    User* users = lire_fichier_stats("DATABASE/USERS.dat", sizeof(User), &nb);
    if (users == NULL){
       return 0;
    }
    free(users);
    return nb;
}

int compter_livres() {
    int nb;
    BOOK* livres = lire_fichier_stats("DATABASE/BOOKS.dat", sizeof(BOOK), &nb);
    if (livres == NULL) return 0;
    free(livres);
    return nb;
}

int compter_emprunts_en_cours() {
    FILE *f;
    int count = 0;
    struct {
        int id;
        char numEmprunt[30];
        int idUtilisateur;
        int idLivre;
        char dateEmprunt[20];
        char dateRetourPrevue[20];
        char etat[20];
    } E;

    f = fopen("DATABASE/BORROWS.dat", "rb");
    if (f == NULL) return 0;

    while (fread(&E, sizeof(E), 1, f) == 1) {
        if (strcmp(E.etat, "EN_COURS") == 0) {
            count++;
        }
    }
    fclose(f);
    return count;
}

int compter_livres_disponibles() {
    int nb;
    BOOK* livres = lire_fichier_stats("DATABASE/BOOKS.dat", sizeof(BOOK), &nb);
    if (livres == NULL){
       return 0;
    }

    int count = 0;
    for (int i = 0; i < nb; i++) {
        if (livres[i].nb_disponible > 0) {
            count++;
        }
    }
    free(livres);
    return count;
}

int compter_reservations_totales() {
    int nb;
    RESERVATION R
    FILE* f = fopen("DATABASE/RESERVATIONS.dat", "rb");
    if (f == NULL) return 0;

    int count = 0;
    while (fread(&R, sizeof(RESERVATION), 1, f) == 1) {
        count++;
    }
    fclose(f);
    return count;
}

int compter_penalites_impayees() {
    FILE *f;
    int count = 0;
    struct {
        int id;
        int idUtilisateur;
        int idEmprunt;
        float montant;
        char dateCalcul[20];
        int payee;
    } P;

    f = fopen("DATABASE/PENALTIES.dat", "rb");
    if (f == NULL) return 0;

    while (fread(&P, sizeof(P), 1, f) == 1) {
        if (P.payee == 0) {
            count++;
        }
    }
    fclose(f);
    return count;
}

int compter_emprunts_du_jour() {
    char today[20];
    strcpy(today, getdate());

    FILE *f;
    int count = 0;
    struct {
        int id;
        char numEmprunt[30];
        int idUtilisateur;
        int idLivre;
        char dateEmprunt[20];
        char dateRetourPrevue[20];
        char etat[20];
    } E;

    f = fopen("DATABASE/BORROWS.dat", "rb");
    if (f == NULL) return 0;

    while (fread(&E, sizeof(E), 1, f) == 1) {
        if (strncmp(E.dateEmprunt, today, 10) == 0) {
            count++;
        }
    }
    fclose(f);
    return count;
}

int compter_retours_du_jour() {
    char today[20];
    strcpy(today, getdate());

    FILE *f;
    int count = 0;
    struct {
        int id;
        int idEmprunt;
        char dateRetourEffective[20];
        int enRetard;
        float montantPenalite;
    } R;

    f = fopen("DATABASE/RETURNS.dat", "rb");
    if (f == NULL) return 0;

    while (fread(&R, sizeof(R), 1, f) == 1) {
        if (strncmp(R.dateRetourEffective, today, 10) == 0) {
            count++;
        }
    }
    fclose(f);
    return count;
}

int compter_reservations_du_jour() {
    char today[20];
    strcpy(today, getdate());

    FILE *f;
    int count = 0;
    RESERVATION R;

    f = fopen("DATABASE/RESERVATIONS.dat", "rb");
    if (f == NULL) return 0;

    while (fread(&R, sizeof(RESERVATION), 1, f) == 1) {
        if (strncmp(R.dateReservation, today, 10) == 0) {
            count++;
        }
    }
    fclose(f);
    return count;
}

int compter_nouveaux_utilisateurs_du_jour() {
    char today[20];
    strcpy(today, getdate());

    int nb;
    User* users = lire_fichier_stats("DATABASE/USERS.dat", sizeof(User), &nb);
    if (users == NULL) return 0;

    int count = 0;
    for (int i = 0; i < nb; i++) {
        if (strncmp(users[i].dateCreation, today, 10) == 0) {
            count++;
        }
    }
    free(users);
    return count;
}

int compter_penalites_du_jour() {
    char today[20];
    strcpy(today, getdate());

    FILE *f;
    int count = 0;
    struct {
        int id;
        int idUtilisateur;
        int idEmprunt;
        float montant;
        char dateCalcul[20];
        int payee;
    } P;

    f = fopen("DATABASE/PENALTIES.dat", "rb");
    if (f == NULL) return 0;

    while (fread(&P, sizeof(P), 1, f) == 1) {
        if (strncmp(P.dateCalcul, today, 10) == 0) {
            count++;
        }
    }
    fclose(f);
    return count;
}

float montant_penalites_du_jour() {
    char today[20];
    strcpy(today, getdate());

    FILE *f;
    float total = 0;
    struct {
        int id;
        int idUtilisateur;
        int idEmprunt;
        float montant;
        char dateCalcul[20];
        int payee;
    } P;

    f = fopen("DATABASE/PENALTIES.dat", "rb");
    if (f == NULL) return 0.0;

    while (fread(&P, sizeof(P), 1, f) == 1) {
        if (strncmp(P.dateCalcul, today, 10) == 0) {
            total += P.montant;
        }
    }
    fclose(f);
    return total;
}

char* get_livre_le_plus_emprunte() {
    static char result[200] = "Aucun emprunt";

    FILE *f;
    int nbLivres;
    BOOK* livres = lire_fichier_stats("DATABASE/BOOKS.dat", sizeof(BOOK), &nbLivres);
    if (livres == NULL) return result;

    int* compteurs = calloc(nbLivres + 1, sizeof(int));
    if (compteurs == NULL) {
        free(livres);
        return result;
    }

    struct {
        int id;
        char numEmprunt[30];
        int idUtilisateur;
        int idLivre;
        char dateEmprunt[20];
        char dateRetourPrevue[20];
        char etat[20];
    } E;

    f = fopen("DATABASE/BORROWS.dat", "rb");
    if (f != NULL) {
        while (fread(&E, sizeof(E), 1, f) == 1) {
            for (int j = 0; j < nbLivres; j++) {
                if (livres[j].id == E.idLivre) {
                    compteurs[j]++;
                    break;
                }
            }
        }
        fclose(f);
    }

    int max = 0, idMax = 0;
    for (int i = 0; i < nbLivres; i++) {
        if (compteurs[i] > max) {
            max = compteurs[i];
            idMax = livres[i].id;
        }
    }

    free(compteurs);

    if (max == 0) {
        free(livres);
        return result;
    }

    for (int i = 0; i < nbLivres; i++) {
        if (livres[i].id == idMax) {
            sprintf(result, "%s (%d emprunts)", livres[i].titre, max);
            break;
        }
    }

    free(livres);
    return result;
}

char* get_utilisateur_le_plus_actif() {
    static char result[200] = "Aucun emprunt";

    int nbUsers;
    User* users = lire_fichier_stats("DATABASE/USERS.dat", sizeof(User), &nbUsers);
    if (users == NULL) return result;

    int* compteurs = calloc(nbUsers + 1, sizeof(int));
    if (compteurs == NULL) {
        free(users);
        return result;
    }

    struct {
        int id;
        char numEmprunt[30];
        int idUtilisateur;
        int idLivre;
        char dateEmprunt[20];
        char dateRetourPrevue[20];
        char etat[20];
    } E;

    FILE* f = fopen("DATABASE/BORROWS.dat", "rb");
    if (f != NULL) {
        while (fread(&E, sizeof(E), 1, f) == 1){
            for (int j = 0; j < nbUsers; j++){
                if (users[j].id == E.idUtilisateur) {
                    compteurs[j]++;
                    break;
                }
            }
        }
        fclose(f);
    }

    int max = 0, idMax = 0;
    for (int i = 0; i < nbUsers; i++){
        if (compteurs[i] > max) {
            max = compteurs[i];
            idMax = users[i].id;
        }
    }

    free(compteurs);

    if (max == 0) {
        free(users);
        return result;
    }

    for (int i = 0; i < nbUsers; i++) {
        if (users[i].id == idMax) {
            sprintf(result, "%s %s (%d emprunts)", users[i].prenom, users[i].nom, max);
            break;
        }
    }

    free(users);
    return result;
}

float get_montant_total_penalites() {
    FILE *f;
    float total = 0;
    struct {
        int id;
        int idUtilisateur;
        int idEmprunt;
        float montant;
        char dateCalcul[20];
        int payee;
    } P;

    f = fopen("DATABASE/PENALTIES.dat", "rb");
    if (f == NULL) return 0.0;

    while (fread(&P, sizeof(P), 1, f) == 1) {
        total += P.montant;
    }
    fclose(f);
    return total;
}

void afficher_statistiques_generales() {
    printf("===================STATISTIQUES GENERALES===================\n");
    printf("Total utilisateurs    : %d\n", compter_utilisateurs());
    printf("Total livres          : %d\n", compter_livres());
    printf("Emprunts en cours     : %d\n", compter_emprunts_en_cours());
    printf("Livres disponibles    : %d\n", compter_livres_disponibles());
    printf("Reservations totales  : %d\n", compter_reservations_totales());
    printf("Penalites impayees    : %d\n", compter_penalites_impayees());
}

void afficher_statistiques_journalieres() {
    printf("==================STATISTIQUES DU JOUR==================\n");
    printf("Date                 : %s\n", getdate());
    printf("Emprunts             : %d\n", compter_emprunts_du_jour());
    printf("Retours              : %d\n", compter_retours_du_jour());
    printf("Reservations         : %d\n", compter_reservations_du_jour());
    printf("Nouveaux utilisateurs: %d\n", compter_nouveaux_utilisateurs_du_jour());
    printf("Penalites            : %d\n", compter_penalites_du_jour());
    printf("Montant penalites    : %.2f FCFA\n", montant_penalites_du_jour());
    printf("============================================\n");
}

void afficher_classements() {
    printf("=====================CLASSEMENTS=====================\n");
    printf("Livre le plus emprunte  : %s\n", get_livre_le_plus_emprunte());
    printf("Utilisateur le plus actif: %s\n", get_utilisateur_le_plus_actif());
    printf("Montant total penalites : %.2f FCFA\n", get_montant_total_penalites());
    printf("============================================\n");
}

void menu_statistiques() {
    int choix;

    do {
        printf("\n========== MENU STATISTIQUES ==========\n");
        printf("1. Statistiques generales\n");
        printf("2. Statistiques du jour\n");
        printf("3. Classements\n");
        printf("4. Tout afficher\n");
        printf("0. Retour au menu principal\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                afficher_statistiques_generales();
                break;
            case 2:
                afficher_statistiques_journalieres();
                break;
            case 3:
                afficher_classements();
                break;
            case 4:
                printf("\n");
                afficher_statistiques_generales();
                afficher_statistiques_journalieres();
                afficher_classements();
                break;
            case 0:
                printf("Retour au menu principal\n");
                break;
            default:
                printf("Choix invalide. Veuillez reessayer.\n");
        }

    } while (choix != 0);
}
