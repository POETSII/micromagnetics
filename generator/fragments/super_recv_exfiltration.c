/* Start timer on first receipt */
if (!SUPSTATE(started)) SUPSTATE(timeStart) = std::chrono::steady_clock::now();
SUPSTATE(started) = true;

/* Write exfiltrated data to file. */
for (int dim = 0; dim < {{v:dim}};
     SUPSTATE(dataStream) << MSG(x)[dim++] << ",");
SUPSTATE(dataStream) <<
    MSG(iteration) << "," <<
    MSG(m_x)[0] << "," <<
    MSG(m_x)[1] << "," <<
    MSG(m_x)[2] << "\n";

/* Has this device just crossed the finish line? If so, count it. */
if (MSG(iteration) >= GRAPHPROPERTIES(finishLine))
{
    SUPSTATE(nodesFinished)++;

    /* Write out time taken when we're done, and post and close. */
    if (SUPSTATE(nodesFinished) == GRAPHPROPERTIES(nodeCount))
    {
        std::ofstream outStream;
        outStream.open(
            (SUPSTATE(outDir) + "/micromagnetics_timing.txt").c_str());
        outStream << std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - SUPSTATE(timeStart)).count();
        outStream << std::endl;
        outStream.close();
        Super::post("Micromagnetic simulation complete.");
        Super::stop_application();
    }
}