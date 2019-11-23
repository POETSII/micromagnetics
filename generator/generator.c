/* Generates micromagnetic XML. */

#include "template.h"

#define DEVICE_INSTANCE_FRAGMENT "fragments/instances/device_instances.xml"
#define EDGE_INSTANCE_FRAGMENT "fragments/instances/edge_instances.xml"
#define MICROMAGNETICS_TEMPLATE "fragments/micromagnetics_template.xml"
#define JSON_BUFFER_SIZE 1000
#define OUTPATH_BUFFER_SIZE 1000

/* Writes some device instances to a file. */
int write_device_instances(unsigned x0Max, char* outPath)
{
    /* Create a properties staging area and a state staging area, used for each
     * iteration. */
    char props[JSON_BUFFER_SIZE];
    char state[JSON_BUFFER_SIZE];
    unsigned bufferIndex;

    /* Co-ordinates. */
    unsigned x0;

    /* Open the fragment to write to, clobbering any existing content. */
    FILE* outFile;
    outFile = fopen(outPath, "wb");
    if (outFile == NULL)
    {
        fprintf(stderr,
                "Error opening output file '%s': %s.\n",
                outPath, strerror(errno));
        return 1;
    }

    for (x0 = 0; x0 <= x0Max; x0++)
    {
        /* Default properties */
        sprintf(props, "\"x0\": %u", x0);

        /* x0=0 edge properties */
        if (x0 == 0) strcat(props, ", \"x0minus_exists\": 0");

        /* x0=x0Max edge properties */
        if (x0 == x0Max) strcat(props, ", \"x0plus_exists\": 0");

        /* Initial conditions at x0. */
        if (x0 == 0) strcpy(state,
                            "\"m_x0\": 0.0, \"m_x1\": 1.0, \"m_x2\": 0.0");

        /* Write the entry. */
        fprintf(outFile, "<DevI id=\"%u\" type=\"fd_point\">", x0);
        if (strlen(props) > 0) fprintf(outFile, "<P>%s</P>", props);
        if (strlen(state) > 0) fprintf(outFile, "<S>%s</S>", state);
        fprintf(outFile, "</DevI>\n");

        /* Clear properties and state. */
        for (bufferIndex = 0; bufferIndex < JSON_BUFFER_SIZE; bufferIndex++)
        {
            props[bufferIndex] = 0;
            state[bufferIndex] = 0;
        }
    }

    return fclose(outFile);
}

int main(int argc, char** argv)
{
    unsigned x0Max = 99;
    char outputPath[OUTPATH_BUFFER_SIZE];

    sprintf(outputPath, "micromagnetics_1d_%u.xml", x0Max);
    if (write_device_instances(x0Max, DEVICE_INSTANCE_FRAGMENT)) return 1;
    /*if (write_edge_instances(x0Max, EDGE_INSTANCE_FRAGMENT)) return 1;*/
    return template_files(MICROMAGNETICS_TEMPLATE, outputPath);
}
