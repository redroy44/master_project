/*
 * Wave.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: pbandurs
 */

#include "Wave.h"
#include <iostream>
#include <armadillo>

// define static members
int Wave::samplerate;
int Wave::nfft;
int Wave::format;
int Wave::channels;
int Wave::framelen;
float Wave::winlen;
float Wave::overlap;

Wave::Wave() {
	samplerate = 16000; // the only change needed to 8/16kHz change
	winlen = 0.025f;
	framelen = (int)(samplerate * winlen);
	nfft = (framelen / 2) + 1;
	overlap = 0.5f;
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

arma::vec Wave::mag2db(arma::vec vector) {
	return 20*log10(vector); // matlab mag2db
}

arma::vec Wave::db2mag(arma::vec vector) {
	return exp10(vector/20); // matlab db2mag
}

float Wave::mag2db(float freq) {
	return 20*log10(freq); // matlab mag2db
}

float Wave::db2mag(float db) {
	return exp10(db/20); // matlab db2mag
}
