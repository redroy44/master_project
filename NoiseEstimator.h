/*
 * NoiseEstimator.h
 *
 *  Created on: 5 paź 2014
 *      Author: piotrek
 */

#ifndef NOISEESTIMATOR_H_
#define NOISEESTIMATOR_H_

#include <Wave.h>
#include <armadillo>

class NoiseEstimator {
public:
	NoiseEstimator(const unsigned int &, const unsigned int &);
	virtual ~NoiseEstimator();

	void init(const arma::vec &);
	void estimateNoise(const arma::vec &);
	const arma::vec & getNoiseSpectrum(void) const;

#ifdef DEBUG
    arma::mat matNoisyRatio;
    arma::mat matSpDecision;
    arma::mat matSpProbability;
    arma::mat matAlpha;
    arma::mat matSmPower;
    arma::mat matMinPower;
#endif

private:
    double eta; // smoothing constant
    double gamma; // constant
    double beta; // constant
    double alpha_p; // smoothing constant
    arma::mat alpha_d; // constant
    arma::vec delta; // frequency-dependent speech-presence threshold
    arma::vec alpha_s; // (7) time-frequency dependent smoothing factor

    arma::vec prevSmPower;
    arma::vec smPower; // (2) smoothed noisy speech power spectrum
    arma::vec prevMinPower;
    arma::vec minPower; // (3) local minimum of noisy speech power spectrum
    arma::vec noisyRatio; // (4) power spectrum to local minimum ratio
    arma::vec spDecision; // (5) speech-presence decision
    arma::vec prevSpProbability;
    arma::vec spProbability; // (6) speech-presence probability
    arma::vec prevNoiseSpectrum;
    arma::vec noiseSpectrum; // (8) noise spectrum estimate

    // filter functions
    void medFilter(arma::vec &, const arma::uword); // median filter of order n; consistent with matlab medfilt1
    void meanFilter(arma::vec &, const arma::uword); // mean filter of order n;

};

#endif /* NOISEESTIMATOR_H_ */
