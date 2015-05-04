/*
 * WaveProcessor.h
 *
 *  Created on: 5 paź 2014
 *      Author: piotrek
 */

#ifndef WAVEPROCESSOR_H_
#define WAVEPROCESSOR_H_

#include "sndfile.hh"
#include <armadillo>

class WaveProcessor {
public:
	WaveProcessor();
	virtual ~WaveProcessor();

    void runAnalysis(const arma::vec &);
	void runSynthesis(arma::vec &);
    const arma::mat & getSpectrum(void) const;
    const unsigned int & getNfft(void) const;

private:
    float winlen;
    unsigned int framelen;
    unsigned int nfft;
    float overlap;

    arma::vec window;
    arma::mat spectrum;
    arma::mat angles;

	//// wave utilities
    void getHamming(void);
    void getHanning(void);

    arma::mat segmentWav(const arma::vec &);
	void winFilter(arma::mat &, const arma::vec &);

	void savePhase(const arma::cx_mat &);
    void getComplex(arma::cx_mat &);
};

#endif /* WAVEPROCESSOR_H_ */
