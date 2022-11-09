#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "csvParser.h"

#define MAX_LINE_SIZE 100 // Enough to read first line, should be enough for any line
#define MAX_CELL_SIZE 10 // Enough to read one cell. "999999999" takes 10 characters, I doubt any cell will contain TODO : wait, what ??

#define START_SIZE 10 // Starting size of a list of minies

/* Returns static string containing next cell
Similar to strtok: subsequent calls on the same line should be done with str == NULL.
*/
char * readNextCell(char * str){
    static char strToParse[MAX_LINE_SIZE];
    static int strIndex;
    static char cell[MAX_CELL_SIZE]; // Value to return.
    int cellIndex;

    // Initialise values (some only if new str is provided)
    cellIndex = 0;
    if (str != NULL){
        strncpy(strToParse, str, MAX_LINE_SIZE);
        strIndex = 0;
    }

    // Parse next cell
    if (strToParse[strIndex] == '\0') {
        return NULL;
    }

    for ( ; strToParse[strIndex] != ';' && strToParse[strIndex] != '\0' && strIndex < MAX_LINE_SIZE && cellIndex < MAX_CELL_SIZE-1; strIndex++, cellIndex++){
        cell[cellIndex] = strToParse[strIndex];
    }
    cell[cellIndex] = '\0';
    strIndex++;
    return cell;
}

/* Returns the integer from a cell, or -1 if cell is empty
*/
int cellContent(char* cell){
    if (cell[0] == '\0') {
        return -1 ;
    } else {
        return atoi(cell) ;
    }
}

// -----
// Functions from header
// -----

int readRulesFile(char * fileName, int points, struct rules * rules){
    // Append ".csv" to fileName
    char fileNameWithCsv[strlen(fileName)+1+4]; // ".csv" takes 4 characters
    strcat(strcpy(fileNameWithCsv, fileName), ".csv");

    // Open file
    FILE * rulesFile = fopen(fileNameWithCsv, "r");
    if (rulesFile == NULL) {
      return -1;
    }

    char buffer[MAX_LINE_SIZE];
    char * cell;

    // Find right line
    bool lineFound = false;
    while(fgetc(rulesFile) != '\n'); //Skip first line
    while (!lineFound){
        if (fgets(buffer, MAX_LINE_SIZE, rulesFile) == NULL){
            return -2;
        }
        cell = readNextCell(buffer);

        int pointsOfLine = atoi(cell);
        if (pointsOfLine == points){
            lineFound = true;
            rules->points = points;
        }
    }

    // Parse found line
    cell = readNextCell(NULL);
    rules->maxLeader = cellContent(cell);
    cell = readNextCell(NULL);
    rules->minBattleline = cellContent(cell);
    cell = readNextCell(NULL);
    rules->maxBehemoth = cellContent(cell);
    cell = readNextCell(NULL);
    rules->maxArtillery = cellContent(cell);
    cell = readNextCell(NULL);
    rules->maxArtefact = cellContent(cell);
    cell = readNextCell(NULL);
    rules->maxTriumph = cellContent(cell);
    cell = readNextCell(NULL);
    rules->maxSpell = cellContent(cell);

    fclose(rulesFile);

    return 0;
}

int readMiniesFile(char * fileName, struct mini ** minies){
    // Append ".csv" to fileName
    char fileNameWithCsv[strlen(fileName)+1+4]; // ".csv" takes 4 characters
    strcat(strcpy(fileNameWithCsv, fileName), ".csv");

    // Open file
    FILE * rulesFile = fopen(fileNameWithCsv, "r");
    if (rulesFile == NULL) {
      return -1;
    }

    char buffer[MAX_LINE_SIZE];
    char * cell;
    int currentSize = START_SIZE;
    * minies = (struct mini *) malloc(sizeof(struct mini)*currentSize);
    int foundMinies = 0;

    while(fgetc(rulesFile) != '\n'); //Skip first line
    for (foundMinies = 0; fgets(buffer, MAX_LINE_SIZE, rulesFile) != NULL; foundMinies++){
        printf("%d\n", foundMinies);
        if (foundMinies >= currentSize){
            currentSize *=2;
            * minies = (struct mini *) realloc(* minies, sizeof(struct mini)*currentSize);
        }
        cell = readNextCell(buffer);
        (* minies)[foundMinies].warscroll = (char *) malloc((strlen(cell)+1)*sizeof(char));
        strcpy((* minies)[foundMinies].warscroll, cell);
        cell = readNextCell(NULL);
        (* minies)[foundMinies].points = cellContent(cell);
        cell = readNextCell(NULL);
        //if (strcmp(cell, )){
            
        //}
    }

    * minies = (struct mini *) realloc(* minies, sizeof(struct mini)*foundMinies);
    return foundMinies;
}

void freeMiniesList(struct mini ** minies, int size){
    // Beware of freeing also the char* (warscroll)
    for (int i = 0; i < size; i++){
        free((*minies)[i].warscroll);
    }
    free(*minies);
}