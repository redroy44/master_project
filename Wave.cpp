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
int Wave::framelen;
int Wave::winlen;
int Wave::overlap;

Wave::Wave() {
	// TODO Auto-generated constructor stub
	samplerate = 16000;
//	nfft = 256;
	winlen = 0.025f;
	framelen = (int)(samplerate * winlen);
	overlap = 0.5f;
}

Wave::~Wave() {
	// TODO Auto-generated destructor stub
}

//int Wave::getNfft() {
//	return nfft;
//}
//
//void Wave::setNfft(int nfft) {
//	this->nfft = nfft;
//}

int Wave::getSamplerate() {
	return samplerate;
}

void Wave::setSamplerate(int samplerate) {
	this->samplerate = samplerate;
}
