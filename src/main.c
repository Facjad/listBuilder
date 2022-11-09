#include <stdio.h>
#include <string.h>

#include <stdlib.h>

#include "miniesAndRules.h"
#include "csvParser.h"
#include "listGenerator.h"

#define POINTS 10 //TODO : to remove later
#define RULESFILE "rules" //TODO : to remove later
#define MINIESFILE "minis" //TODO : to remove later

int main (int argc, char ** argv) {
    struct rules rules;
    
    char rulesFile[strlen(RULESFILE)+1];
    strcpy(rulesFile, RULESFILE);

    int fileok = readRulesFile(rulesFile, POINTS, &rules);
    if (fileok == -1) {
        printf("Problem opening file. Did you provide the correct file name?\n");
        return 1;
    } else if (fileok == -2){
        printf("In the rules file, the line for %d points could not be found.\n", POINTS);
        return 1;
    } else if (fileok < 0){
        printf("Unidentified problem with rules file.\n");
        return 1;
    }
    printf("The following rules will be used:\n");
    printRules(&rules);
    
    struct mini * minies;
    fileok = readMiniesFile(MINIESFILE, &minies);
    if (fileok == -1) {
        printf("Minies file could not be opened\n");
        return 1;
    } else if (fileok < 0) {
        printf("Problem with minies reader.\n");
        return 1;
    }
    printf("fileok: %d\n", fileok);
    printMinies(minies, fileok);
    generateLists(rules, minies);
    freeMiniesList(&minies, fileok);
    return 0;
}