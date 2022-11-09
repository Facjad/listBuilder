#ifndef __MINI_H
#define __MINI_H

#include <stdbool.h>

/* A field with a negative value means that there is no limit
For example maxLeader = 0 means no leader is allowed, but maxLeader = -1 means there is no leader limit.
*/
struct rules {
    int points;
    int maxLeader;
    int minBattleline;
    int maxBehemoth;
    int maxArtillery;
    int maxArtefact;
    int maxTriumph;
    int maxSpell;
};

struct mini {
    char * warscroll;
    int points;
    bool unique;
    bool leader;
    bool battleline;
    bool behemoth;
    bool artillery;
    bool artefact;
    bool triumph;
    bool allied;
    bool spell;
};

void printRules(struct rules * rules);
void printMini(struct mini * mini);
void printMinies(struct mini * mini, int size);

#endif