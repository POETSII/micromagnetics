/* Write exfiltrated data to file. */
fprintf(SUPSTATE(dataFile), "%u,%u,%f,%f,%f\n",
        MSG(x0), MSG(iteration), MSG(m_x0), MSG(m_x1), MSG(m_x2));

/* Has this device just crossed the finish line? If so, count it. */
if (MSG(iteration) == SUPPROPERTIES(finishLine))
{
    SUPSTATE(nodesFinished)++;

    /* Post when we're done. */
    if (SUPSTATE(nodesFinished) == SUPPROPERTIES(nodeCount))
        Super::post("Micromagnetic simulation complete.");
}

/* Stop the timer, if this is the last device to cross the finish line.
if (SUPSTATE(nodesFinished) >= SUPPROPERTIES(nodeCount))
{
    FILE* timeFile;
    timeFile = fopen(
        (SUPSTATE(outDir) + "/micromagnetics_end_time.txt").c_str(), "w");
    time_t timeNtv;  /* "Native"
    time(&timeNtv);
    char timeBuf[sizeof "YYYY-MM-DDTHH:MM:SS"];
    strftime(timeBuf, sizeof timeBuf, "%FT%T", localtime(&timeNtv));
    fprintf(timeFile, "%s\n", timeBuf);
    fclose(timeFile);
} */