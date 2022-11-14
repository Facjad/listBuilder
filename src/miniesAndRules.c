#include <stdio.h>
#include <string.h>
#include "miniesAndRules.h"

#define MAXSTRINGLENGTH 110 // Is enough to contain ", unique, leader, battleline, behemoth, artillery, artefact, triumph, allied, endless spell or invocation"

const struct mini defaultMini = {"", 0, false, false, false, false, false, false, false, false, false};

void printRules(struct rules * rules){
    printf(" - %d points -\n", rules->points);
    printf(" - Maximum number of leaders: "); (rules->maxLeader < 0) ? printf("unlimited") : printf("%d", rules->maxLeader); printf(" \n");
    printf(" - Mininmum number of battleline units: %d \n", rules->minBattleline);
    printf(" - Maximum number of behemoth units: "); (rules->maxBehemoth < 0) ? printf("unlimited") : printf("%d", rules->maxBehemoth); printf(" \n");
    printf(" - Maximum number of artillery units: "); (rules->maxArtillery < 0) ? printf("unlimited") : printf("%d", rules->maxArtillery); printf(" \n");
    printf(" - Maximum number of artefacts: "); (rules->maxArtefact < 0) ? printf("unlimited") : printf("%d", rules->maxArtefact); printf(" \n");
    printf(" - Maximum number of triumphs: "); (rules->maxTriumph < 0) ? printf("unlimited") : printf("%d", rules->maxTriumph); printf(" \n");
    printf(" - Maximum number of spells: "); (rules->maxSpell < 0) ? printf("unlimited") : printf("%d", rules->maxSpell); printf(" \n");
}

void printMini(struct mini * mini){
    printf("%s: %d points", mini->warscroll, mini->points);
    char other[MAXSTRINGLENGTH];
    other[0] = '\0';
    if (mini->unique) {
        strcat(other, ", unique");
    }
    if (mini->leader) {
        strcat(other, ", leader");
    }
    if (mini->battleline) {
        strcat(other, ", battleline");
    }
    if (mini->behemoth) {
        strcat(other, ", behemoth");
    }
    if (mini->artillery) {
        strcat(other, ", artillery");
    }
    if (mini->artefact) {
        strcat(other, ", artefact");
    }
    if (mini->triumph) {
        strcat(other, ", triumph");
    }
    if (mini->allied) {
        strcat(other, ", allied");
    }
    if (mini->spell) {
        strcat(other, ", endless spell or invocation");
    }
    printf("%s.", other);
}

void printMinies(struct mini * mini, int size){
    for (int i = 0; i < size; i++){
        printMini(mini+i);
        printf("\n");
    }
    //printf("\n");
}