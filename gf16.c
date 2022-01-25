#include "gf16.h"

// ATTENTION : pas d'inversion à ce stade (pas sûre d'en avoir besoin plus tard)

/*int main (){
    
    testFonctions1();
    return 0;
    
}*/

void printChar(char c){
    printf("%X  ", c);
}

void printPol(char c){
    int test = 0;
    for (int i = 4; i < 8; i++) {
        if (!!((c << i) & 0x80)){
            if (i == 7){
                printf(" + 1");
            }
            else{
                if (i == 6){
                    if (test == 0){
                        printf("x");
                        test += 1;

                    }
                    else{
                        printf(" + x");
                        test += 1;
                    }
                }
                else {
                    if (test == 0){
                        printf("x^%d", (7-i));
                        test += 1;
                    }
                    else{
                        printf(" + x^%d", (7-i));
                        test += 1;
                    }
                }
                
            }
        }
    else{
        if (i == 7){
            if (test == 0)
                printf("0");
            }
        }
    }
}

void addPol(char c1, char c2, char *c3){
    *c3 = c1 ^ c2;
}

void multX(char *c1){
    char t = 0x10;
    char t1 = 9;
    if ((!!((*c1 << 4) & 0x80)) == 1){
        *c1 = ((*c1 << 1) ^ t);
        *c1 = (*c1 ^ t1);
    }
    else {
        *c1 = (*c1 << 1);
    }
}

void multGF(char c1, char c2, char *c3){
    /* MULTIPLICATION A AMELIORER SI BESOIN D ETRE RAPIDES*/
    /* Potentiellement faire une table 16*16 pour la multiplication de polynôme*/
    /* Ou voir le papier */
    char c4 = c1;
    char c5 = c2;
    *c3 = 0;
    for(int k = 0; k < 4; k++){
        c5 = c2;
        if (!!((c4 << 4) & 0x80)){
            for (int i = 0; i < 3 - k; i++){
                multX(&c5);
            }
            *c3 = *c3 ^ c5;
        }
        c4 = c4 << 1;
    }
}

void testFonctions1(){
    printf("\n\n******************************************\n*** DEBUT DES TESTS SUR LE CORPS FINI ***\n******************************************\n\n");
    srand(time(NULL));
    
    char P1 = (char)(rand()%16);
    char P2 = (char)(rand()%16);
    printf("P1 = %d \n\n", P1);
    printf("P2 = %d \n\n", P2);
    
    /*AFFICHAGE*/
    printf("/***** Affichage sous forme polynomiale : *****/ \n");
    printPol(P1);
    printf("\n");
    
    /* ADDITION */
    printf("/***** Addition *****/");
    printf("\nP1 = ");
    printPol(P1);
    printf("\net P2 = ");
    printPol(P2);
    printf("\nnous donne\n");
    addPol(P1, P2, &P1);
    printPol(P1);
    printf("\n");
    
    /*MULTIPLICATION PAR X*/
    printf("/***** Multiplication par X *****/");
    printf("\nP2 = ");
    printPol(P2);
    multX(&P2);
    printf("\nxP2 = ");
    printPol(P2);
    multX(&P2);
    printf("\nx^2P2 = ");
    printPol(P2);
    printf("\nP1 = ");
    printPol(P1);
    multX(&P1);
    printf("\nxP1 = ");
    printPol(P1);
    
    /*PRODUIT DE POLYNOMES*/
    printf("\n/***** Produit de polynômes *****/");
    P1 = (char)(rand()%16);
    P2 = (char)(rand()%16);
    printf("\nP1 = ");
    printPol(P1);
    printf("\nmultiplié par P2 =");
    printPol(P2);
    printf("\nnous donne:\n");
    multGF(P1, P2, &P2);
    printPol(P2);
    
    printf("\n\n***************************************\n*** FIN DES TESTS SUR LE CORPS FINI ***\n***************************************\n\n");
}
