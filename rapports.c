#include "rapports.h"
#include "statistiques.h"
#include "reservation.h"
#include "structure.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

char* gettime_rapport() {
    static char buffer[20];
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    sprintf(buffer, "%02d:%02d:%02d",
            tm->tm_hour, tm->tm_min, tm->tm_sec);
    return buffer;
}

void* lire_fichier_rapport(const char* nom, size_t taille, int* nb) {
    FILE* f = fopen(nom, "rb");
    if (f == NULL) {
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

void ecrire_historique(const char* action, const char* detail) {
    FILE* f = fopen("REPORTS/HISTORY.txt", "a");
    if (f == NULL) {
        printf("Erreur : Impossible d'ouvrir HISTORY.txt\n");
        return;
    }

    char dateTime[30];
    strcpy(dateTime, getdatetime());
    fprintf(f, "[%s] %s: %s\n", dateTime, action, detail);
    fclose(f);
}

void afficher_historique() {
    FILE* f = fopen("REPORTS/HISTORY.txt", "r");
    if (f == NULL) {
        printf("Aucun historique disponible.\n");
        return;
    }

    printf("\n=========== HISTORIQUE DES OPERATIONS ===========\n");
    printf("--------------------------------------------------------\n");

    char ligne[500];
    while (fgets(ligne, sizeof(ligne), f)) {
        printf("%s", ligne);
    }

    fclose(f);
}

void generer_recu_emprunt(int idEmprunt) {
    int nb;
    EMPRUNT* emprunts = lire_fichier_rapport("DATABASE/BORROWS.dat", sizeof(EMPRUNT), &nb);
    if (emprunts == NULL) {
        printf("Aucun emprunt trouve.\n");
        return;
    }

    EMPRUNT* E = NULL;
    for (int i = 0; i < nb; i++) {
        if (emprunts[i].idEmprunt == idEmprunt) {
            E = &emprunts[i];
            break;
        }
    }

    if (E == NULL) {
        printf("Emprunt ID %d inexistant.\n", idEmprunt);
        free(emprunts);
        return;
    }

    int nbLivres;
    BOOK* livres = lire_fichier_rapport("DATABASE/BOOKS.dat", sizeof(BOOK), &nbLivres);
    if (livres == NULL) {
        free(emprunts);
        return;
    }

    BOOK* livre = NULL;
    for (int i = 0; i < nbLivres; i++) {
        if (livres[i].id == E->idLivre) {
            livre = &livres[i];
            break;
        }
    }

    if (livre == NULL) {
        free(emprunts);
        free(livres);
        return;
    }

    int nbUsers;
    User* users = lire_fichier_rapport("DATABASE/USERS.dat", sizeof(User), &nbUsers);
    if (users == NULL) {
        free(emprunts);
        free(livres);
        return;
    }

    User* user = NULL;
    for (int i = 0; i < nbUsers; i++) {
        if (users[i].id == E->idUtilisateur) {
            user = &users[i];
            break;
        }
    }

    if (user == NULL) {
        free(emprunts);
        free(livres);
        free(users);
        return;
    }

    char nomFichier[200];
    char jour[3], mois[3], annee[5], heure[3], minute[3], seconde[3];
    strncpy(jour, E->date_emprunt, 2);
    strncpy(mois, E->date_emprunt + 3, 2);
    strncpy(annee, E->date_emprunt + 6, 4);
    strncpy(heure, E->date_emprunt + 11, 2);
    strncpy(minute, E->date_emprunt + 14, 2);
    strncpy(seconde, E->date_emprunt + 17, 2);

    jour[2] = '\0';
    mois[2] = '\0';
    annee[4] = '\0';
    heure[2] = '\0';
    minute[2] = '\0';
    seconde[2] = '\0';

    sprintf(nomFichier, "REPORTS/BORROWS/BORROW_%s%s%s%s%s%s_%s.txt",
            annee, mois, jour, heure, minute, seconde, user->login);

    FILE* f = fopen(nomFichier, "w");
    if (f == NULL) {
        printf("Erreur : Impossible de creer le fichier.\n");
        free(emprunts);
        free(livres);
        free(users);
        return;
    }

    fprintf(f, "============================================\n");
    fprintf(f, "          BIBLIOTHEQUE E-LIBRARY ISI\n");
    fprintf(f, "              RECU D'EMPRUNT\n");
    fprintf(f, "============================================\n\n");
    fprintf(f, "Numero d'emprunt: %s\n", E->num_emprunt);
    fprintf(f, "Lecteur : %s %s (%s)\n", user->prenom, user->nom, user->login);
    fprintf(f, "Date d'emprunt : %s\n", E->date_emprunt);
    fprintf(f, "Date prevue retour: %s\n\n", E->date_retour);
    fprintf(f, "Livre emprunte    : %s\n", livre->titre);

    int nbAuteurs;
    AUTHORS* auteurs = lire_fichier_rapport("DATABASE/AUTHORS.dat", sizeof(AUTHORS), &nbAuteurs);
    if (auteurs != NULL) {
        int auteurTrouve = 0;
        for (int i = 0; i < nbAuteurs; i++) {
            if (auteurs[i].id == livre->id_auteur) {
                fprintf(f, "Auteur : %s\n", auteurs[i].nom_auteur);
                auteurTrouve = 1;
                break;
            }
        }
        if (!auteurTrouve) {
            fprintf(f, "Auteur : Inconnu\n");
        }
        free(auteurs);
    } else {
        fprintf(f, "Auteur : Inconnu\n");
    }

    fprintf(f, "ISBN : %s\n", livre->isbn);
    fprintf(f, "Merci de respecter la date de retour !\n");
    fprintf(f, "Penalite : 500 FCFA par jour de retard\n");
    fprintf(f, "============================================\n");
    fclose(f);

    printf("Recu d'emprunt genere : %s\n", nomFichier);

    char detail[200];
    sprintf(detail, "Recu emprunt genere pour %s - Livre: %s", user->login, livre->titre);
    ecrire_historique("Generation recu emprunt", detail);

    free(emprunts);
    free(livres);
    free(users);
}

void generer_recu_retour(int idRetour) {
    int nb;
    RETOUR* retours = lire_fichier_rapport("DATABASE/RETURNS.dat", sizeof(RETOUR), &nb);
    if (retours == NULL) {
        printf("Aucun retour trouve.\n");
        return;
    }

    RETOUR* R = NULL;
    for (int i = 0; i < nb; i++) {
        if (retours[i].idRetour == idRetour) {
            R = &retours[i];
            break;
        }
    }

    if (R == NULL) {
        printf("Retour ID %d inexistant.\n", idRetour);
        free(retours);
        return;
    }

    int nbEmprunts;
    EMPRUNT* emprunts = lire_fichier_rapport("DATABASE/BORROWS.dat", sizeof(EMPRUNT), &nbEmprunts);
    if (emprunts == NULL) {
        free(retours);
        return;
    }

    EMPRUNT* E = NULL;
    for (int i = 0; i < nbEmprunts; i++) {
        if (emprunts[i].idEmprunt == R->idEmprunt) {
            E = &emprunts[i];
            break;
        }
    }

    if (E == NULL) {
        free(retours);
        free(emprunts);
        return;
    }

    int nbLivres;
    BOOK* livres = lire_fichier_rapport("DATABASE/BOOKS.dat", sizeof(BOOK), &nbLivres);
    if (livres == NULL) {
        free(retours);
        free(emprunts);
        return;
    }

    BOOK* livre = NULL;
    for (int i = 0; i < nbLivres; i++) {
        if (livres[i].id == E->idLivre) {
            livre = &livres[i];
            break;
        }
    }

    if (livre == NULL) {
        free(retours);
        free(emprunts);
        free(livres);
        return;
    }

    int nbUsers;
    User* users = lire_fichier_rapport("DATABASE/USERS.dat", sizeof(User), &nbUsers);
    if (users == NULL) {
        free(retours);
        free(emprunts);
        free(livres);
        return;
    }

    User* user = NULL;
    for (int i = 0; i < nbUsers; i++) {
        if (users[i].id == E->idUtilisateur) {
            user = &users[i];
            break;
        }
    }

    if (user == NULL) {
        free(retours);
        free(emprunts);
        free(livres);
        free(users);
        return;
    }

    char nomFichier[200];
    char jour[3], mois[3], annee[5], heure[3], minute[3], seconde[3];

    strncpy(jour, R->dateR, 2);
    strncpy(mois, R->dateR + 3, 2);
    strncpy(annee, R->dateR + 6, 4);
    strncpy(heure, R->dateR + 11, 2);
    strncpy(minute, R->dateR + 14, 2);
    strncpy(seconde, R->dateR + 17, 2);
    jour[2] = '\0';
    mois[2] = '\0';
    annee[4] = '\0';
    heure[2] = '\0';
    minute[2] = '\0';
    seconde[2] = '\0';

    sprintf(nomFichier, "REPORTS/RETURNS/RETURN_%s%s%s%s%s%s_%s.txt",
            annee, mois, jour, heure, minute, seconde, user->login);

    FILE* f = fopen(nomFichier, "w");
    if (f == NULL) {
        printf("Erreur : Impossible de creer le fichier.\n");
        free(retours);
        free(emprunts);
        free(livres);
        free(users);
        return;
    }

    fprintf(f, "============================================\n");
    fprintf(f, "          BIBLIOTHEQUE E-LIBRARY ISI\n");
    fprintf(f, "              RECU DE RETOUR\n");
    fprintf(f, "============================================\n\n");
    fprintf(f, "Numero d'emprunt : %s\n", E->num_emprunt);
    fprintf(f, "Lecteur : %s %s (%s)\n", user->prenom, user->nom, user->login);
    fprintf(f, "Date de retour : %s\n\n", R->dateR);
    fprintf(f, "Livre retourne : %s\n", livre->titre);
    fprintf(f, "ISBN : %s\n\n", livre->isbn);

    if (R->nbr_jourR) {
        fprintf(f, "           RETARD DETECTE !\n");
        fprintf(f, "============================================\n");
        fprintf(f, "Date prevue : %s\n", E->date_retour);
        fprintf(f, "Penalite : %.2f FCFA\n\n", R->mont_penalite);
        fprintf(f, "Merci de regler la penalite au plus vite.\n");
    } else {
        fprintf(f, "Retour dans les delais. Merci !\n");
    }

    fprintf(f, "============================================\n");
    fclose(f);

    printf("Recu de retour genere : %s\n", nomFichier);
    char detail[200];
    sprintf(detail, "Recu retour genere pour %s - Livre: %s", user->login, livre->titre);
    ecrire_historique("Generation recu retour", detail);

    free(retours);
    free(emprunts);
    free(livres);
    free(users);
}

void generer_rapport_journalier() {
    char nomFichier[200];
    char date[30];
    strcpy(date, getdate());
    char dateFichier[15];
    strcpy(dateFichier, date);
    for (int i = 0; dateFichier[i]; i++) {
        if (dateFichier[i] == '/') dateFichier[i] = '-';
    }

    sprintf(nomFichier, "REPORTS/DAILY/REPORT_%s.txt", dateFichier);
    FILE* f = fopen(nomFichier, "w");
    if (f == NULL) {
        printf("Erreur : Impossible de creer le rapport.\n");
        return;
    }

    fprintf(f, "============================================\n");
    fprintf(f, "          RAPPORT JOURNALIER\n");
    fprintf(f, "       BIBLIOTHEQUE E-LIBRARY ISI\n");
    fprintf(f, "================================================\n\n");
    fprintf(f, "Date : %s\n", date);
    fprintf(f, "Heure generation : %s\n\n", gettime_rapport());
    fprintf(f, "--- STATISTIQUES GENERALES ---\n");
    fprintf(f, "Total utilisateurs : %d\n", compter_utilisateurs());
    fprintf(f, "Total livres : %d\n", compter_livres());
    fprintf(f, "Emprunts en cours : %d\n", compter_emprunts_en_cours());
    fprintf(f, "Livres disponibles : %d\n", compter_livres_disponibles());
    fprintf(f, "Reservations totales : %d\n", compter_reservations_totales());
    fprintf(f, "Penalites impayees : %d\n\n", compter_penalites_impayees());
    fprintf(f, "--- STATISTIQUES DU JOUR ---\n");
    fprintf(f, "Emprunts : %d\n", compter_emprunts_du_jour());
    fprintf(f, "Retours : %d\n", compter_retours_du_jour());
    fprintf(f, "Reservations : %d\n", compter_reservations_du_jour());
    fprintf(f, "Nouveaux utilisateurs : %d\n", compter_nouveaux_utilisateurs_du_jour());
    fprintf(f, "Penalites : %d\n", compter_penalites_du_jour());
    fprintf(f, "Montant des penalites : %.2f FCFA\n\n", montant_penalites_du_jour());
    fprintf(f, "--- CLASSEMENTS ---\n");
    fprintf(f, "Livre le plus emprunte : %s\n", get_livre_le_plus_emprunte());
    fprintf(f, "Utilisateur le plus actif: %s\n\n", get_utilisateur_le_plus_actif());
    fprintf(f, "--- FINANCES ---\n");
    fprintf(f, "Montant total des penalites: %.2f FCFA\n", get_montant_total_penalites());
    fprintf(f, "============================================\n");
    fclose(f);
    printf("Rapport journalier genere : %s\n", nomFichier);
    ecrire_historique("Generation rapport journalier", date);
}

void afficher_liste_rapports() {
    printf("\n========== LISTE DES RAPPORTS ==========\n");
    printf("1. Historique (HISTORY.txt)\n");
    printf("2. Recus d'emprunt (REPORTS/BORROWS/)\n");
    printf("3. Recus de retour (REPORTS/RETURNS/)\n");
    printf("4. Rapports journaliers (REPORTS/DAILY/)\n");
    printf("------------------------------------------------\n");
}

void menu_rapports() {
    int choix;

    do {
        printf("\n========== MENU RAPPORTS ==========\n");
        printf("1. Afficher l'historique\n");
        printf("2. Generer un recu d'emprunt\n");
        printf("3. Generer un recu de retour\n");
        printf("4. Generer le rapport journalier\n");
        printf("5. Afficher la liste des rapports\n");
        printf("0. Retour au menu principal\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                afficher_historique();
                break;
            case 2: {
                int id;
                printf("ID de l'emprunt : ");
                scanf("%d", &id);
                generer_recu_emprunt(id);
                break;
            }
            case 3: {
                int id;
                printf("ID du retour : ");
                scanf("%d", &id);
                generer_recu_retour(id);
                break;
            }
            case 4:
                generer_rapport_journalier();
                break;
            case 5:
                afficher_liste_rapports();
                break;
            case 0:
                printf("Retour au menu principal\n");
                break;
            default:
                printf("Choix invalide.\n");
        }

    } while (choix != 0);
}
