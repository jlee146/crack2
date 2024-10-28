#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md5.h"

const int PASS_LEN = 20;        // Maximum any password will be
const int HASH_LEN = 33;        // Length of MD5 hash strings


// Given a target plaintext word, use it to try to find
// a matching hash in the hashFile.
// Get this function working first!
char * tryWord(char * plaintext, char * hashFilename)
{
    // Hash the plaintext
    char *hashed_plaintext = md5(plaintext, strlen(plaintext)); // I used chatgpt for this line, initially I passed in PASS_LEN for the length but it was wrong

    // Open the hash file
    FILE *inFile = fopen(hashFilename, "r"); // we're only reading from the file

    // Verify that the file was able to open
    if (inFile == NULL)
    {
        printf("Could not open file...\nProgram now exiting...\n");
        exit(1);
    }

    char line[HASH_LEN];
    // Loop through the hash file, one line at a time.
    while(fgets(line, 255, inFile) != NULL)
    {
        // Attempt to match the hash from the file to the
        // hash of the plaintext.
        if (strcmp(hashed_plaintext,line) != 0)
            // If there is a match, you'll return the hash.
            fclose(inFile);
            return hashed_plaintext;
    }

    // Before returning, do any needed cleanup:
    //   Close files?
    //   Free memory?
    free(hashed_plaintext);
    fclose(inFile);

    // If not, return NULL.
    return NULL;
}


int main(int argc, char *argv[])
{
    if (argc < 3) 
    {
        fprintf(stderr, "Usage: %s hash_file dict_file\n", argv[0]);
        exit(1);
    }

    // These two lines exist for testing. When you have
    // tryWord working, it should display the hash for "hello",
    // which is 5d41402abc4b2a76b9719d911017c592.
    // Then you can remove these two lines and complete the rest
    // of the main function below.
    // char *found = tryWord("hello", "hashes00.txt");
    // printf("%s %s\n", found, "hello");


    // Open the dictionary file for reading.
    FILE *dictionary = fopen(argv[2], "r");

    // Verify that the file was able to open
    if (dictionary == NULL)
    {
        printf("Could not open file...\nProgram now exiting...\n");
        exit(1);
    }

    // For each dictionary word, pass it to tryWord, which
    // will attempt to match it against the hashes in the hash_file.
    char line[PASS_LEN + 1];
    int count = 0;
    while(fgets(line, sizeof(line), dictionary) != NULL)
    {
        // remove the new line character
        line[strcspn(line, "\n")] = 0;
        
        // Hash the plaintext
        char *found = tryWord(line, argv[1]);

        // If we got a match, display the hash and the word. For example:
        //   5d41402abc4b2a76b9719d911017c592 hello
        if (found)
        {
            printf("%s %s\n", found, line);
            count++;
            free(found);
        }
    }

    // Close the dictionary file.
    fclose(dictionary);

    // Display the number of hashes that were cracked.
    printf("%d hashes cracked!\n", count);
    // Free up any malloc'd memory?
}

