/* A small mustache-inspired templating library, written by MLV, because Jinja
 * doesn't exist in C.
 *
 * Basically, loads a file, performs substitutions based on mustache ({{ and
 * }}) presence, and writes the file somewhere else.
 *
 * Example: Lets say you have the following filesystem hierarchy:
 *  - ./a.xml
 *  - ./b/c.txt
 *
 * Where ./a.xml contains:
 *
 *  <SomeTag>{{./b/c.txt}}</SomeTag>>
 *
 * And where c.txt contains (with no newline at EOF):
 *
 *  Hello World!
 *  This is a multiline file
 *  {{./d.c}}
 *
 * And where d.c contains (with a newline at EOF):
 *
 *  Quack quack jibber jibber
 *
 * When a.xml is parsed by this templating library, it writes the following to
 * a file (somewhere specified by the user):
 *
 *  <SomeTag>Hello World!
 *  This is a multiline file
 *  Quack quack jibber jibber
 *  </SomeTag>
 *
 * Pretty simple, no?
 *
 * There's very little in here that protects the user, so be careful! */

#include <stdio.h>

#define MOUSTACHE_OPEN_CODE 123  /* '{' */
#define MOUSTACHE_CLOSE_CODE 125  /* '}' */
#define MOUSTACHE_BUFFER_SIZE 1000

int template(char*, char*);

/* This function performs templating on the file at "in", and writes it to a
 * file at "out". Both character arrays must be null-terminated.
 *
 * If there are no moustaches in the file at "in", the file is simply copied to
 * "out".
 *
 * Returns 0 if no errors occured, and 1 otherwise. */
int template(char* inPath, char* outPath)
{
    /* Files */
    FILE* inFile;
    FILE* outFile;

    /* Characters */
    int thisChar;
    int previousChar;

    /* Moustache mode */
    int moustacheMode;
    char moustacheBuffer[MOUSTACHE_BUFFER_SIZE];
    int moustacheIndex;

    /* Reporting */
    int error;

    /* Initialisation */
    moustacheIndex = 0;
    moustacheMode = 0;
    error = 0;

    /* Open the input and output files. */
    inFile = fopen(inPath, "rb+");
    if (inFile == NULL)
    {
        fprintf(stderr, "Error opening input file '%s'.\n", inPath);
        return 1;
    }

    outFile = fopen(outPath, "wb");
    if (outFile == NULL)
    {
        fprintf(stderr, "Error opening output file '%s'.\n", outPath);
        fclose(inFile);
        return 1;
    }

    /* Read until we hit EOF in the input file. */
    thisChar = fgetc(inFile);
    previousChar = 0;
    while (thisChar != EOF)
    {
        /* Are we currently in moustache mode? */
        if (moustacheMode == 1)
        {
            /* Has the moustache ended? */
            if (thisChar == MOUSTACHE_CLOSE_CODE &&
                previousChar == MOUSTACHE_CLOSE_CODE)
            {
                /* If so, exit moustache mode, remove the stray curly brace
                 * from the buffer, and print the moustache buffer to
                 * stdout. */
                moustacheMode = 0;
                moustacheBuffer[moustacheIndex - 1] = EOF;
                printf("Found moustache: '%s'\n", moustacheBuffer);
            }

            /* Otherwise, store the character in the moustache buffer. */
            moustacheBuffer[moustacheIndex++] = thisChar;
        }

        else
        {
            /* Has a moustache started? */
            if (thisChar == MOUSTACHE_OPEN_CODE &&
                previousChar == MOUSTACHE_OPEN_CODE)
            {
                /* Enter moustache mode. */
                moustacheMode = 1;

                /* Clear the moustache buffer. */
                for (moustacheIndex = 0;
                     moustacheIndex < MOUSTACHE_BUFFER_SIZE;
                     moustacheBuffer[moustacheIndex++] = EOF);
                moustacheIndex = 0;

                /* If so, remove the previous curly brace by moving the write
                 * pointer backwards by one. */
                fseek(outFile, -1, SEEK_CUR);
            }

            /* Otherwise, write the character to output file. */
            else
            {
                if (fputc(thisChar, outFile) == EOF)
                {
                    fprintf(stderr, "Error writing to output file '%s'.\n",
                            outPath);
                    error = 1;
                    break;
                }
            }
        }

        /* Next loop setup */
        previousChar = thisChar;
        thisChar = fgetc(inFile);
    }

    /* Cleanup. */
    fclose(inFile);
    fclose(outFile);

    return error;
}

int main(int argc, char** argv)
{
    return template(argv[1], argv[2]);
}
