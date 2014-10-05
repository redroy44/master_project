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

    // check samplerate
    if (samplerate != infile.samplerate()) {
    	cout << "samplerates don't match!!! Exiting."
    	exit(EXIT_FAILURE);
    }
    channels = infile.channels();
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
        cout << "Cannot write to file!" << endl;
        exit(EXIT_FAILURE);
    }
    outfile.write(outwave.colptr(0), outwave.n_elem);
}

vec WaveProcessor::getHamming(void) {

    vec window = ones(framelen);

    for (unsigned int i = 0; i < window.n_rows; i++) {
        window(i) = 0.54 - 0.46 * cos((2*M_PI*i) / (framelen - 1));
    }

    return window;
}

arma::vec WaveProcessor::getHanning(void) {
    vec window = ones(framelen);

    for (unsigned int i = 0; i < window.n_rows; i++) {
        window(i) = 0.5 * (1 - cos((2 * M_PI*i) / (framelen - 1)));
    }

    return window;
}

arma::mat WaveProcessor::winFilter(arma::mat segments, arma::vec window) {
	// TODO check segment - window lengths
	for (unsigned int i = 0; i < segments.n_cols; i++) {
		segments.col(i) = segments.col(i) % window; // element-wise multiplication
	}
	return segments;
}

void WaveProcessor::runAnalysis(arma::vec wave) {
	//hamming
	//segment
	//filter
	//fft
	//half_spec
	//save angles
	//done
}

arma::mat WaveProcessor::getPhase(arma::cx_mat spectrum) {
    mat angle;
    angle.copy_size(spectrum);

    for (unsigned int i = 0; i < spectrum.n_rows; i++) {
        for (unsigned int j = 0; j < spectrum.n_cols; j++) {
            angle(i, j) = atan2(spectrum(i, j).imag(), spectrum(i, j).real());
        }
    }
    return angle;
}
