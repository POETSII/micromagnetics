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
 *  <SomeTag>{{ ./b/c.txt }}</SomeTag>>
 *
 * And where c.txt contains (with no newline at EOF):
 *
 *  Hello World!
 *  This is a multiline file
 *  {{ ./d.c }}
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

#define MOUSTACHE_START {{
#define MOUSTACHE_END }}

int template(char*, char*);

/* This function performs templating on the file at "in", and writes it to a
 * file at "out". Both character arrays must be null-terminated.
 *
 * If there are no moustaches in the file at "in", the file is simply copied to
 * "out".
 *
 * Returns 0 if no errors occured, and 1 otherwise. */
int template(char* in, char* out)
{
    FILE* inputFile;
    FILE* outputFile;
    int thisChar;
    /*int previousChar;*/
    int error;

    error = 0;

    /* Open the input and output files. */
    inputFile = fopen(in, "rb+");
    if (inputFile == NULL)
    {
        fprintf(stderr, "Error opening input file '%s'.\n", in);
        return 1;
    }

    outputFile = fopen(out, "wb");
    if (outputFile == NULL)
    {
        fprintf(stderr, "Error opening output file '%s'.\n", out);
        fclose(inputFile);
        return 1;
    }

    /* Read until we hit EOF in the input file. */
    thisChar = fgetc(inputFile);
    /*previousChar = 0;*/
    while (thisChar != EOF)
    {
        /* Write character to output file. */
        if (fputc(thisChar, outputFile) == EOF)
        {
            fprintf(stderr, "Error writing to output file '%s'.\n", out);
            error = 1;
            break;
        }

        /* Next loop setup */
        /*previousChar = thisChar;*/
        thisChar = fgetc(inputFile);
    }

    /* Cleanup. */
    fclose(inputFile);
    fclose(outputFile);

    return error;
}

int main(int argc, char** argv)
{
    return template(argv[1], argv[2]);
}
