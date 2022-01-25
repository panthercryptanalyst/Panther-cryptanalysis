//
//  attack.c
//  
//
//  Created by Rachelle Heim on 20/01/2022.
//

#include "attack.h"


char tabSb[16] = {(char)12,(char)5,(char)6,(char)11,(char)9,(char)0,(char)10,(char)13,(char)3,(char)14,(char)15,(char)8,(char)4,(char)7,(char)1,(char)2};

char tabInvSb[16] = {(char)5,(char)14,(char)15,(char)8,(char)12,(char)1,(char)2,(char)13,(char)11,(char)4,(char)6,(char)3,(char)0,(char)7,(char)9,(char)10};

char tabToeplitzMatrix[4][4] = {
    {(char)1,(char)1,(char)2,(char)(12)},
    {(char)(6),(char)1,(char)1,(char)2},
    {(char)1,(char)(6),(char)1,(char)1},
    {(char)(12),(char)1,(char)(6),(char)1}
    
};

char invTabToeplitzMatrix[4][4] = {
    {(char)14,(char)12,(char)10,(char)12},
    {(char)6,(char)14,(char)12,(char)10},
    {(char)5,(char)6,(char)14,(char)12},
    {(char)6,(char)5,(char)6,(char)14}
};

int main(){
    printf("\n **** ATTACKS ON PANTHER **** \n");
    srand(time(NULL));
    
    //Secret key generation
    char *key =  (char *)malloc(32*sizeof(char));
    for (int i = 0; i < 32; i++){
        key[i] = (char)(rand()%16);
    }
    printf("\n\n **  Random key generated :  ** \n");
    printf("\n");
    for (int i = 0; i < 32; i++){
        printf("%X  ", key[i]);
    }
    
    //IV generation
    char *IV =  (char *)malloc(32*sizeof(char));
    for (int i = 0; i < 32; i++){
        IV[i] = (char)(rand()%16);
    }
    printf("\n\n **  Random IV generated :  ** \n\n");
    for (int i = 0; i < 32; i++){
        printf("%X  ", IV[i]);
    }
    
    //PT generation
    int ptlen = 8; //In order to test all of our attacks, we choose a plaintext with more than 6 blocks
    char **PT =  (char **)malloc(ptlen*sizeof(char*));
    for (int i = 0; i < ptlen; i++){
        PT[i] = (char *)malloc(16*sizeof(char));
    }
    for (int i = 0; i < ptlen; i++){
        for (int j = 0; j < 16; j++)
            PT[i][j] = (char)(rand()%16);
    }
    
    printf("\n\n **  Random %d blocks plaintext generated :  ** \n\n", ptlen);
    for (int i = 0; i < ptlen; i++){
        for(int j = 0; j < 16; j++){
            printf("%X   ", PT[i][j]);
        }
        printf("\n");
    }
    
    //AD generation (no AD in the encrypt function so we just set it to NULL)
    
    printf("\n **  No associated data  ** \n");
    printf("\n ... ");
    
    char *AD = NULL;
    
    //Encryption
    //We encrypt the plaintext with the secret key.
    //We set the tag to be 128 bits long.
    //Warning : the following code only works iff hashlen < ptlen
    int hashlen = 4;
    char **CTT = (char **)malloc((ptlen+hashlen)*sizeof(char*));
    char **CT = (char**)malloc(ptlen*sizeof(char*));
    char **T = (char**)malloc(hashlen*sizeof(char*));
    for (int i = 0; i < (hashlen + ptlen); i++){
        CTT[i] = (char *)malloc(16*sizeof(char));
        if (i < ptlen)
            CT[i] = (char *)malloc(16*sizeof(char));
        if (i < hashlen)
            T[i] = (char *)malloc(16*sizeof(char));
    }
    CTT = encrypt(key,IV,AD,PT,hashlen,ptlen);
    for(int i = 0; i < ptlen; i++){
        for (int j = 0; j < 16; j++){
            CT[i][j] = CTT[i][j];
            if (i < hashlen){
                T[i][j] = CTT[i + ptlen][j];
            }
        }
    }
    
    printf("\n\n ** The plaintext is encrypted. We get the following ciphertext : ** \n\n");
    for (int i = 0; i < ptlen; i++){
        for(int j = 0; j < 16; j++){
            printf("%X   ", CT[i][j]);
        }
        printf("\n");
    }

    printf("\n\n ** ... and the following tag ** \n\n");
    for (int i = 0; i < hashlen; i++){
        for(int j = 0; j < 16; j++){
            printf("%X   ", T[i][j]);
        }
        printf("\n");
    }
    
    //Key recovery
    printf("\n\n ** With the plaintext/ciphertext pair, we recover the secret key. We get : ** \n\n");
    char *recoveredKey;
    recoveredKey = keyrecovery(PT, CT);
    for (int i = 0; i < 32; i++){
        printf("%X  ", recoveredKey[i]);
    }
    
    int check = 0;
    for (int i = 0; i < 32; i++){
        if (key[i] != recoveredKey[i]){
            check = 1;
        }
    }
    if (check == 1){
        printf("\n Failed to recover the right key\n");
    }
    else{
        printf("\n\n\n *********************************\n **** Key recovery successful **** \n *********************************\n\n");
    }
    
    //Plaintext recovery
    char **recoveredPT;
    recoveredPT = plainrecovery(CT,ptlen);
    
    printf("\n \n ** With the ciphertext only, we recover the last blocks of the plaintext. We get : ** \n\n");
    for(int i = 0; i < ptlen - 6; i++){
        for (int j = 0; j < 16; j++){
            printf("%X  ",recoveredPT[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    
    check = 0;
    for(int i = 0; i < ptlen - 6; i++){
        for (int j = 0; j < 16; j++){
            if(recoveredPT[i][j] != PT[i+6][j])
                check = 1;
        }
    }
    if (check == 1){
        printf("\n Failed to recover the right plaintext\n");
    }
    else{
        printf("\n ****************************************\n **** Plaintext recovery successful **** \n ****************************************\n\n");
    }
    
    //Forge
    char **forgedCT;
    forgedCT = forge(CT, ptlen);
    
    for (int i = 0; i < ptlen; i++){
        for(int j = 0; j < 16; j++){
            printf("%X   ", forgedCT[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("\n");
    check = checkforge(key,IV,forgedCT,T,ptlen);
    
    printf("** We now forge a new valid ciphertext for the same tag ** \n\n");
    
    printf("** Our forged ciphertext is : ** \n\n");
    
    for (int i = 0; i < ptlen; i++){
        for(int j = 0; j < 16; j++){
            printf("%X   ", forgedCT[i][j]);
        }
        printf("\n");
    }
    
    printf("\n \n ** We check whether the forged ciphertext is valid... ** \n\n");
    
    if (check == 0){
        printf("\n Failed to forge \n");
    }
    else{
        printf("\n ******************************************************\n ********* The tag is valid, forge successful ********* \n ******************************************************\n\n");
    }
    
    //Freeing
    free(recoveredKey);
    free(AD);
    for(int i = 0; i < ptlen; i++){
        free(PT[i]);
    }
    free(PT);
    free(key);
    free(IV);
    
    for (int i = 0; i<(ptlen+hashlen); i++){
        free(CTT[i]);
        if (i < ptlen){
            free(CT[i]);
        }
        if (i < hashlen){
            free(T[i]);
        }
    }
    free(CTT);
    free(CT);
    free(T);
    return 0;
}

/******************************************************/
/******************************************************/
//Panther state
/******************************************************/
/******************************************************/

void initState(char **State){
    *State = (char*)malloc(82 * sizeof(char));
    for (int i = 0 ; i < 82; i++){
        (*State)[i] = (char)0;
    }
}

void freeState(char **State){
    free(*State);
    *State = NULL;
}

void printState(char *State){
    printf("\n");
    int t = 18;
    for (int i = 0; i < 4; i++){
        int k = t;
        for(int j = 0; j < (19 + i); j++){
            printf("%X   ", State[k]);
            k--;
            t++;
        }
        t++;
        printf("\n");
    }
    printf("\n");
}

void printState2(char *State){
    printf("\n");
    int t = 0;
    for (int i = 0; i < 4; i++){
        for(int j = 0; j < (19 + i); j++){
            printf("%X   ", State[t]);
            t++;
        }
        printf("\n");
    }
    printf("\n");
}

/******************************************************/
/******************************************************/
//Internal functions
/******************************************************/
/******************************************************/

void matrixMultiplication(char **Vect, char Matrix[4][4]){
    char *Copie;
    initState(&Copie);
    for (int i = 0; i < 4; i++)
        Copie[i] = (*Vect)[i];
    
    for (int i = 0; i < 4; i++)
        (*Vect)[i] = (char)0;
    
    char temp = (char)0;
    for (int i = 0; i < 4; i++){
        for (int k = 0; k < 4; k++){
            multGF((Copie)[k], Matrix[i][k], &temp);
            (*Vect)[i] = (*Vect)[i] ^ temp;
        }
    }
    freeState(&Copie);
}

void sBox(char **Vect, char *Sbox){
    char *build = (char *)malloc(4*sizeof(char));
    for(int i = 0; i < 4; i++ ){
        build[i] = (*Vect)[i];
        (*Vect)[i] = Sbox[((int)build[i])];
    }
    free(build);
}

void computesT (char **vectT, char *S){
    //t1, t2, t3, t4 computing
    char temp = 0;
    char temp1 = 0;
    int q =19;
    int r =39;
    int s =60;
    
    //Compute fp fq fr fs
    multGF(S[6], S[18], &temp);
    char fp = S[0] ^ S[7] ^ S[10] ^ temp;
    
    multGF(S[3+q], S[7+q], &temp);
    char fq = S[0+q] ^ S[4+q] ^ S[6+q] ^ S[7+q] ^ S[15+q] ^ temp;
    
    multGF(S[13+r], S[15+r], &temp);
    char fr = S[0+r] ^ S[1+r] ^ S[15+r] ^ S[17+r] ^ S[19+r] ^ temp;
    
    multGF(S[4+s], S[10+s], &temp);
    multGF(S[11+s], S[18+s], &temp1);
    char fs = S[0+s] ^ S[1+s] ^ temp ^ temp1;
    
    //Compute gp gq gr gs
    char gp = S[9+q] ^ S[10+r] ^ S[12+s];
    char gq = S[4] ^ S[2+r] ^ S[5+s];
    char gr = S[12] ^ S[11+q] ^ S[16+s];
    char gs = S[16] ^ S[17+q] ^ S[2+r];
    
    //Round constants
    char rc1 = (char)(7);
    char rc2 = (char)(9);
    char rc3 = (char)(11);
    char rc4 = (char)(13);
    
    //Compute l
    (*vectT)[0] = fp^gp^rc1; //l1
    (*vectT)[1] = fq^gq^rc2; //l2
    (*vectT)[2] = fr^gr^rc3; //l3
    (*vectT)[3] = fs^gs^rc4; //l4
    
    //Toeplitz matrix and sbox step
    matrixMultiplication(vectT, tabToeplitzMatrix); //d1,d2,d3,d4
    sBox(vectT, tabSb);
    matrixMultiplication(vectT, tabToeplitzMatrix); //t1,t2,t3,t4
}

void computesInv0 (char **vectT, char *S){
    int q = 19;
    int r = 39;
    int s = 60;
    char temp = 0;
    char temp1 = 0;

    //Assign the value P18, Q19, R20, S21 to vecttT
    (*vectT)[0] = S[18];
    (*vectT)[1] = S[19 + q];
    (*vectT)[2] = S[20 + r];
    (*vectT)[3] = S[21 + s];
    
    //Toeplitz matrix and sbox step
    matrixMultiplication(vectT, invTabToeplitzMatrix);
    sBox(vectT, tabInvSb);
    matrixMultiplication(vectT, invTabToeplitzMatrix);
    
    //Compute fp' fq' fr' fs'
    multGF(S[6-1], S[18-1], &temp);
    char fp = S[7-1] ^ S[10-1] ^ temp;
    
    multGF(S[3+q-1], S[7+q-1], &temp);
    char fq = S[4+q-1] ^ S[6+q-1] ^ S[7+q-1] ^ S[15+q-1] ^ temp;
    
    multGF(S[13+r-1], S[15+r-1], &temp);
    char fr = S[1+r-1] ^ S[15+r-1] ^ S[17+r-1] ^ S[19+r-1] ^ temp;
    
    multGF(S[4+s-1], S[10+s-1], &temp);
    multGF(S[11+s-1], S[18+s-1], &temp1);
    char fs = S[1+s-1] ^ temp ^ temp1;
    
    //Compute gp gq gr gs
    char gp = S[9+q-1] ^ S[10+r-1] ^ S[12+s-1];
    char gq = S[4-1] ^ S[2+r-1] ^ S[5+s-1];
    char gr = S[12-1] ^ S[11+q-1] ^ S[16+s-1];
    char gs = S[16-1] ^ S[17+q-1] ^ S[2+r-1];
    
    //Round constants
    char rc1 = (char)(7);
    char rc2 = (char)(9);
    char rc3 = (char)(11);
    char rc4 = (char)(13);
    
    //Compute l
    (*vectT)[0] ^= fp^gp^rc1; //l1
    (*vectT)[1] ^= fq^gq^rc2; //l2
    (*vectT)[2] ^= fr^gr^rc3; //l3
    (*vectT)[3] ^= fs^gs^rc4; //l4
}

void shiftState(char **S){
    int q = 19;
    int r = 39;
    int s = 60;
    
    char *copie = (char*)malloc(82*sizeof(char));
    for (int j = 0; j < 82 ; j++)
        copie[j] = (*S)[j];
    
    for (int i = 0 ; i < (q-1) ; i++)
        (*S)[i] = copie[(i+1)%19];
    
    for (int i = q ; i < (r-1) ; i++)
        (*S)[i] = copie[(i+1)%r];
    
    for (int i = r ; i < (s-1) ; i++)
        (*S)[i] = copie[(i+1)%s];
    
    for (int i = s ; i < (82-1) ; i++)
        (*S)[i] = copie[(i+1)%82];
    
    (*S)[18] = 0;
    (*S)[38] = 0;
    (*S)[59] = 0;
    (*S)[81] = 0;
    free(copie);
}

/******************************************************/
/******************************************************/
//State update function
/******************************************************/
/******************************************************/

void F(char **State, int nr){
    char *t = (char *)malloc(4*sizeof(char));
    for (int i = 1; i < (nr+1); i++){
        computesT(&t, *State);
        shiftState(State);
        (*State)[18] = t[0];
        (*State)[38] = t[1];
        (*State)[59] = t[2];
        (*State)[81] = t[3];
    }
}

void invF(char **State, int nr){
    char *l = (char *)malloc(4*sizeof(char));
    char *tempo = (char*)malloc(82*sizeof(char));
    for (int i = 1; i < (nr+1); i++){
        computesInv0(&l, *State);
        for (int j = 0; j < 82; j++){
            tempo[j] = (*State)[j];
        }
        for(int j = 1; j < 82; j++){
            (*State)[j] = tempo[j-1];
        }
        (*State)[0] = l[0];
        (*State)[19] = l[1];
        (*State)[39] = l[2];
        (*State)[60] = l[3];
    }
    free(tempo);
}

/******************************************************/
/******************************************************/
//Sponge tools
/******************************************************/
/******************************************************/

void addToInternalState(char **S, char *I){
    int j = 0;
    while (j < 4){
        (*S)[15 + j] = (*S)[15 + j] ^ I[j];
        (*S)[16 + 19 + j] = (*S)[16 + 19 + j] ^ I[j + 4];
        (*S)[17 + 39 + j] = (*S)[17 + 39 + j] ^ I[j + 8];
        (*S)[18 + 60 + j] = (*S)[18 + 60 + j] ^ I[j + 12];
        j++;
    }
}

void copyInternalState(char **iCT, char *S){
    int j = 0;
    while (j < 4){
        (*iCT)[j] = S[15 + j];
        (*iCT)[j+4] = S[16 + 19 + j];
        (*iCT)[j+8] = S[17 + 39 + j];
        (*iCT)[j+12] = S[18 + 60 + j];
        j++;
    }
}

void internalStateToZero(char **S){
    int j = 0;
    while (j < 4){
        (*S)[15 + j] = 0;
        (*S)[16 + 19 + j] = 0;
        (*S)[17 + 39 + j] = 0;
        (*S)[18 + 60 + j] = 0;
        j++;
    }
}

/******************************************************/
/******************************************************/
//Panther
/******************************************************/
/******************************************************/

void initialise(char *key, char *IV, char **S){
    
    for (int j = 0; j < 32; j++)
        (*S)[j] = key[j];
    
    for (int i = 0; i < 32; i++)
        (*S)[i+32] = IV[i];
    
    for (int i = 0; i < 16; i++)
        (*S)[i+64] = (key[i] ^ (char)15);
    
    (*S)[80] = (char)15;
    (*S)[81] = (char)14;
}

char** encrypt(char *key, char *IV, char *AD, char **PT, int hashlen, int ptlen){
    //INITIALISATION CIPHER AND TAG
    char **CTT = (char **)malloc((ptlen+hashlen)*sizeof(char*)); //the first ptlen entries are the cipher text, the next hashlen are the tag
    
    for (int i = 0; i<(ptlen+hashlen); i++){
        CTT[i] = (char*)malloc(sizeof(char)*16);
    }
    
    //INITIALISATION PHASE
    char *state;
    initState(&state);
    initialise(key, IV, &state);
    F(&state, 92);
    
    //PLAINTEXT PROCESSING
    int ct = 0;
    for (int i = 0; i < ptlen ; i++){
        addToInternalState(&state, PT[i]);
        copyInternalState(&(CTT[i]), state);
        if(i < (ptlen - 1))
            F(&state, 4);
    }
    
    //FINALISATION PHASE
    F(&state, 92);
    for (int i = 0; i < hashlen; i++){
        copyInternalState(&CTT[i + ptlen], state);
        F(&state,4);
    }
    freeState(&state);
    
    return CTT;
}

/******************************************************/
/******************************************************/
//Attaques
/******************************************************/
/******************************************************/

char* keyrecovery(char **PT, char **CT){
    char *k =  (char *)malloc(32*sizeof(char));
    char *stateToInv;
    initState(&stateToInv);
    for (int i = 0; i < 6; i++){
        addToInternalState(&stateToInv, CT[i]);
        if (i < 5){
            for (int j = 0; j < 4; j++){
                shiftState(&stateToInv);
            }
        }
    }
    for (int i = 5; i > -1; i--){
        addToInternalState(&stateToInv,PT[i]);
        if (i > 0)
            invF(&stateToInv, 4);
        if(i == 0)
            invF(&stateToInv, 92);
    }
    for (int i = 0; i < 32; i++){
        k[i] = stateToInv[i];
    }
    return k;
}

char** plainrecovery(char **CT, int ptlen){
    char **recoveredPT =  (char **)malloc((ptlen-6)*sizeof(char*));
    for (int i = 0; i < ptlen - 6; i++){
        recoveredPT[i] = (char *)malloc(16*sizeof(char));
    }
    
    char *state;
    initState(&state);
    for (int i = 0; i < 6; i++){
        addToInternalState(&state, CT[i]);
        if (i < 5){
            for (int j = 0; j < 4; j++){
                shiftState(&state);
            }
        }
    }
    for(int i = 0; i < ptlen - 6; i++){
        F(&state,4);
        addToInternalState(&state, CT[i + 6]);
        copyInternalState(&(recoveredPT[i]),state);
        addToInternalState(&state, CT[i + 6]);
        addToInternalState(&state, recoveredPT[i]);
    }
    return recoveredPT;
 }
 
char** forge(char **CT, int ptlen){
    char **forgedCT =  (char **)malloc((ptlen)*sizeof(char*));
    for (int i = 0; i < ptlen; i++){
        forgedCT[i] = (char *)malloc(16*sizeof(char));
        for (int j = 0; j < 16; j++)
            forgedCT[i][j] = CT[i][j];
    }
    for (int j = 0; j < 16; j++)
        forgedCT[0][j] = (char)(0);
    
    return(forgedCT);
 }

int checkforge(char *k, char *IV, char **forge, char **T, int ptlen){
    //INITIALISATION CIPHER AND TAG
     char **newT = (char**)malloc(4*sizeof(char*));
     for (int i = 0; i < 4; i++){
             newT[i] = (char *)malloc(16*sizeof(char));
     }
     
     //INITIALISATION PHASE
     char *state;
     initState(&state);
     initialise(k, IV, &state);
     F(&state, 92);
    
     //PLAINTEXT PROCESSING
     int ct = 0;
     for (int i = 0; i < ptlen ; i++){
         internalStateToZero(&state);
         addToInternalState(&state, forge[i]);
         if(i < (ptlen - 1))
             F(&state, 4);
     }
    
     //FINALISATION PHASE
     F(&state, 92);
     for (int i = 0; i < 4; i++){
         copyInternalState(&newT[i],state);
         F(&state,4);
     }
     freeState(&state);

     for(int i = 0; i < 4; i++){
         for (int j = 0; j <16; j++){
             if (T[i][j] != newT[i][j])
                 return 0;
         }
     }
     return 1;
 }
