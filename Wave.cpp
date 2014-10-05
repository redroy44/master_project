/*
 * Wave.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: pbandurs
 */

#include "Wave.h"
#include <iostream>

// define static members
int Wave::samplerate;
int Wave::nfft;
int Wave::format;
int Wave::channels;
int Wave::framelen;
float Wave::winlen;
float Wave::overlap;

Wave::Wave() {
	samplerate = 16000;
	nfft = 512;
	winlen = 0.025f;
	framelen = (int)(samplerate * winlen);
	overlap = 0.5f;
	// TODO compute nfft

	std::cout << "framelen: " << framelen << " nfft: " << nfft << std::endl;
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

float Wave::getOverlap() {
	return overlap;
}

void Wave::setOverlap(float overlap) {
	this->overlap = overlap;
}

int Wave::getFramelen() {
	return framelen;
}

void Wave::setFramelen(int framelen) {
	this->framelen = framelen;
}

int Wave::getChannels() {
	return channels;
}

void Wave::setChannels(int channels) {
	this->channels = channels;
}

int Wave::getFormat() {
	return format;
}

void Wave::setFormat(int format) {
	this->format = format;
}
