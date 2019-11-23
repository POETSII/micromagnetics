/* Generates micromagnetic XML. */

#include "template.h"

#define DEVICE_INSTANCE_FRAGMENT "fragments/instances/device_instances.xml"
#define EDGE_INSTANCE_FRAGMENT "fragments/instances/edge_instances.xml"
#define GRAPH_INSTANCE_FRAGMENT "fragments/instances/graph_instance.xml"
#define MICROMAGNETICS_TEMPLATE "fragments/micromagnetics_template.xml"
#define JSON_BUFFER_SIZE 1000
#define OUTPATH_BUFFER_SIZE 1000

/* Writes a graph instance to file. */
int write_graph_instance(char* graphInstanceName,
                         char* graphInstancePath)
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
"                 graphTypeId=\"micromagnetic_simulation_1d\">\n"
"    <DeviceInstances>\n"
"{{device_instances.xml}}"
"    </DeviceInstances>\n"
"    <EdgeInstances>\n"
"{{edge_instances.xml}}"
"    </EdgeInstances>\n"
"  </GraphInstance>", graphInstanceName);

    fclose(graphInstanceFile);
    return 0;
}

/* Writes device and edge instances to a file. */
int write_instances(unsigned x0Max, char* deviceInstancePath,
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

        /* x0=0 device properties */
        if (x0 == 0) strcat(props, ", \"x0minus_exists\": 0");

        /* x0=x0Max device properties */
        if (x0 == x0Max) strcat(props, ", \"x0plus_exists\": 0");

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

        /* Supervisor edge. */
        fprintf(edgeInstanceFile,
                "<EdgeI path=\":exfiltrate-%u:exfiltrate\"/>\n", x0);
    }

    fclose(deviceInstanceFile);
    fclose(edgeInstanceFile);
    return 0;
}

int main(int argc, char** argv)
{
    char* unused;
    char outputPath[OUTPATH_BUFFER_SIZE];
    unsigned x0Max = strtoul(argv[1], &unused, 10);

    /* Expected arguments:
     *  - x0Max
     *  - optional: path to put the XML into */

    /* Sanity. */
    if (argc < 2)
    {
        fprintf(stderr, "One argument is expected - the number of nodes. "
                "Another argument is also accepted - the name of the file "
                "(and the graph instance) to produce.\n");
        return 1;
    }

    /* Come up with a file/instance name. */
    if (argc < 3)
    {
        sprintf(outputPath, "micromagnetics_1d_%u.xml", x0Max);
        return template_files(MICROMAGNETICS_TEMPLATE, outputPath);
    }
    else
    {
        strcpy(outputPath, argv[2]);
    }

    /* Make stuff. */
    if (write_instances(x0Max, DEVICE_INSTANCE_FRAGMENT,
                        EDGE_INSTANCE_FRAGMENT)) return 1;
    if (write_graph_instance(outputPath, GRAPH_INSTANCE_FRAGMENT)) return 1;
    return template_files(MICROMAGNETICS_TEMPLATE, outputPath);
}
