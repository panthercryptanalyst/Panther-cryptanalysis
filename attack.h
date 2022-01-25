//
//  attack.h
//  
//
//  Created by Rachelle Heim on 20/01/2022.
//

/*Remarks :
 1) The functions plaintextProcessing and encrypt only process plaintexts whose length is a multiple of 64 (= r). Therefore ptlen must be a multiple of 16.
 2) Likewise, the encrypt functions takes as an entry only tag sizes that are multiples of 64 (= r). Hashlen beeing the length in nibbles, it must be dividable by 16.
 3) the function AD processing is not built, thus so far encrypt only works with empty AD
 */

#ifndef attack_h
#define attack_h

#include <stdio.h>
#include "gf16.h"
#include <time.h>

//Panther state
void initState(char **State);
void freeState(char **State);
void printState (char *State); //Prints state in the order P18 -> P0, Q19 -> Q0 and so on
void printState2(char *State); //Prints state in the normal order P0 -> S21

//Internal functions
void matrixMultiplication(char **Vect, char Matrix[4][4]);
void sBox(char **Vect, char *Sbox);
void computesT (char **vectT, char *S);
void computesInv0 (char **vectT, char *S);
void shiftState(char **S);

//State update function
void F(char **State, int nr);
void invF(char **State, int nr);

//Sponge tools
void addToInternalState(char **S, char *I);
void copyInternalState(char **iCT, char *S);
void internalStateToZero(char **S);

//Panther
void initialise(char *key, char *IV, char **S);
char** encrypt(char *key, char *IV, char *AD, char **PT, int hashlen, int ptlen);

//Attaques
char* keyrecovery(char **PT, char **CT);
char** plainrecovery(char **CT, int ptlen);
char** forge(char **CT, int ptlen);

//Vérification
int checkforge(char *k, char *IV, char **forge, char **T, int ptlen);

#endif /* attack_h */
