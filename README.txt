
This repository contains : 
I) a program which implements 3 different attacks on the authenticated encryption scheme Panther. 
II) a program which computes the minimum number of rounds n_r for the state update function of Panther to ensure good diffusion. 



I) Attacks on Panther 

1) Compiling and running instructions :

To run this code, type in 

make run

in your terminal in the folder where all the .c and the .h files are. 

2) What the program does :

This code implements 3 different attacks on the authenticated encryption scheme Panther.

First, the program generates a random 128-bits key, a random 128-bits initial value (IV) and a random 512-bits plaintext. Note that the IV is also a nonce. Since Panther has a rate of 64 bits, the plaintext will be processed in 8 blocks. The code does not generate associated data as our attacks work regardless. The key, IV and plaintext are printed by the program. 

The plaintext is then encrypted with the key and IV. The corresponding ciphertext and tag are returned. Both are also printed by our program. 

The program then runs our three attacks. 

a -  Key recovery

First, we implemented a function that takes as input the plaintext/ciphertext pair and returns the secret key. Once a key is recovered, the program prints it. If the recovered key matches the random one used to encrypt the ciphertext, the program prints the following message : 

*********************************
**** Key recovery successful **** 
*********************************

b - Plaintext recovery

Second, we implemented a function that takes as input the ciphertext and returns all plaintext blocks but the first six. Once plaintext blocks are recovered, the program prints it. If they match the actual plaintext encrypted, the program prints the following message : 

 ****************************************
 **** Plaintext recovery successful **** 
 ****************************************

c - Forge 

Lastly, we implemented a function that takes as input the ciphertext and returns a forged ciphertext which has the same tag. Once a forged ciphertext is outputted, the program prints it. We then implemented a function that checks whether the forged ciphertext is valid. This function takes as input the key and the IV. It works as a decryption function on the forged ciphertext and returns the valid 128-bit tag for the forged ciphertext. If the forged ciphertext tag matches the initial ciphertext tag, the program prints the following message : 

 ******************************************************
 ********* The tag is valid, forge successful ********* 
 ******************************************************
 






II) How to ensure good diffusion of the state update function

This program computes the minimum number of rounds needed for the state update function of Panther to have good diffusion, that is, the minimum number of rounds such that all output bits depend on all input bits. To run it, type in 

gcc diffusion.c -c exec
./exec

in your terminal, in the folder where the files diffusion.c and diffusion.h are stored. 
