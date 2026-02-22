/* 
AVIS:
    artifact: main.c
    role: program-entry
    family: cyhy-cmt
    version: 1.0
    description: Entry point for cyhy-cmt using Fire‑Gem kernel
*/

#include "fire-gem.h"
#include <stdio.h>

// Example drop‑in module
void SeedModule(void) {
    AVIS_Log("SeedModule: Running inside Fire‑Gem.");
    printf("SeedModule executed.\n");
}

int main(void) {
    FireGem_Register(SeedModule);
    FireGem_Start();
    return 0;
}
