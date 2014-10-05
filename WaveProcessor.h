/*
 * WaveProcessor.h
 *
 *  Created on: 5 paź 2014
 *      Author: piotrek
 */

#ifndef WAVEPROCESSOR_H_
#define WAVEPROCESSOR_H_

#include "Wave.h"
#include "sndfile.hh"
#include <armadillo>

class WaveProcessor: public Wave {
public:
	WaveProcessor(std::string, std::string);
	virtual ~WaveProcessor();

	arma::vec readWave(void);
	void writeWave(arma::vec);

	// wave utilities
	arma::vec getHamming(void);
	arma::vec getHanning(void);

	arma::mat winFilter(arma::mat, arma::vec);

	arma::mat getPhase(arma::cx_mat);
	arma::mat getComplex(arma::mat, arma::mat);

private:
	static const int BUFFER_LEN;
	std::string inWaveFile;
	std::string outWaveFile;


};

#endif /* WAVEPROCESSOR_H_ */
