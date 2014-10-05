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
    if (getSamplerate() != infile.samplerate()) {
    	cout << "samplerates don't match!!! Exiting.";
    	exit(EXIT_FAILURE);
    }
    setChannels(infile.channels());
    setFormat(infile.format());

    return wave;
}

void WaveProcessor::writeWave(arma::vec outwave) {
    SndfileHandle outfile(outWaveFile, SFM_WRITE, getFormat(), getChannels(), getSamplerate());

    cout << outWaveFile << endl;
    cout << getChannels() << endl;
    cout << getSamplerate() << endl;
    cout << outwave.n_elem << endl;
    cout << getFormat() << endl;

    if (!outfile) {
        cout << "Cannot write to file!" << endl;
        exit(EXIT_FAILURE);
    }
    outfile.write(outwave.colptr(0), outwave.n_elem);
}

vec WaveProcessor::getHamming(void) {

    vec window = ones(getFramelen());

    for (unsigned int i = 0; i < window.n_rows; i++) {
        window(i) = 0.54 - 0.46 * cos((2*M_PI*i) / (getFramelen() - 1));
    }

    return window;
}

arma::vec WaveProcessor::getHanning(void) {
    vec window = ones(getFramelen());

    for (unsigned int i = 0; i < window.n_rows; i++) {
        window(i) = 0.5 * (1 - cos((2 * M_PI*i) / (getFramelen() - 1)));
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
	vec window = getHamming();
	//segment
	mat segments = segmentWav(wave);
	//filter
	segments = winFilter(segments, window);
	//fft
	cx_mat spectrum = fft(segments);
	cout << "full_spec_length: " << spectrum.n_rows << " "<< spectrum.n_cols << endl;
	//half_spec
	mat spec = abs(spectrum.rows(0,(int)(spectrum.n_rows/2))); // take first half of spectrum
	cout << "half_spec_length: " << spec.n_rows  << " "<< spec.n_cols << endl;
	//save angles
	mat phase = getPhase(spectrum); // atan2(x.imag(), x.real());

	setSpectrum(spec);
	setAngles(phase);
	setNfft(spec.n_rows);
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

arma::mat WaveProcessor::getAngles() {
	return angles;
}

void WaveProcessor::setAngles(arma::mat angles) {
	this->angles = angles;
}

arma::mat WaveProcessor::getSpectrum() {
	return spectrum;
}

void WaveProcessor::setSpectrum(arma::mat spectrum) {
	this->spectrum = spectrum;
}

arma::mat WaveProcessor::segmentWav(arma::vec wave) {
	// compute number of segments based on seg length and num of audio samples
	int length = getFramelen();
	int seg_start = static_cast<int>( length * (1 - getOverlap())); // (1 - overlap) is the segment shift
	int num_segments = ((wave.n_elem - length) / seg_start) + 1;

	mat segments = zeros(length, num_segments);

	// begin segmentation
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < num_segments;j++) {
			segments(i, j) = wave(j*seg_start + i);
		}
	}

	return segments;
}

arma::cx_mat WaveProcessor::getComplex(arma::mat magnitude) {
    cx_mat spectrum;
    spectrum.copy_size(magnitude);
    mat angles = getAngles();

    cout << "rows: " << spectrum.n_rows << " cols: " << spectrum.n_cols << endl;

    for (unsigned int i = 0; i < spectrum.n_rows; i++) {
        for (unsigned int j = 0; j < spectrum.n_cols; j++) {
            spectrum(i, j) = std::polar(magnitude(i,j), angles(i,j));
        }
    }

    return spectrum;
}

arma::vec WaveProcessor::runSynthesis() {
	mat magnitude = getSpectrum();
	int length = getFramelen();
	float overlap = getOverlap();

	// if number of spectrum bins is odd
    if (length % 2) {
        magnitude = join_vert(magnitude, flipud(magnitude.rows(1, magnitude.n_rows - 1 )));
    }
    else {
        magnitude = join_vert(magnitude, flipud(magnitude.rows(1, magnitude.n_rows - 2)));
    }

    std::cout << "magnitude: "<< magnitude.n_rows << " " << magnitude.n_cols << std::endl;
    std::cout << "phase : " << getAngles().n_rows << " " << getAngles().n_cols << std::endl;

    cx_mat spectrum = getComplex(magnitude); // retrieve complex spec from polar coordinates

    std::cout << "complex spectrum : " << spectrum.n_rows << " " << spectrum.n_cols << std::endl;

    // do the overlap-add reconstruction

    int seg_shift = static_cast<int>(length * (1 - overlap)); // (1 - overlap) is the segment shift
    vec output = zeros((spectrum.n_cols - 1)*seg_shift + length);

    std::cout << "overlap-add: " << output.n_elem << std::endl;

    for (unsigned int i = 0; i < spectrum.n_cols; i++) {
        int start = i*seg_shift;
        cx_vec spec = spectrum.col(i);
        output.rows(start, start + length - 1) = output.rows(start, start + length - 1) + real(ifft(spec, length));
    }

    return output;
}
