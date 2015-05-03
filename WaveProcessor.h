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


	//void runAnalysis(arma::vec);
	//arma::vec runSynthesis(void);

	//arma::mat getAngles(void);
	//void setAngles(arma::mat);
	//arma::mat getSpectrum(void);
	//void setSpectrum(arma::mat);

private:
	//static const int BUFFER_LEN;
	//std::string inWaveFile;
	//std::string outWaveFile;

	//arma::mat spectrum;
	//arma::mat angles;

	//// wave utilities
	//arma::vec getHamming(void);
	//arma::vec getHanning(void);

	//arma::mat winFilter(arma::mat, arma::vec);

	//arma::mat getPhase(arma::cx_mat);
	//arma::cx_mat getComplex(arma::mat);

	//arma::mat segmentWav(arma::vec);
};

#endif /* WAVEPROCESSOR_H_ */
