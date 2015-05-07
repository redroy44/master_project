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

class LsaEstimator {
public:
	LsaEstimator(const unsigned int &);
	virtual ~LsaEstimator();

    void estimateSpec(const arma::vec &, const arma::vec &);
	arma::vec getCleanSpectrum(void) const;

#ifdef DEBUG
    arma::mat matGain;
    arma::mat matSNRposteriori;
    arma::mat matSNRpriori;
#endif

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

    void snrDD(arma::vec &);
    void expint(arma::vec &);

    //conversion functions
    template<typename T> T mag2db(T &val) {return 20*log10(val); }
    template<typename T> T db2mag(T &val) {return exp10(val/20); }

    template<typename T> T pow2db(T &val) {return 10*log10(val); }
    template<typename T> T db2pow(T &val) {return exp10(val/10); }
};

#endif /* LSAESTIMATOR_H_ */
