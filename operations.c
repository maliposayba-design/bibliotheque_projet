#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operation.h"

void ActualiserFBinaire(){
    FILE *f;
    f=fopen("DATABASES/BARROWS.dat","ab");

    if(f!=NULL){
        fclose(f);
    }
    else{
        printf("fichier non creer");
    }

    f=fopen("DATABASES/RETURNS.dat","ab");

    if(f!=NULL){
        fclose(f);
    }
    else{
        printf("fichier non creer");
    }

    f=fopen("DATABASES/PENALTIES.dat","ab");

    if(f!=NULL){
        fclose(f);
    }
    else{
        printf("fichier non creer");
    }

}



void EmpruntLivre(){
    FILE *f;
    f=fopen("DATABASE/BORROWS.dat","wb");

    EMPRUNT E;

    printf("IdEmprunt :");
    scanf("%d",E.idEmprunt);
    printf("numero Emprunt :");
    fflush(stdin);
    fgets()


}


