#include <iostream>
#include <stdlib.h>
#include <armadillo>
#include <boost/program_options.hpp>

#ifdef DEBUG
#include <boost/filesystem.hpp>
#endif

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
		("help,h", "Print help messages")
		("inputfile,i", po::value<string>()->required(), "Input file path")
		("outputfile,o", po::value<string>()->required(), "Output file path")
		("samplerate,s", po::value<int>()->default_value(16), "Inputfile samplerate in kHz")
		("channels,c", po::value<int>()->default_value(1), "Number of channels");

    po::variables_map vm;
	try {
		po::store(po::parse_command_line(argc, argv, desc), vm);

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
    wall_clock timer;
    timer.tic();
	cout << "Hello Speech Enhancement" << endl;
	po::variables_map vm = parseArgs(argc, argv);
	Wave wave(vm["inputfile"].as<string>(),
              vm["outputfile"].as<string>(),
              vm["samplerate"].as<int>(),
              vm["channels"].as<int>());

    wave.read();
    wave.process();
    wave.save();
	//cout << "reading wave...\n";
	//vec wave = waveProcessor.readWave();
	//wave.runAnalysis();
    //NoiseEstimator noiseEstimator;
	//LsaEstimator lsaEstimator;
	//mat spectrum = waveProcessor.getSpectrum();
	//mat clean;
	//clean.copy_size(spectrum);
	// main-loop
	//for (unsigned int i = 0; i < waveProcessor.getSpectrum().n_cols; i++) {
		//vec powerSpec = square(spectrum.col(i));
        //if (i == 0) {
            //noiseEstimator.init(powerSpec);
        //}
        //else {
            //noiseEstimator.estimateNoise(powerSpec);
        //}
        //lsaEstimator.estimateSpec(powerSpec, noiseEstimator.getNoiseSpectrum());
        //clean.col(i) = lsaEstimator.getCleanSpectrum();

	//}
	//waveProcessor.setSpectrum(clean);
	//cout << "Synthesizing wave...\n";
	//vec out = waveProcessor.runSynthesis();
	//cout << "writing wave...\n";
	//waveProcessor.writeWave(out);
	cout << "done.\n";
	double n_secs = timer.toc();
	cout << "took " << n_secs << " seconds" << endl;
	return 0;
}
