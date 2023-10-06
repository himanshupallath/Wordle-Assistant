/*
Description: This is the header file for Wordle.c which holds all the prototypes for all the for
             the wordle functionality
Sources used for this project:
 1. Lecture Notes from the C Unit
 2. Dr. Plaue's Linked List Demo
 3. My original Colab-06 C code
 4. The C section in Cplusplus.com
    - Anon. Tutorials. Retrieved December 3, 2022 from https://cplusplus.com/ 
*/
#ifndef WORDLE_H
#define WORDLE_H

int loadDictionary(char *[]);   // Opens up the library & finds 5 letter words
char * promptUser(void);        // Gets user input
void processGuess(char *[], int num5, char, char, char, char, char);  // Finds viable words
void printResults(char *[], int);       // Prints all the viable words
void helpMe(void);      // Runs the whole program
int checkUpper(char []);    // Helper function for finding words with capital letters
#endif