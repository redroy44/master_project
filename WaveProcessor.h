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

    void runAnalysis(arma::vec);
	//arma::vec runSynthesis(void);

private:
    float winlen;
    int framelen;
    int nfft;
    float overlap;

    arma::mat window;
    arma::mat spectrum;
    arma::mat angles;

	//// wave utilities
    void getHamming(void);
    void getHanning(void);

    arma::mat segmentWav(const arma::vec &);
	void winFilter(arma::mat &, const arma::vec &);

	void savePhase(const arma::cx_mat &);
	//arma::cx_mat getComplex(arma::mat);
};

#endif /* WAVEPROCESSOR_H_ */
