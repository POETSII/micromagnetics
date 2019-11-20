#include "template.h"

/* e.g. ./template "input_file" "output_directory/output_file" */

int main(int argc, char** argv)
{
    FILE* inFile;
    FILE* outFile;
    int rc;

    /* Open the input and output files. */
    inFile = fopen(argv[1], "rb+");
    if (inFile == NULL)
    {
        fprintf(stderr,
                "Error opening input file '%s': %s.\n",
                argv[1], strerror(errno));
        return 1;
    }

    outFile = fopen(argv[2], "wb");
    if (outFile == NULL)
    {
        fprintf(stderr,
                "Error opening output file '%s': %s.\n",
                argv[2], strerror(errno));
        fclose(inFile);
        return 1;
    }

    /* Use templating engine. */
    rc = template(inFile, outFile);

    /* Cleanup. */
    fclose(inFile);
    fclose(outFile);
    return rc;
}
