#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "csvParser.h"

//#define MAX_LINE_SIZE 100 // Enough to read first line, should be enough for any line
#define MAX_CELL_SIZE 50 // Enough to read one cell. "999999999" takes 10 characters, I doubt any cell will contain TODO : wait, what ??

#define START_SIZE 10 // Starting size of a list of minies

/* Reads the next cell in the file. After returning from this function, the next available character is the first chracter of the next cell, or 
the new line character if there is none.
   This function ensures the pointer in the file remains on the same line. To go to next line, use skipLine.
   Returns a static string containing that next cell, or NULL if no cell is left on current line. (note NULL -> no cell left ; "" -> empty cell found)
*/
char * readNextCell(FILE * csvFile){
    static char cell[MAX_CELL_SIZE]; // Value to return.
    int cellIndex = 0;
    int nextChar = fgetc(csvFile);

    // Special cases
    if (nextChar == '\n'){
        ungetc('\n', csvFile); // Avoid moving to the next line
        return NULL;
    } else if (nextChar == EOF) {
        return NULL;
    }

    // Parse next cell
    for ( ; nextChar != ';' && nextChar != '\n' && nextChar != EOF ; nextChar = fgetc(csvFile)){
        if(cellIndex < MAX_CELL_SIZE-1) {
            cell[cellIndex] = nextChar;
            cellIndex++;
        }
    }
    cell[cellIndex] = '\0';

    if (nextChar == '\n'){
        ungetc('\n', csvFile); // Avoid moving to the next line
    }
    
    return cell;
}

/* Skip line, and returns a static string containing the beginning of some eventual remaining content in the line.
Returns NULL if no content was remaining.*/
char * skipLine(FILE * csvFile){
    char * nextCell = readNextCell(csvFile);
    while(fgetc(csvFile) != '\n'); // Skip remaining of line
    return nextCell;
}

/* Returns the integer from a cell, or valueIfEmpty if cell is empty
*/
int parseIntCell(char* cell, int valueIfEmpty){
    if (cell == NULL || cell[0] == '\0') {
        return valueIfEmpty ;
    } else {
        return atoi(cell) ;
    }
}

/* Returns false if cell is empty (NULL or "") or is either "no" or "0". Otherwise, defaults to true.
*/
bool parseYesNoCell(char* cell){
    if (cell == NULL || strcmp(cell, "no") == 0 || strcmp(cell, "0") == 0 || strcmp(cell, "") == 0) {
        return false;
    } else {
        return true;
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

    char * cell;

    // Find right line
    skipLine(rulesFile);
    bool lineFound = false;
    while (!lineFound){
        cell = readNextCell(rulesFile);
        if (cell == NULL){
            return -2;
        }

        int pointsOfLine = atoi(cell);
        if (pointsOfLine == points){
            lineFound = true;
            rules->points = points;
        } else {
            skipLine(rulesFile);
        }
    }

    // Parse found line
    cell = readNextCell(rulesFile);
    rules->maxLeader = parseIntCell(cell, -1);
    cell = readNextCell(rulesFile);
    rules->minBattleline = parseIntCell(cell, 0);
    cell = readNextCell(rulesFile);
    rules->maxBehemoth = parseIntCell(cell, -1);
    cell = readNextCell(rulesFile);
    rules->maxArtillery = parseIntCell(cell, -1);
    cell = readNextCell(rulesFile);
    rules->maxArtefact = parseIntCell(cell, -1);
    cell = readNextCell(rulesFile);
    rules->maxTriumph = parseIntCell(cell, -1);
    cell = readNextCell(rulesFile);
    rules->maxSpell = parseIntCell(cell, -1);

    cell = skipLine(rulesFile);
    if (cell != NULL){
        printf("Warning : some content has been ignored, starting with %s", cell);
    }

    fclose(rulesFile);

    return 0;
}

int readMiniesFile(char * fileName, struct mini ** minies){
    // Append ".csv" to fileName
    char fileNameWithCsv[strlen(fileName)+1+4]; // ".csv" takes 4 characters
    strcat(strcpy(fileNameWithCsv, fileName), ".csv");

    // Open file
    FILE * miniesFile = fopen(fileNameWithCsv, "r");
    if (miniesFile == NULL) {
      return -1;
    }

    char * cell;
    int currentSize = START_SIZE;
    * minies = (struct mini *) malloc(sizeof(struct mini)*currentSize);
    int foundMinies = 0;

    skipLine(miniesFile);
    for (foundMinies = 0, cell = readNextCell(miniesFile) ; cell != NULL ; foundMinies++, cell = readNextCell(miniesFile)){
        if (foundMinies >= currentSize){
            currentSize *=2;
            * minies = (struct mini *) realloc(* minies, sizeof(struct mini)*currentSize);
        }

        // Warscroll
        // NB : First cell has already been read (see declaration of this for loop)
        (* minies)[foundMinies].warscroll = (char *) malloc((strlen(cell)+1)*sizeof(char));
        strcpy((* minies)[foundMinies].warscroll, cell);
        // Points
        cell = readNextCell(miniesFile);
        (* minies)[foundMinies].points = parseIntCell(cell, 0);
        // Unique
        cell = readNextCell(miniesFile);
        (* minies)[foundMinies].unique = parseYesNoCell(cell);
        // Leader
        cell = readNextCell(miniesFile);
        (* minies)[foundMinies].leader = parseYesNoCell(cell);
        //Battleline
        cell = readNextCell(miniesFile);
        (* minies)[foundMinies].battleline = parseYesNoCell(cell);
        // Behemoth
        cell = readNextCell(miniesFile);
        (* minies)[foundMinies].behemoth = parseYesNoCell(cell);
        // Artillery
        cell = readNextCell(miniesFile);
        (* minies)[foundMinies].artillery = parseYesNoCell(cell);
        // Artefact
        cell = readNextCell(miniesFile);
        (* minies)[foundMinies].artefact = parseYesNoCell(cell);
        // Triumph
        cell = readNextCell(miniesFile);
        (* minies)[foundMinies].triumph = parseYesNoCell(cell);
        // Allied
        cell = readNextCell(miniesFile);
        (* minies)[foundMinies].allied = parseYesNoCell(cell);
        // Spell
        cell = readNextCell(miniesFile);
        (* minies)[foundMinies].spell = parseYesNoCell(cell);

        cell = skipLine(miniesFile);
        if (cell != NULL){
            printf("Warning : some content has been ignored, starting with %s", cell);
        }
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