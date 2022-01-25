//
//  diffusion.c
//  
//
//  Created by Rachelle Heim on 24/01/2022.
//

#include "diffusion.h"

int main (){
    
    int q = 19;
    int r = 39;
    int s = 60;
    
    //We construct a tab of size 82x82
    //If the nibble i depends on the initial nibble j, then tab[i][j] = 1. Else tab[i][j] = 0.
    char **tab = (char**)malloc(n*sizeof(char*));
    for (int i = 0; i < n; i++){
        tab[i] = (char*)malloc(n*sizeof(char));
    }
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            tab[i][j] = (char)0;
        }
        tab[i][i] = 1;
    }
    
    //We wish to know the minimum number of rounds nr_min such that tab[i][j] = 1 for all i, j
    
    int nr_min = 0;
    while (nr_min < 1000){
        //First, P0, Q0, R0 and S0 are modified
        //Then, the state is shifted
    
        //P0
        Li_becomes_Li_or_Lj(&tab, 0, 0);
        Li_becomes_Li_or_Lj(&tab, 0, 7);
        Li_becomes_Li_or_Lj(&tab, 0, 10);
        Li_becomes_Li_or_Lj(&tab, 0, 6);
        Li_becomes_Li_or_Lj(&tab, 0, 18);
        Li_becomes_Li_or_Lj(&tab, 0, q+9);
        Li_becomes_Li_or_Lj(&tab, 0, r+10);
        Li_becomes_Li_or_Lj(&tab, 0, s+12);
        
        //Q0
        Li_becomes_Li_or_Lj(&tab, q+0, q+0);
        Li_becomes_Li_or_Lj(&tab, q+0, q+4);
        Li_becomes_Li_or_Lj(&tab, q+0, q+6);
        Li_becomes_Li_or_Lj(&tab, q+0, q+7);
        Li_becomes_Li_or_Lj(&tab, q+0, q+15);
        Li_becomes_Li_or_Lj(&tab, q+0, q+3);
        Li_becomes_Li_or_Lj(&tab, q+0, q+7);
        Li_becomes_Li_or_Lj(&tab, q+0, 4);
        Li_becomes_Li_or_Lj(&tab, q+0, r+2);
        Li_becomes_Li_or_Lj(&tab, q+0, s+5);
        
        //R0
        Li_becomes_Li_or_Lj(&tab, r+0, r+0);
        Li_becomes_Li_or_Lj(&tab, r+0, r+1);
        Li_becomes_Li_or_Lj(&tab, r+0, r+15);
        Li_becomes_Li_or_Lj(&tab, r+0, r+17);
        Li_becomes_Li_or_Lj(&tab, r+0, r+19);
        Li_becomes_Li_or_Lj(&tab, r+0, r+13);
        Li_becomes_Li_or_Lj(&tab, r+0, r+15);
        Li_becomes_Li_or_Lj(&tab, r+0, 12);
        Li_becomes_Li_or_Lj(&tab, r+0, q+11);
        Li_becomes_Li_or_Lj(&tab, r+0, s+16);
        
        //S0
        Li_becomes_Li_or_Lj(&tab, s+0, s+0);
        Li_becomes_Li_or_Lj(&tab, s+0, s+1);
        Li_becomes_Li_or_Lj(&tab, s+0, s+4);
        Li_becomes_Li_or_Lj(&tab, s+0, s+10);
        Li_becomes_Li_or_Lj(&tab, s+0, s+11);
        Li_becomes_Li_or_Lj(&tab, s+0, s+18);
        Li_becomes_Li_or_Lj(&tab, s+0, 16);
        Li_becomes_Li_or_Lj(&tab, s+0, q+17);
        Li_becomes_Li_or_Lj(&tab, s+0, r+2);
        
        //P0, Q0, R0, S0 are stored, their value will be assigned to P18,Q19,R20,S21
        char *Ptemp = (char*)malloc(sizeof(char)*n);
        char *Qtemp = (char*)malloc(sizeof(char)*n);
        char *Rtemp = (char*)malloc(sizeof(char)*n);
        char *Stemp = (char*)malloc(sizeof(char)*n);

        for (int i = 0; i < n; i++){
            Ptemp[i] = tab[0][i];
            Qtemp[i] = tab[0+q][i];
            Rtemp[i] = tab[0+r][i];
            Stemp[i] = tab[0+s][i];
        }
        
        //P is shifted
        for (int i = 0; i < 18; i++){
            Li_becomes_Lj(&tab, i, i+1);
        }
        for (int i = 0; i < n; i++){
            tab[18][i] = Ptemp[i];
        }
        
        //Q is shifted
        for (int i = 0; i < 19; i++){
            Li_becomes_Lj(&tab, i+q, i+q+1);
        }
        for (int i = 0; i < n; i++){
            tab[19+q][i] = Qtemp[i];
        }
        
        //R is shifted
        for (int i = 0; i < 20; i++){
            Li_becomes_Lj(&tab, i+r, i+r+1);
        }
        for (int i = 0; i < n; i++){
            tab[20+r][i] = Rtemp[i];
        }
        
        //S is shifted
        for (int i = 0; i < 21; i++){
            Li_becomes_Lj(&tab, i+s, i+s+1);
        }
        for (int i = 0; i < n; i++){
            tab[21+s][i] = Stemp[i];
        }
        
        //Checking if tab[i][j] = 1 for all i, j
        //As soon as the program finds a 0 in tab, it goes to the next round up until it's only 1
        int check = 1;
        for (int i = 0; i < n; i++){
            if (check == 0)
                break;
            for (int j = 0; j < n; j++){
                if (tab[i][j] != (char)1){
                    check = 0;
                    break;
                }
            }
        }
        if(check == 1){
            break;
        }
        nr_min++;
    }
    printf("\nThe minimum number of rounds is %d\n\n", nr_min);
    return 0;
}

void printTab(char **Tab){
    printf("\n\n");
    for (int i = 0; i < 1; i++){
        for (int j = 0; j < n; j++){
            printf("%X  ",Tab[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

void Li_becomes_Li_or_Lj(char ***Tab, int i, int j){
    for (int k = 0; k < n; k++){
        (*Tab)[i][k] |= (*Tab)[j][k];
    }
}

void Li_becomes_Lj(char ***Tab, int i, int j){
    for (int k = 0; k < n; k++){
        (*Tab)[i][k] = (*Tab)[j][k];
    }
}
