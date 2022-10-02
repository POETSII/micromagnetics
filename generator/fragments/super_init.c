SUPSTATE(outDir) = Super::get_output_directory();
SUPSTATE(dataStream).open(
    (SUPSTATE(outDir) + "/micromagnetics_data.csv").c_str());