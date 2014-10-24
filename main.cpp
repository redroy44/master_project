#include <iostream>
#include <stdlib.h>
#include <armadillo>
#include <boost/program_options.hpp>

#include "WaveProcessor.h"
#include "NoiseEstimator.h"
#include "LsaEstimator.h"

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
	cout << "Hello Speech Enhancement" << endl;
	po::variables_map vm = parseArgs(argc, argv);
	WaveProcessor waveProcessor(vm["inputfile"].as<string>(), vm["outputfile"].as<string>());

	cout << "reading wave...\n";
	vec wave = waveProcessor.readWave();

	waveProcessor.runAnalysis(wave);

	NoiseEstimator noiseEstimator;
	LsaEstimator lsaEstimator;

	mat spectrum = waveProcessor.getSpectrum();
	mat clean;
	clean.copy_size(spectrum);

	// main-loop
	cout << "processing wave...\n";

	for (unsigned int i = 0; i < waveProcessor.getSpectrum().n_cols; i++) {
		vec powerSpec = square(spectrum.col(i));
        if (i == 0) {
            noiseEstimator.init(powerSpec);
        }
        else {
        	noiseEstimator.estimateNoise(powerSpec);
        }
        lsaEstimator.estimateSpec(powerSpec, noiseEstimator.getNoiseSpectrum());
        clean.col(i) = lsaEstimator.getCleanSpectrum();

#ifdef DEBUG
        noiseSpectrum = join_horiz(noiseSpectrum, sqrt(noiseEstimator.getNoiseSpectrum()));
#endif
	}
#ifdef DEBUG
	clean.save("enhancedSpectrum.dat", raw_ascii);
	noiseSpectrum.save("noiseEstSpectrum.dat", raw_ascii);
	lsaEstimator.matGain.save("matGain.dat", raw_ascii);
	lsaEstimator.matSNRpriori.save("matSNRpriori.dat", raw_ascii);
	lsaEstimator.matSNRposteriori.save("matSNRposteriori.dat", raw_ascii);
#endif

	waveProcessor.setSpectrum(clean);

	vec out = waveProcessor.runSynthesis();
	cout << "writing wave...\n";
	waveProcessor.writeWave(out);
	cout << "done.\n";

	return 0;
}
