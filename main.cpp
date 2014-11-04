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
    wall_clock timer;
    timer.tic();
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

#ifdef DEBUG
	mat noiseSpectrum;
#endif

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
	boost::filesystem::path dir("dumps");
	boost::filesystem::create_directory(dir);
	clean.save("dumps/enhancedSpectrum.dat", raw_ascii);
	noiseSpectrum.save("dumps/noiseEstSpectrum.dat", raw_ascii);
	lsaEstimator.matGain.save("dumps/matGain.dat", raw_ascii);
	lsaEstimator.matSNRpriori.save("dumps/matSNRpriori.dat", raw_ascii);
	lsaEstimator.matSNRposteriori.save("dumps/matSNRposteriori.dat", raw_ascii);
	spectrum.save("dumps/noisySpectrum.dat", raw_ascii);
	noiseEstimator.matNoisyRatio.save("dumps/noisyRatio.dat", raw_ascii);
	noiseEstimator.matSpDecision.save("dumps/spDecision.dat", raw_ascii);
	noiseEstimator.matSpProbability.save("dumps/spProbability.dat", raw_ascii);
//	noiseEstimator.matAlpha.save("dumps/alpha_s.dat", raw_ascii);
	noiseEstimator.matSmPower.save("dumps/smPower.dat", raw_ascii);
	noiseEstimator.matMinPower.save("dumps/minPower.dat", raw_ascii);

	// reading clean speech
	WaveProcessor cleanProcessor("/home/piotrek/master_testing/adding_noise/spk2_2.wav", "true_noise.wav");
	vec cleanSpeech = cleanProcessor.readWave();
	cleanProcessor.runAnalysis(cleanSpeech);
	cleanProcessor.getSpectrum().save("dumps/cleanSpeech.dat", raw_ascii);

	//reading pure noise
	WaveProcessor noiseProcessor("/home/piotrek/master_testing/adding_noise/noise_segment.wav", "true_noise.wav");
	vec noise = noiseProcessor.readWave();
	noiseProcessor.runAnalysis(noise);
	noiseProcessor.getSpectrum().save("dumps/trueNoise.dat", raw_ascii);
#endif
	waveProcessor.setSpectrum(clean);
	cout << "Synthesizing wave...\n";
	vec out = waveProcessor.runSynthesis();
	cout << "writing wave...\n";
	waveProcessor.writeWave(out);
	cout << "done.\n";

	double n_secs = timer.toc();
	cout << "took " << n_secs << " seconds" << endl;

	return 0;
}
