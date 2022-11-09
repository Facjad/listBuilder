#ifndef __CSVPARSER_H
#define __CSVPARSER_H

#include "miniesAndRules.h"

/* Opens a csv file and reads the rules it contains.
fileName : name of the csv file (without the ".csv") TODO : add the possibility to precise ".csv"
points the number of points we want to get the rules from.
rules : the struct in which to write the rules. If some cell is empty in the csv, the value used for the rules is -1.
Returns -1 if file could not be opened, -2 if file could be opened but the requested number of points was not found, -3 for another error, and 0 if everything went good.
*/
int readRulesFile(char * fileName, int points, struct rules * rules);

/* Opens a csv file and reads the minies it contains. Allocates memory.
fileName : name of the csv file (without the ".csv") TODO : add the possibility to precise ".csv"
minies : a pointer that after execution will contain the list of minies (its a pointer to a list of struct mini)
Returns: -1 if file could not be opened. The number of found minies if the file could be opened (0 if the file does not contain any mini)
*/
int readMiniesFile(char * fileName, struct mini ** minies);

void freeMiniesList(struct mini ** minies, int size);

#endif