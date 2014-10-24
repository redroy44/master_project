/*
 * LsaEstimator.h
 *
 *  Created on: 6 paź 2014
 *      Author: piotrek
 */

#ifndef LSAESTIMATOR_H_
#define LSAESTIMATOR_H_

#include <Wave.h>
#include <armadillo>

class LsaEstimator: public Wave {
public:
	LsaEstimator();
	virtual ~LsaEstimator();

    void estimateSpec(arma::vec, arma::vec);
	arma::vec getCleanSpectrum(void);

private:
	double alpha;

	float gainUpper;	// upper gain boundary in dB
	float gainLower;	// lower gain boundary in dB

	float prioriUpper;	// upper a-priori SNR boundary in dB
	float prioriLower;	// lower a-priori SNR boundary in dB


    arma::vec spectrum;
    arma::vec cleanSpectrum;
    arma::vec noise;
    arma::vec gain;
    arma::vec prevSNRposteriori;
    arma::vec SNRposteriori;

    arma::vec snrDD(void);
    arma::vec expint(arma::vec);
};

#endif /* LSAESTIMATOR_H_ */
