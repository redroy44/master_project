/*
 * Wave.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: pbandurs
 */

#include "Wave.h"
#include "NoiseEstimator.h"
#include "LsaEstimator.h"
#include "SpecSubEstimator.h"
#include "sndfile.hh"
#include <armadillo>
#include <stdexcept>

using namespace arma;

// define static members
const int Wave::BUFFER_LEN = 1024;

Wave::Wave(const std::string &in, const std::string &out, const int &sr, const int &ch, const bool &ss) : waveProcessor() {
    inputfile = in;
    outputfile = out;
	samplerate = sr*1000; // the only change needed to 8/16kHz change
    channels = ch;
    spec_sub = ss;
    noiseLen = 300;
}

Wave::~Wave() {
	// TODO Auto-generated destructor stub
}

void Wave::read() {
    // process the wave up to FFT
    readWave();
    inSpectrum = waveProcessor.runAnalysis(inWave);
}

void Wave::process() {
	cout << "processing wave...\n";
    outSpectrum.copy_size(inSpectrum);

    if (!spec_sub) {
        NoiseEstimator noiseEstimator(waveProcessor.getNfft(), samplerate);
        LsaEstimator lsaEstimator(waveProcessor.getNfft());
        // TODO implement an col_iterator
        for (unsigned int i = 0; i < waveProcessor.getSpectrum().n_cols; i++) {
            vec powerSpec = square(waveProcessor.getSpectrum().col(i));
            if (i == 0) {
                noiseEstimator.init(powerSpec);
            }
            else {
                noiseEstimator.estimateNoise(powerSpec);
            }
            lsaEstimator.estimateSpec(powerSpec, noiseEstimator.getNoiseSpectrum());
            outSpectrum.col(i) = lsaEstimator.getCleanSpectrum();

        }
    } else {
        std::cout << "using julius spectral subtraction" << std::endl;
        SpecSubEstimator ssEstimator(waveProcessor.getNfft());
        // estimate Noise
        int num = (int)(samplerate*noiseLen/1000.0/waveProcessor.getFrameshift());

        ssEstimator.estimateNoise(waveProcessor.getSpectrum().cols(0, num));
        // main loop
        for (unsigned int i = 0; i < waveProcessor.getSpectrum().n_cols; i++) {
            ssEstimator.estimateSpec(waveProcessor.getSpectrum().col(i));
            outSpectrum.col(i) = ssEstimator.getCleanSpectrum();
        }
    }
}

void Wave::save() {
    std::cout << "saving wave" << std::endl;
    //outSpectrum = inSpectrum;
    waveProcessor.runSynthesis2(outSpectrum, outWave);
    writeWave();
}

void Wave::readWave() {
    SndfileHandle infile = SndfileHandle(inputfile);
    if (!infile || infile.frames() == 0) {
        throw std::invalid_argument("ERROR File doesn't exist or is empty");
    } else if (samplerate != infile.samplerate()) { // check samplerate
        throw std::invalid_argument("ERROR Samplerates don't match!");
    } else if (channels != infile.channels()) {
        throw std::invalid_argument("ERROR Number of channels don't match");
    }

    // TODO set to verbose output
    cout << "Input file name: " << inputfile << endl;
    cout << "Number of channels: " << infile.channels() << endl;
    cout << "Samplerate: " << infile.samplerate() << endl;
    cout << "Number of samples: " << infile.frames() << endl;
    cout << "Format: " << infile.format() << endl;

    float buffer[BUFFER_LEN];
    inWave = colvec((const arma::uword)infile.frames());

    for (int i = 0; i < infile.frames(); i++) {
        if ((i % BUFFER_LEN) == 0) {
            infile.read(buffer, BUFFER_LEN);
        }
        inWave(i) = buffer[i % BUFFER_LEN];
    }
    format = infile.format();
}

void Wave::writeWave() const {
    SndfileHandle outfile(outputfile, SFM_WRITE, format, channels, samplerate);
    if (!outfile) {
        throw std::invalid_argument("Cannot write to file!");
    }

    cout << "Output file name: " << outputfile << endl;
    cout << "Number of samples: " << outWave.n_elem << endl;

    outfile.write(outWave.colptr(0), outWave.n_elem);
}
