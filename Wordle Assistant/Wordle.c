/*
Description: This is Wordle.c file where I added the functionality to all of the prototypes
             that was in the Wordle.h file. 
*/
#include <stdio.h>      // Used for printing a fgets
#include <stdlib.h>     // Used for malloc
#include <string.h>     // Used for string manipulation
#include <ctype.h>      // Used for toupper, tolower, isupper and islower
#include "Wordle.h"     // Including the header file 

/*
Description: This is a method which opens up a dictionary file. It reads through the whole file
             and adds words that are of length 5 and doesn't have any apostrophes into a array.
             The return value is the number of 5 letter words that is in the array
*/
int loadDictionary(char *words[]) {
    puts("Processing Dictionary ");
    FILE *file;
    char dictWord[20];
    int size5 = 0; int sizeDict = 0;
    int i = 0;
    file = fopen("/usr/share/dict/words", "r");     // Opening the dictionary file
    // Allocating memory for each index in the array holding the 5 letter words
    for (i = 0; i < 7000; i++) {
        words[i] = (char *)malloc(sizeof(char) * 6);
    }
    // Loops through the whole file until the file reaches the end
    while(!feof(file)) {
        sizeDict++;
        fgets(dictWord, 20, file);      // Gets the words from the dictionary and adds it to a char array
        if (strlen(dictWord) == 6) {    // Checks if it is 5 letters long
            int j = 0; int check = 0;
            // Checks for punctuation especially apostrophees
            for (j = 0; j < 6; j++) {       
                if (ispunct(dictWord[j])) {
                    check = 1;
                }
            }
            // Adds the word to the list if no punctuation was found
            if (check != 1) {
                strcpy(words[size5], dictWord);
                size5++;        
            }
        }
    }
    fclose(file);       // Closes file
    // Prints the size of 5 words array and dictionary
    printf("%d out of %d words in the dictionary are 5 characters! \n\n", size5, sizeDict-51);
    return size5;
}

/*
Description: This method gets the user's input with capital letter (Green Letter), lowercase letter 
             (Yellow letter) and * for unknown letter. The user inputs the letters separated with
             spaces and this method takes that and strips the whitespaces
             The return type is a pointer to a char array of the user input
*/
char * promptUser(void) {
    puts("Enter the Wordle clues using the following scheme: ");
    puts("         A-Z for a known letter in the right position ");
    puts("         a-z for a letter that is correct in the wrong position");
    puts("         * for unknown letter");
    puts("         Separate each with a space (e.g., T * G i R)");
    char input[10];

    printf("Enter your query now: ");   // Getting the user input
    fgets(input, 10, stdin);
    int i = 0;
    char *inputNoSpaces = (char *)malloc(sizeof(char) * 6);     // Array that holds the input w/o spaces
    int j = 0;
    for (i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            (inputNoSpaces[j]) = input[i];  // Adds the values from all the even indexes in the input
            j++;
        }
    }
    return inputNoSpaces;
}

/*
Description: This method takes all the characters from the input, the array holding the 5 letter words and
             the size of the dictionary. It finds all the uppercase letter and see how many words in the
             array has that capital letter in that certain index. Then it will take all the clues given from
             the input and finds all the viable words based on those clues
             There is no return value for the method
*/
void processGuess(char *words[], int num5, char one, char two, char three, char four, char five) {
    int wordNum = 0;
    char word[5] = {one, two, three, four, five};   // Put all the clues into one array for easier accesibility
    char *foundWord[num5];      // The array for all the viable words
    int i; int k;
    // This set of words finds the number of words with the captital letter in that certain index
    // ex. Found 438 words with L in position 3
    for (i = 0; i < 5; i++) {
        if (isupper(word[i])) {     // Checks if the clue is Uppercase (A Green Letter)
            k++; int j;
            for (j = 0; j < num5; j++) {    // Loops through the array of five letter words
                if (word[i] == toupper((words[j][i]))) {    // If the letter matches in the index
                    wordNum++;      
                }
            }
            if (i == 0) {
                // Prompt for the first index
                printf("Found %d words starting with %c\n", wordNum, word[i]);
            }
            else {
                // Prompt for index 2-5
                printf("Found %d words with %c in position %d\n", wordNum, word[i], i+1);
            }
        }
        wordNum = 0;
    }
    // The for loop below just checks for the number of capital letter (Green letters) there are
    // in the user's input
    int numCap = 0;
    for (k = 0; k < 5; k++) {
        if (isupper(word[k])) {
            numCap++;   // Counter increase for every capital letter
        }
    }
    // The for loop below is to find all the viable words based on the user's input and add it to an array
    int boolC = 1; int r = 0; int counter = 0;
    for (k = 0; k < num5; k++) {    // Loops through all the words in the array
        if (checkUpper(words[k]) == 0 && boolC == 1) {      // Weeds out words that have uppercase letters (ex. proper nouns)
            for (r = 0; r < 5; r++) {    // Loops through each clue
                // Checks for the green letters match the word in proper index
                if (isupper(word[r]) && word[r] != '*' && boolC == 1) { 
                    if (words[k][r] == tolower(word[r])) {
                        boolC = 1;      // if word is still viable
                    } else {
                        boolC = 0;      // if word is not viable
                    }
                }
                // Extra credit portion that accounts for lowercase letters to see if they are inside to word
                else if (islower(word[r]) && word[r] != '*' && boolC == 1) {
                    int f; int lowerCheck = 0;
                    for (f = 0; f < 5; f++) {
                        if (f != r && words[k][f] == word[r]) {
                            lowerCheck = 1; // If the word is still viable
                        }
                    }
                    // Check if the yellow clue failed and the word is not viable
                    if (lowerCheck == 0) { boolC = 0; }
                }
            }
            // if the word is viable after checking all clues then we add it to the array
            if (boolC == 1) {
                foundWord[counter] = (char *)malloc(sizeof(char) * 6);  // Allocates memory in the array
                strcpy(foundWord[counter], words[k]);   // Adds the word into the array
                counter++;
            }
        }
        boolC = 1;  // Resets the boolean integer
    }
    // When all the viable letters are found, then they are printed.
    if (numCap > 1) {   // If more than one green letter is inputed, then print out all the options
        printResults(foundWord, counter);
    } else {            // If only one green letter is inputed, then print the prompt below
        puts("There are too many possibilities!");
    }
}

/*
Description: This is a helped method used to check if there are any uppercase letters in the word. We are not using 
             any words with uppercase for our list. I return an integer which will return 0 or 1 with 0 being false 
             and 1 being true.
*/
int checkUpper(char word[]) {
    // 0 = false and 1 = true;
    int i = 0;
    for (i = 0; i < 5; i++) {
        if (isupper(word[i])) {
            return 1;   // return 1 (true) for having a capital letter
        }
    }
    return 0;   // return 0 (false) for having a capital letter
}

/*
Description: This is a method used to print out all the viable words for the given clues the user inputted. It takes
             the char array pointer that holds all the viable words and the size of that array. It doesn't have a 
             return type
*/
void printResults(char *words[], int num5) {
    puts("Viable Options:");
    int i;
    // Loops through the list of viable words
    for (i = 0; i < num5; i++) {
        printf("        %d: %s",i,words[i]);    // prints out each word in a new line
    }
    puts("");
}

/*
Description: This is the method used to run the whole wordle assistant solver. There are no input paramaters and 
             output return values.
*/
void helpMe() {
    puts(" \nWelcome to the Wordle Assistant! ");
    char *words[7000];  // Char array pointer which holds all the 5 letter words from the dictionary
    int dictSize = loadDictionary(words);   // Calling the loadDictionary function with dictSize being the number of 5 letter words
    char *inputWord = (char *)malloc(sizeof(char) * 6);     // Allocating memory for the user input
    strcpy(inputWord,promptUser());     // Adding the user input into the inputWord array
    // This is where we call processGuess to find all the viable words to print
    processGuess(words, dictSize, inputWord[0], inputWord[1], inputWord[2], inputWord[3], inputWord[4]);
}