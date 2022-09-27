SUPSTATE(outDir) = Super::get_output_directory();
SUPSTATE(dataFile) = fopen(
    (SUPSTATE(outDir) + "/micromagnetics_data.csv").c_str(), "w");