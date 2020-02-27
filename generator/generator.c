/* Generates micromagnetic XML. */

#include "initial_conditions.h"
#include "template.h"

#define DEVICE_INSTANCE_FRAGMENT "fragments/instances/device_instances.xml"
#define EDGE_INSTANCE_FRAGMENT "fragments/instances/edge_instances.xml"
#define GRAPH_INSTANCE_FRAGMENT "fragments/instances/graph_instance.xml"
#define MICROMAGNETICS_TEMPLATE_1D "fragments/micromagnetics_template_1d.xml"
#define MICROMAGNETICS_TEMPLATE_2D "fragments/micromagnetics_template_2d.xml"
#define DEVICE_COUNT_PARAMETER_FRAGMENT "fragments/parameters/node_count.txt"
#define FINAL_ITERATION_PARAMETER_FRAGMENT "fragments/parameters/final_iteration.txt"
#define JSON_BUFFER_SIZE 1000
#define OUTPATH_BUFFER_SIZE 1000
#define FINAL_ITERATION 10000  /* Given dt=1e-13, this represents t=1e-9 */

/* Writes a graph instance to file. */
int write_graph_instance(char* graphInstanceName,
                         char* graphInstancePath,
                         char* graphType)
{
    /* Open the fragment to write to, clobbering any existing content. */
    FILE* graphInstanceFile;
    graphInstanceFile = fopen(graphInstancePath, "wb");
    if (graphInstanceFile == NULL)
    {
        fprintf(stderr,
                "Error opening output file '%s': %s.\n",
                graphInstancePath, strerror(errno));
        return 1;
    }

    /* Do it */
    fprintf(graphInstanceFile,
"  <GraphInstance id=\"%s\"\n"
"                 graphTypeId=\"%s\">\n"
"    <DeviceInstances>\n"
"{{device_instances.xml}}"
"    </DeviceInstances>\n"
"    <EdgeInstances>\n"
"{{edge_instances.xml}}"
"    </EdgeInstances>\n"
            "  </GraphInstance>", graphInstanceName, graphType);

    fclose(graphInstanceFile);
    return 0;
}

/* Writes device and edge instances to a file. */
int write_instances_1d(unsigned x0Max, char* deviceInstancePath,
                       char* edgeInstancePath)
{
    /* Create a properties staging area and a state staging area, used for each
     * iteration. */
    char props[JSON_BUFFER_SIZE];
    char state[JSON_BUFFER_SIZE];
    unsigned bufferIndex;

    /* Co-ordinates. */
    unsigned x0;

    /* Open the fragments to write to, clobbering any existing content. */
    FILE* deviceInstanceFile;
    FILE* edgeInstanceFile;
    deviceInstanceFile = fopen(deviceInstancePath, "wb");
    if (deviceInstanceFile == NULL)
    {
        fprintf(stderr,
                "Error opening output file '%s': %s.\n",
                deviceInstancePath, strerror(errno));
        return 1;
    }

    edgeInstanceFile = fopen(edgeInstancePath, "wb");
    if (edgeInstanceFile == NULL)
    {
        fprintf(stderr,
                "Error opening output file '%s': %s.\n",
                edgeInstancePath, strerror(errno));
        fclose(deviceInstanceFile);
        return 1;
    }

    /* Iterate over each device. */
    for (x0 = 0; x0 <= x0Max; x0++)
    {
        /* Default properties */
        sprintf(props, "\"x0\": %u", x0);

        /* Initial conditions at x0. */
        if (x0 == 0) strcpy(state,
                            "\"m_x0\": 0.0, \"m_x1\": 1.0, \"m_x2\": 0.0");

        /* Write the device entry. */
        fprintf(deviceInstanceFile, "<DevI id=\"%u\" type=\"fd_point\">", x0);
        if (strlen(props) > 0) fprintf(deviceInstanceFile, "<P>%s</P>", props);
        if (strlen(state) > 0) fprintf(deviceInstanceFile, "<S>%s</S>", state);
        fprintf(deviceInstanceFile, "</DevI>\n");

        /* Clear properties and state for device entries. */
        for (bufferIndex = 0; bufferIndex < JSON_BUFFER_SIZE; bufferIndex++)
        {
            props[bufferIndex] = 0;
            state[bufferIndex] = 0;
        }

        /* Edges connecting this device with the device x0-ahead (if there is
         * one) */
        if (x0 < x0Max)
        {
            fprintf(edgeInstanceFile,
                    "<EdgeI path=\"%u:state_recv_x0minus-%u:state_push\"/>\n",
                    x0 + 1, x0);
            fprintf(edgeInstanceFile,
                    "<EdgeI path=\"%u:state_recv_x0plus-%u:state_push\"/>\n",
                    x0, x0 + 1);
        }
        else  /* Periodic! */
        {
            fprintf(edgeInstanceFile,
                    "<EdgeI path=\"%u:state_recv_x0minus-%u:state_push\"/>\n",
                    0, x0);
            fprintf(edgeInstanceFile,
                    "<EdgeI path=\"%u:state_recv_x0plus-%u:state_push\"/>\n",
                    x0, 0);
        }

        /* Supervisor edge. */
        fprintf(edgeInstanceFile,
                "<EdgeI path=\":exfiltrate-%u:exfiltrate\"/>\n", x0);
    }

    /* Special case - device zero starts the timer. */
    fprintf(edgeInstanceFile, "<EdgeI path=\":starttimer-0:starttimer\"/>\n");

    fclose(deviceInstanceFile);
    fclose(edgeInstanceFile);
    return 0;
}

/* Writes device and edge instances to a file. */
int write_instances_2d(unsigned x0Max, unsigned x1Max,
                       char* deviceInstancePath, char* edgeInstancePath)
{
    /* Create a properties staging area and a state staging area, used for each
     * iteration. */
    char props[JSON_BUFFER_SIZE];
    char state[JSON_BUFFER_SIZE];
    unsigned bufferIndex;

    /* Co-ordinates. */
    unsigned x0;
    unsigned x1;

    /* Initial conditions (per co-ordinate). */
    float mx0;
    float mx1;
    float mx2;

    /* Open the fragments to write to, clobbering any existing content. */
    FILE* deviceInstanceFile;
    FILE* edgeInstanceFile;
    deviceInstanceFile = fopen(deviceInstancePath, "wb");
    if (deviceInstanceFile == NULL)
    {
        fprintf(stderr,
                "Error opening output file '%s': %s.\n",
                deviceInstancePath, strerror(errno));
        return 1;
    }

    edgeInstanceFile = fopen(edgeInstancePath, "wb");
    if (edgeInstanceFile == NULL)
    {
        fprintf(stderr,
                "Error opening output file '%s': %s.\n",
                edgeInstancePath, strerror(errno));
        fclose(deviceInstanceFile);
        return 1;
    }

    /* Clear properties and state for device entries. */
    for (bufferIndex = 0; bufferIndex < JSON_BUFFER_SIZE; bufferIndex++)
    {
        props[bufferIndex] = 0;
        state[bufferIndex] = 0;
    }

    /* Iterate over each device. */
    for (x0 = 0; x0 <= x0Max; x0++) for (x1 = 0; x1 <= x1Max; x1++)
    {
        /* Default properties */
        sprintf(props, "\"x0\": %u, \"x1\": %u", x0, x1);

        /* Plop a skyrmion in there, normalising the co-ordinates first. */
        skyrmion_profile(&mx0, &mx1, &mx2,
                         x0 - x0Max / 2, x1 - x1Max / 2, 0,
                         x0Max / 3);
        sprintf(state, "\"m_x0\": %f, \"m_x1\": %f, \"m_x2\": %f",
                mx0, mx1, mx2);

        /* Write the device entry. */
        fprintf(deviceInstanceFile,
                "<DevI id=\"%u_%u\" type=\"fd_point\">", x0, x1);
        if (strlen(props) > 0) fprintf(deviceInstanceFile, "<P>%s</P>", props);
        if (strlen(state) > 0) fprintf(deviceInstanceFile, "<S>%s</S>", state);
        fprintf(deviceInstanceFile, "</DevI>\n");

        /* Clear properties and state for device entries. */
        for (bufferIndex = 0; bufferIndex < JSON_BUFFER_SIZE; bufferIndex++)
        {
            props[bufferIndex] = 0;
            state[bufferIndex] = 0;
        }

        /* Edges connecting this device with the device x0-ahead (if there is
         * one) */
        if (x0 < x0Max)
        {
            fprintf(edgeInstanceFile,
                    "<EdgeI path=\"%u_%u:state_recv_x0minus-%u_%u:state_push\"/>\n",
                    x0 + 1, x1, x0, x1);
            fprintf(edgeInstanceFile,
                    "<EdgeI path=\"%u_%u:state_recv_x0plus-%u_%u:state_push\"/>\n",
                    x0, x1, x0 + 1, x1);
        }
        else  /* Periodic! */
        {
            fprintf(edgeInstanceFile,
                    "<EdgeI path=\"%u_%u:state_recv_x0minus-%u_%u:state_push\"/>\n",
                    0, x1, x0, x1);
            fprintf(edgeInstanceFile,
                    "<EdgeI path=\"%u_%u:state_recv_x0plus-%u_%u:state_push\"/>\n",
                    x0, x1, 0, x1);
        }

        /* Edges connecting this device with the device x1-ahead (if there is
         * one) */
        if (x1 < x1Max)
        {
            fprintf(edgeInstanceFile,
                    "<EdgeI path=\"%u_%u:state_recv_x1minus-%u_%u:state_push\"/>\n",
                    x0, x1 + 1, x0, x1);
            fprintf(edgeInstanceFile,
                    "<EdgeI path=\"%u_%u:state_recv_x1plus-%u_%u:state_push\"/>\n",
                    x0, x1, x0, x1 + 1);
        }
        else  /* Periodic! */
        {
            fprintf(edgeInstanceFile,
                    "<EdgeI path=\"%u_%u:state_recv_x1minus-%u_%u:state_push\"/>\n",
                    x0, 0, x0, x1);
            fprintf(edgeInstanceFile,
                    "<EdgeI path=\"%u_%u:state_recv_x1plus-%u_%u:state_push\"/>\n",
                    x0, x1, x0, 0);
        }

        /* Supervisor edge. */
        fprintf(edgeInstanceFile,
                "<EdgeI path=\":exfiltrate-%u_%u:exfiltrate\"/>\n", x0, x1);
    }

    /* Special case - device zero starts the timer. */
    fprintf(edgeInstanceFile,
            "<EdgeI path=\":starttimer-0_0:starttimer\"/>\n");

    fclose(deviceInstanceFile);
    fclose(edgeInstanceFile);
    return 0;
}

/* Defines parameters for the templating engine. */
int write_parameters(unsigned numberOfDevices, unsigned stoppingIteration)
{
    /* Open the fragments to write to, clobbering any existing content. */
    FILE* deviceCountFile;
    FILE* finalIterationFile;
    deviceCountFile = fopen(DEVICE_COUNT_PARAMETER_FRAGMENT, "wb");
    if (deviceCountFile == NULL)
    {
        fprintf(stderr,
                "Error opening output file '%s': %s.\n",
                DEVICE_COUNT_PARAMETER_FRAGMENT, strerror(errno));
        return 1;
    }

    finalIterationFile = fopen(FINAL_ITERATION_PARAMETER_FRAGMENT, "wb");
    if (finalIterationFile == NULL)
    {
        fprintf(stderr,
                "Error opening output file '%s': %s.\n",
                FINAL_ITERATION_PARAMETER_FRAGMENT, strerror(errno));
        fclose(deviceCountFile);
        return 1;
    }

    /* Do it. */
    fprintf(deviceCountFile, "%u", numberOfDevices);
    fprintf(finalIterationFile, "%u", stoppingIteration);

    /* Good bye! */
    fclose(deviceCountFile);
    fclose(finalIterationFile);
    return 0;
}

/* Populates x0Max, x1Max, and outputPath based of argc and argv.  */
int parse_args(int argc, char** argv,
                unsigned* x0Max, unsigned* x1Max, char* outputPath)
{
    char* unused;
    const char* help = \
"There are four valid forms for specifying input arguments:\n\n"
"  1d, default output:\n"
"      - unsigned (>0): x0Max\n"
"      - string (optional): outputPath (a default is used if not defined).\n\n"
"      (sets x1Max to zero)\n\n"
"  2d, default output:\n"
"      - unsigned (>0): x0Max\n"
"      - unsigned (>0): x1Max\n"
"      - string (optional): outputPath (a default is used if not defined).\n";

    /* Sanity checking on argument count. */
    if (argc < 2 || argc > 4)
    {
        fprintf(stderr, "%s", help);
        return 1;
    }

    /* Attempt to grab x0Max. */
    unused = 0;
    *x0Max = strtoul(argv[1], &unused, 10);
    if (*x0Max == 0)
    {
        fprintf(stderr, "%s", help);
        return 1;
    }

    /* Set defaults for one (real) input. */
    if (argc == 2)
    {
        *x1Max = 0;
        sprintf(outputPath, "micromagnetics_1d_%u.xml", *x0Max);
        return 1;
    }

    /* Attempt to grab x1Max. */
    *x1Max = strtoul(argv[2], &unused, 10);

    /* If it was invalid, assume it was an output path for the 1D case. */
    if (*x1Max == 0)
    {
        strcpy(outputPath, argv[2]);
        return 0;
    }

    /* Now we're in the realm of 2D: check for an outputPath. */
    if (argc == 4)
    {
        strcpy(outputPath, argv[3]);
        return 0;
    }
    else
    {
        sprintf(outputPath, "micromagnetics_2d_%u_%u.xml", *x0Max, *x1Max);
        return 1;
    }
}

/* The meat and potatoes - made externally available in shared-object land. */
int do_it(unsigned x0Max, unsigned x1Max, char* outputPath)
{
    /* Make stuff. */
    write_parameters((x0Max + 1) * (x1Max + 1), FINAL_ITERATION);
    if (x1Max == 0)  /* 1D */
    {
        if (write_instances_1d(x0Max, DEVICE_INSTANCE_FRAGMENT,
                               EDGE_INSTANCE_FRAGMENT)) return 1;
        if (write_graph_instance("micromagnetics",
                                 GRAPH_INSTANCE_FRAGMENT,
                                 "micromagnetic_simulation_1d")) return 1;
        return template_files(MICROMAGNETICS_TEMPLATE_1D, outputPath);
    }

    else  /* 2D */
    {
        if (write_instances_2d(x0Max, x1Max, DEVICE_INSTANCE_FRAGMENT,
                               EDGE_INSTANCE_FRAGMENT)) return 1;
        if (write_graph_instance("micromagnetics",
                                 GRAPH_INSTANCE_FRAGMENT,
                                 "micromagnetic_simulation_2d")) return 1;
        return template_files(MICROMAGNETICS_TEMPLATE_2D, outputPath);
    }
}

int main(int argc, char** argv)
{
    unsigned x0Max;
    unsigned x1Max;
    char outputPath[OUTPATH_BUFFER_SIZE];
    if(parse_args(argc, argv, &x0Max, &x1Max, outputPath)) return 1;
    return do_it(x0Max, x1Max, outputPath);
}
