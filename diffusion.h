//
//  diffusion.h
//  
//
//  Created by Rachelle Heim on 24/01/2022.
//

#ifndef diffusion_h
#define diffusion_h

#include <stdio.h>
#include <stdlib.h>

#define n 82

void printTab(char **Tab);
void Li_becomes_Li_or_Lj(char ***Tab, int i, int j);
void Li_becomes_Lj(char ***Tab, int i, int j);

#endif /* diffusion_h */
