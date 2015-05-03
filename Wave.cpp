/*
 * Wave.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: pbandurs
 */

#include "Wave.h"
//#include <iostream>
#include "sndfile.hh"
#include <armadillo>
#include <stdexcept>

using namespace arma;

// define static members
const int Wave::BUFFER_LEN = 1024;

//int Wave::samplerate;
//int Wave::nfft;
//int Wave::format;
//int Wave::channels;
//int Wave::framelen;
//float Wave::winlen;
//float Wave::overlap;

Wave::Wave(const std::string &in, const std::string &out, const int &sr, const int &ch) {
    inputfile = in;
    outputfile = out;
	samplerate = sr*1000; // the only change needed to 8/16kHz change
    channels = ch;
	//winlen = 0.02f;
	//framelen = 256;//(int)(samplerate * winlen);
	//nfft = framelen;
	//overlap = 0.75f;
}

Wave::~Wave() {
	// TODO Auto-generated destructor stub
}

void Wave::read() {
    readWave();
}

void Wave::process() {
}

void Wave::save() {
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
}
//int Wave::getNfft() {
	//return nfft;
//}

//void Wave::setNfft(int nfft) {
	//this->nfft = nfft;
//}

//int Wave::getSamplerate() {
	//return samplerate;
//}

//void Wave::setSamplerate(int samplerate) {
	//this->samplerate = samplerate;
//}

//float Wave::getOverlap() {
	//return overlap;
//}

//void Wave::setOverlap(float overlap) {
	//this->overlap = overlap;
//}

//int Wave::getFramelen() {
	//return framelen;
//}

//void Wave::setFramelen(int framelen) {
	//this->framelen = framelen;
//}

//int Wave::getChannels() {
	//return channels;
//}

//void Wave::setChannels(int channels) {
	//this->channels = channels;
//}

//int Wave::getFormat() {
	//return format;
//}

//void Wave::setFormat(int format) {
	//this->format = format;
//}

//arma::vec Wave::mag2db(arma::vec vector) {
	//return 20*log10(vector); // matlab mag2db
//}

//arma::vec Wave::db2mag(arma::vec vector) {
	//return exp10(vector/20); // matlab db2mag
//}

//float Wave::mag2db(float freq) {
	//return 20*log10(freq); // matlab mag2db
//}

//float Wave::db2mag(float db) {
	//return exp10(db/20); // matlab db2mag
//}

//float Wave::pow2db(float freq) {
    //return 10*log10(freq); // matlab mag2db
//}

//float Wave::db2pow(float db) {
    //return exp10(db/10); // matlab db2mag
//}

//arma::vec Wave::medFilter(arma::vec frame, int order) {
    //int length = frame.n_elem;
    //int center = order/2;
    //vec medFiltered = frame;
    //vec window = zeros<vec>(order);

    //// expand frame
    //vec exFrame = join_cols(zeros<vec>(center), join_cols(frame, zeros<vec>(center)));

    //for (int i = center; i < length-center+1; i++) {
        //window.zeros();
        //for ( int j = -center; j < center+1; j++) {
            //window[center+j] = exFrame[i+j];
        //}
        //medFiltered[i-center] = median(window);
    //}

    //return medFiltered;
//}

//arma::vec Wave::meanFilter(arma::vec frame, int order) {
    //int length = frame.n_elem;
    //vec window = 1.0/order * ones<vec>(order);

    //vec filtered = conv(frame, window);

    //filtered = filtered.rows(0, length-1);

    //return filtered;
//}
