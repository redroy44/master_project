/*
 * WaveProcess.cpp
 *
 *  Created on: 5 paź 2014
 *      Author: piotrek
 */

#include "WaveProcessor.h"
#include <armadillo>
#include "sndfile.hh"

#define _USE_MATH_DEFINES


using namespace arma;
using namespace std;

const int WaveProcessor::BUFFER_LEN = 1024;

WaveProcessor::WaveProcessor(string inWaveFile, string outWaveFile) {
	this->inWaveFile = inWaveFile;
	this->outWaveFile = outWaveFile;
}

WaveProcessor::~WaveProcessor() {
	// TODO Auto-generated destructor stub
}

arma::vec WaveProcessor::readWave(void) {

    SndfileHandle infile = SndfileHandle(inWaveFile);
    if (!infile) {
        cout << "Cannot load the file!" << endl;
        exit(EXIT_FAILURE);
    }

    cout << inWaveFile << endl;
    cout << infile.channels() << endl;
    cout << infile.samplerate() << endl;
    cout << infile.frames() << endl;
    cout << infile.format() << endl;


    static float buffer[BUFFER_LEN];
    vec wave = colvec((const arma::uword)infile.frames());

    for (int i = 0; i < infile.frames(); i++) {
        if ((i % BUFFER_LEN) == 0) {
            infile.read(buffer, BUFFER_LEN);
        }
        wave(i) = buffer[i % BUFFER_LEN];
    }

    channels = infile.channels();
    samplerate = infile.samplerate();
    format = infile.format();

    return wave;
}

void WaveProcessor::writeWave(arma::vec outwave) {
    SndfileHandle outfile(outWaveFile, SFM_WRITE, format, channels, samplerate);

    cout << outWaveFile << endl;
    cout << channels << endl;
    cout << samplerate << endl;
    cout << outwave.n_elem << endl;
    cout << format << endl;

    if (!outfile) {
        std::cout << "Cannot write to file!" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    outfile.write(outwave.colptr(0), outwave.n_elem);
}

arma::vec WaveProcessor::getHamming(void) {
}
