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
		("channels,c", po::value<int>()->default_value(1), "Number of channels")
		("spec_sub,j", po::value<bool>()->default_value(false), "Use julius-like speech denoising");

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
              vm["channels"].as<int>(),
              vm["spec_sub"].as<bool>());

    wave.read();
    wave.process();
    wave.save();
	cout << "done.\n";
	double n_secs = timer.toc();
	cout << "took " << n_secs << " seconds" << endl;
	return 0;
}
