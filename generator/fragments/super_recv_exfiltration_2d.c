/* Write exfiltrated data to file. */
FILE* dataFile;
dataFile = fopen("micromagnetics.csv", "a");
fprintf(dataFile, "%u,%u,%u,%f,%f,%f\n",
        message->x0, message->x1, message->iteration,
        message->m_x0, message->m_x1, message->m_x2);
fclose(dataFile);

/* Has this device just crossed the finish line? If so, count it. */
if (message->iteration >= sEdgeProperties->finishLine)
{
    sEdgeState->nodesFinished++;

    /* Print when we're done. */
    if (sEdgeState->nodesFinished == sEdgeProperties->nodeCount)
    {
        printf("Micromagnetic simulation with %u devices complete.\n",
               sEdgeProperties->nodeCount);
    }
}

/* Stop timer, if this is the last device to cross the finish line. */
if (sEdgeState->nodesFinished >= sEdgeProperties->nodeCount)
{
    FILE* timeFile;
    timeFile = fopen("micromagnetics_timing.txt", "a");
    time_t timeNtv;  /* "Native" */
    time(&timeNtv);
    char timeBuf[sizeof "YYYY-MM-DDTHH:MM:SS"];
    strftime(timeBuf, sizeof timeBuf, "%FT%T", localtime(&timeNtv));
    fprintf(timeFile, "endTime: %s\n", timeBuf);
    fclose(timeFile);
}
