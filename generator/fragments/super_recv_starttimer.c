/* Only start the timer once. Note that message->payload is always zero (see
 * populate_timer_message.c) */
if (sEdgeState->hasTimerStarted == message->payload)
{
    FILE* timeFile;
    timeFile = fopen("micromagnetics_timing.txt", "a");
    time_t timeNtv;  /* "Native" */
    time(&timeNtv);
    char timeBuf[sizeof "YYYY-MM-DDTHH:MM:SS"];
    strftime(timeBuf, sizeof timeBuf, "%FT%T", localtime(&timeNtv));
    fprintf(timeFile, "startTime: %s\n", timeBuf);
    sEdgeState->hasTimerStarted = 1;
    fclose(timeFile);
}
