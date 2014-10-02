#include <iostream>
#include <stdlib.h>
#include <armadillo>
#include <boost/program_options.hpp>

using namespace std;
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
	return 0;
}
