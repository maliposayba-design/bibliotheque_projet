int connexion(){
    FILE *f;
    FILE *temp;
    User u;
    char login[15];
    char motPasse[100];
    int role = 0;
    int trouve = 0;
    time_t rawtime;
    struct tm *timeinfo;
    char timestamp[20];

    printf("CONNEXION\n");
    printf("Login : ");
    scanf("%s", login);
    printf("Mot de passe : ");
    scanf("%s", motPasse);

    f = fopen("DATABASE/USERS.dat", "rb");
    temp = fopen("DATABASE/TEMP.dat", "wb");

    if(f == NULL || temp == NULL)
    {
        printf("Impossible d'ouvrir le fichier.\n");
        if(f != NULL) fclose(f);
        if(temp != NULL) fclose(temp);
        return 0;
    }

    while(fread(&u, sizeof(User), 1, f))
    {
        if(strcmp(login, u.login) == 0 && strcmp(motPasse, u.motPasse) == 0)
        {
            trouve = 1;

            if(strcmp(u.etat, "ACTIF") != 0)
            {
                printf("\nCe compte est inactif.\n");
                fwrite(&u, sizeof(User), 1, temp);
                continue;
            }

            time(&rawtime);
            timeinfo = localtime(&rawtime);
            strftime(timestamp, sizeof(timestamp), "%d/%m/%Y %H:%M", timeinfo);
            strcpy(u.derniereConnexion, timestamp);

            printf("\nConnexion reussie.\n");
            printf("Bienvenue %s %s\n", u.prenom, u.nom);

            if(strcmp(u.role, "ADMIN") == 0)
            {
                printf("Vous etes connecte en tant qu'ADMIN.\n");
                role = 1;
            }
            else
            {
                printf("Vous etes connecte en tant qu'UTILISATEUR.\n");
                role = 2;
            }
        }

        fwrite(&u, sizeof(User), 1, temp);
    }

    fclose(f);
    fclose(temp);

    if(trouve == 0)
    {
        remove("DATABASE/TEMP.dat");
        printf("\nLogin ou mot de passe incorrecte");
        return 0;
    }

    remove("DATABASE/USERS.dat");
    rename("DATABASE/TEMP.dat", "DATABASE/USERS.dat");

    return role;
}