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


WaveProcessor::WaveProcessor(){
    winlen = 0.02f;
    framelen = 256; //(int)(samplerate*winlen)
    nfft = framelen;
    overlap = 0.75f;
    
};

WaveProcessor::~WaveProcessor() {
	// TODO Auto-generated destructor stub
}

void WaveProcessor::runAnalysis(arma::vec wave) {
	//hamming
	getHamming();
	//segment
	mat segments = segmentWav(wave);
	cout << segments.n_rows << " " << segments.n_cols << endl;
	//filter
	winFilter(segments, window);
	//fft
	cx_mat cx_spectrum = fft(segments, nfft);
	//save angles
	savePhase(cx_spectrum); // atan2(x.imag(), x.real());
	//half_spec
	spectrum = abs(cx_spectrum.rows(0,floor((cx_spectrum.n_rows/2)-1))); // take first half of spectrum
	// normalize spectrum
    //	spec = normalise(spec);
}

arma::mat WaveProcessor::segmentWav(const arma::vec &wave) {
	// compute number of segments based on seg length and num of audio samples
	int seg_start = static_cast<int>( framelen * (1 - overlap)); // (1 - overlap) is the segment shift
	int num_segments = ((wave.n_elem - framelen) / seg_start) + 1;

	mat segments = zeros(framelen, num_segments);

	// begin segmentation
	for (int i = 0; i < framelen; i++) {
		for (int j = 0; j < num_segments;j++) {
			segments(i, j) = wave(j*seg_start + i);
		}
	}
	return segments;
}

void WaveProcessor::getHamming(void) {
    window = ones(framelen);

    for (unsigned int i = 0; i < window.n_rows; i++) {
        window(i) = 0.54 - 0.46 * cos((2*M_PI*i) / (framelen - 1));
    }
}

void WaveProcessor::getHanning(void) {
    window = ones(framelen);

    for (unsigned int i = 0; i < window.n_rows; i++) {
        window(i) = 0.5 * (1 - cos((2 * M_PI*i) / (framelen - 1)));
    }
}

void WaveProcessor::winFilter(arma::mat &segments, const arma::vec &window) {
    // TODO check segment - window lengths
    segments.each_col() %= window;
}

void WaveProcessor::savePhase(const arma::cx_mat &spc) {
    // TODO optimize this with iterator
    angles.copy_size(spc);

    for (unsigned int i = 0; i < spectrum.n_rows; i++) {
        for (unsigned int j = 0; j < spectrum.n_cols; j++) {
            angles(i, j) = atan2(spc(i, j).imag(), spc(i, j).real());
        }
    }
}

//arma::cx_mat WaveProcessor::getComplex(arma::mat magnitude) {
    //cx_mat spectrum;
    //spectrum.copy_size(magnitude);
    //mat angles = getAngles();

    //for (unsigned int i = 0; i < spectrum.n_rows; i++) {
        //for (unsigned int j = 0; j < spectrum.n_cols; j++) {
            //spectrum(i, j) = std::polar(magnitude(i,j), angles(i,j));
        //}
    //}

    //return spectrum;
//}

//arma::vec WaveProcessor::runSynthesis() {
	//mat magnitude = getSpectrum();
	//int length = getFramelen();
	//float overlap = getOverlap();

	//// if number of spectrum bins is odd
    //if (length % 2) {
        //magnitude = join_vert(magnitude, flipud(magnitude.rows(0, magnitude.n_rows - 2)));
    //}
    //else { // even
        //magnitude = join_vert(magnitude, flipud(magnitude.rows(0, magnitude.n_rows - 1)));
    //}
    //cx_mat spectrum = getComplex(magnitude); // retrieve complex spec from polar coordinates

    //// do the overlap-add reconstruction
    //int seg_shift = static_cast<int>(length * (1 - overlap)); // (1 - overlap) is the segment shift
    //int len2 = (length-floor(length*(1-overlap)));

    //vec synthesisWindow = getHamming();

    //vec xfinal = zeros((spectrum.n_cols)*seg_shift+len2);
    //vec synthesis = zeros((spectrum.n_cols)*seg_shift+len2);
    //vec x_old = zeros(len2);
    //vec syn_old = zeros(len2);
    //for (unsigned int i = 0; i < spectrum.n_cols; i++) {
        //int start = i*seg_shift;
        //cx_vec spec = spectrum.col(i);
        //vec xi = real(ifft(spec));

        //xfinal.rows(start, start+len2-1)= xi.rows(0, len2-1)+ x_old;
        //synthesis.rows(start, start+len2-1)= synthesisWindow.rows(0, len2-1) + syn_old;

        //x_old = xi.rows(seg_shift, length-1);
        //syn_old = synthesisWindow.rows(seg_shift, length-1);
    //}
    //return xfinal / synthesis;
//}
