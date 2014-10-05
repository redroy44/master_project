/*
 * Wave.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: pbandurs
 */

#include "Wave.h"

// define static members
int Wave::samplerate;
int Wave::nfft;
int Wave::format;
int Wave::channels;

Wave::Wave() {
	// TODO Auto-generated constructor stub
	samplerate = 16000;
	nfft = 256;
}

Wave::~Wave() {
	// TODO Auto-generated destructor stub
}

int Wave::getNfft() {
	return nfft;
}

void Wave::setNfft(int nfft) {
	this->nfft = nfft;
}

int Wave::getSamplerate() {
	return samplerate;
}

void Wave::setSamplerate(int samplerate) {
	this->samplerate = samplerate;
}
