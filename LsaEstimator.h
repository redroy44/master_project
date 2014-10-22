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

    double alpha;

    void estimateSpec(arma::vec, arma::vec);
	arma::vec getCleanSpectrum(void);

#ifdef DEBUG
	arma::mat matGain;
	arma::mat matSNRposteriori;
	arma::mat matSNRpriori;
#endif

private:
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
