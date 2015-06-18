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
    window = ones(framelen);
};

WaveProcessor::~WaveProcessor() {
	// TODO Auto-generated destructor stub
}

arma::mat WaveProcessor::runAnalysis(const arma::vec &wave) {
    original_len = wave.n_elem;
    //hamming
    getHamming();
    //segment
    mat segments = segmentWav2(wave);
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
    return spectrum;
}

arma::mat WaveProcessor::segmentWav(const arma::vec &wave) {
    // compute number of segments based on seg length and num of audio samples
    unsigned int seg_start = static_cast<int>( framelen * (1 - overlap)); // (1 - overlap) is the segment shift
    unsigned int num_segments = ((wave.n_elem - framelen) / seg_start) + 1;

    mat segments = zeros(framelen, num_segments);

    // begin segmentation
    for (unsigned int i = 0; i < framelen; i++) {
        for (unsigned int j = 0; j < num_segments;j++) {
            segments(i, j) = wave(j*seg_start + i);
        }
    }
    return segments;
}

arma::mat WaveProcessor::segmentWav2(const arma::vec &wave) {

    // unsigned int num_segments = ((wave.n_elem - framelen) / seg_start) + 1;
    //temporary WA
    frame_step = (framelen * (1 - overlap)); // (1 - overlap) is the segment shift
    //std::cout << frame_step << std::endl;
    num_segments = 2 + (wave.n_elem - framelen)/frame_step;
    padded_len = (num_segments-1)*frame_step + framelen;

    vec padded_wave = join_cols(wave, zeros<vec>(padded_len-wave.n_elem));
    mat indices = zeros(framelen, num_segments);

    std::cout << "num segments " << num_segments << std::endl;
    std::cout << "padded len " << padded_len << std::endl;

    indices = repmat(linspace<vec>(0, framelen - 1, framelen), 1, num_segments)
    + repmat(linspace<rowvec>(0, padded_len-framelen, num_segments), framelen, 1);

    std::cout << indices(0,0) << " " << indices(indices.n_rows-1, indices.n_cols-1) << std::endl;
    // TODO Use .transform() function
    for (mat::iterator i = indices.begin(); i!=indices.end(); ++i) {
        *i = padded_wave(*i);
    }
    return indices;
}

void WaveProcessor::getHamming(void) {
    for (unsigned int i = 0; i < window.n_rows; i++) {
        window(i) = 0.54 - 0.46 * cos((2*M_PI*i) / (framelen - 1));
    }
}

void WaveProcessor::getHanning(void) {
    for (unsigned int i = 0; i < window.n_rows; i++) {
        window(i) = 0.5 * (1 - cos((2 * M_PI*i) / (framelen - 1)));
    }
}

void WaveProcessor::getRectangular(void) {
    window = ones(window.n_rows);
}

void WaveProcessor::winFilter(arma::mat &segments, const arma::vec &window) {
    // TODO check segment - window lengths
    segments.each_col() %= window;
}

void WaveProcessor::savePhase(const arma::cx_mat &spc) {
    // TODO optimize this with iterator
    angles.copy_size(spc);

    for (unsigned int i = 0; i < spc.n_rows; i++) {
        for (unsigned int j = 0; j < spc.n_cols; j++) {
            angles(i, j) = std::arg(spc(i, j));
        }
    }
}

void WaveProcessor::getComplex(arma::cx_mat &magnitude) {
    for (unsigned int i = 0; i < spectrum.n_rows; i++) {
        for (unsigned int j = 0; j < spectrum.n_cols; j++) {
            magnitude(i, j) = std::polar(spectrum(i,j), angles(i,j));
        }
    }
}

void WaveProcessor::runSynthesis(const arma::mat &spc, arma::vec &outWave) {
     spectrum = spc;
    // if number of spectrum bins is odd
    if (framelen % 2) {
        spectrum = join_vert(spectrum, flipud(spectrum.rows(0, spectrum.n_rows - 2)));
    }
    else { // even
        spectrum = join_vert(spectrum, flipud(spectrum.rows(0, spectrum.n_rows - 1)));
    }
    cx_mat cx_spectrum;
    cx_spectrum.copy_size(spectrum);
    getComplex(cx_spectrum); // retrieve complex spec from polar coordinates



    // do the overlap-add reconstruction
    int seg_shift = static_cast<int>(framelen * (1 - overlap)); // (1 - overlap) is the segment shift
    int len2 = (framelen-floor(framelen*(1-overlap)));

    vec xfinal = zeros((cx_spectrum.n_cols)*seg_shift+len2);
    vec synthesis = zeros((cx_spectrum.n_cols)*seg_shift+len2);
    vec x_old = zeros(len2);
    vec syn_old = zeros(len2);
    for (unsigned int i = 0; i < cx_spectrum.n_cols; i++) {
        int start = i*seg_shift;
        vec xi = real(ifft(cx_spectrum.col(i)));

        xfinal.rows(start, start+len2-1)= xi.rows(0, len2-1)+ x_old;
        synthesis.rows(start, start+len2-1)= window.rows(0, len2-1) + syn_old;

        x_old = xi.rows(seg_shift, framelen-1);
        syn_old = window.rows(seg_shift, framelen-1);
    }
    outWave = xfinal / synthesis;
}

void WaveProcessor::runSynthesis2(const arma::mat &spc, arma::vec &outWave) {
     spectrum = spc;
    // if number of spectrum bins is odd
    if (framelen % 2) {
        spectrum = join_vert(spectrum, flipud(spectrum.rows(0, spectrum.n_rows - 2)));
    }
    else { // even
        spectrum = join_vert(spectrum, flipud(spectrum.rows(0, spectrum.n_rows - 1)));
    }
    cx_mat cx_spectrum;
    cx_spectrum.copy_size(spectrum);
    getComplex(cx_spectrum); // retrieve complex spec from polar coordinates

    umat indices = repmat(linspace<uvec>(0, framelen - 1, framelen), 1, num_segments)
    + repmat(linspace<urowvec>(0, padded_len-framelen, num_segments), framelen, 1);

    // do the overlap-add reconstruction
    vec signal = zeros<vec>(padded_len);
    vec correction = zeros<vec>(padded_len);

    for (unsigned int i = 0; i < cx_spectrum.n_cols; i++) {
        correction.rows(indices.col(i)) = correction.rows(indices.col(i)) + window;

        signal.rows(indices.col(i)) = signal.rows(indices.col(i)) + real(ifft(cx_spectrum.col(i)));

    }
    outWave = signal / correction;
    outWave = outWave.rows(0, original_len - 1);
}

const arma::mat & WaveProcessor::getSpectrum() const {
    return spectrum;
}

const unsigned int & WaveProcessor::getNfft() const {
    return nfft;
}

const unsigned int WaveProcessor::getFrameshift() const {
    return static_cast<unsigned int>(framelen * (1 - overlap));
}
