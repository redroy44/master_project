#include <iostream>
#include <stdlib.h>
#include <armadillo>
#include <boost/program_options.hpp>

#include "WaveProcessor.h"
#include "NoiseEstimator.h"

using namespace std;
using namespace arma;
namespace po = boost::program_options;

po::variables_map parseArgs(int argc, char *argv[])
{
	po::options_description desc("Options");
	desc.add_options()
		("help", "Print help messages")
		("inputfile", po::value< string >(), "Input file path")
		("outputfile", po::value< string >(), "Output file path");
	
	po::variables_map vm;
	try {
		po::store(po::parse_command_line(argc, argv, desc),
			vm);

		if (vm.count("help"))
		{
			std::cout << "Basic Command Line Parameter App" << std::endl
				<< desc << std::endl;
			exit(EXIT_SUCCESS);
		}

		po::notify(vm);
	}
	catch (po::error& e) {
		std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
		std::cerr << desc << std::endl;
		exit(EXIT_FAILURE);
	}
	return vm;
}


int main(int argc, char *argv[])
{
	po::variables_map vm = parseArgs(argc, argv);

	cout << "Hello Speech Enhancement" << endl;

	WaveProcessor waveProcessor(vm["inputfile"].as<string>(), vm["outputfile"].as<string>());

	cout << "reading wave...\n";
	vec wave = waveProcessor.readWave();

	waveProcessor.runAnalysis(wave);

	NoiseEstimator noiseEstimator;

	mat spectrum = waveProcessor.getSpectrum();
	// main-loop
	for (unsigned int i = 0; i < waveProcessor.getSpectrum().n_cols; i++) {
		vec powerSpec = square(spectrum.col(i));
        if (i == 0) {
            noiseEstimator.init(powerSpec);
//            parameters_lsa = initialize_lsa(powerSpec);
        }
        else {
        	noiseEstimator.estimateNoise(powerSpec);
        }
//        lsaEstimator.estimateSpec(powerSpec, noiseEstimator.getNoiseSpectrum());
	}



	// do the processing
	// waveProcessor.setSpectrum(spectrum);

	vec out = waveProcessor.runSynthesis();

	cout << "writing wave...\n";
	waveProcessor.writeWave(out);
	cout << "done.\n";



	return 0;
}
