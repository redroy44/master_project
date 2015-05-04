/*
 * NoiseEstimator.cpp
 *
 *  Created on: 5 paź 2014
 *      Author: piotrek
 */

#include <NoiseEstimator.h>

using namespace arma;

NoiseEstimator::NoiseEstimator(const unsigned int &nfft, const unsigned int &sr) {
	unsigned int length = nfft/2;
	eta = 0.7; // smoothing constant
	gamma = 0.998; // constant
	beta = 0.96; // constant
	alpha_p = 0.2; // smoothing constant - końcówki słów
	alpha_d  = 0.85; // constant

	delta = 2 * ones<vec>(length); // frequency-dependent speech-presence threshold
	alpha_s = zeros<vec>(length); // (7) time-frequency dependent smoothing factor

	noisyRatio = zeros<vec>(length); // (4) power spectrum to local minimum ratio
	spDecision = zeros<vec>(length); // (5) speech-presence decision
	spProbability = zeros<vec>(length); // (6) speech-presence probability

//	int bin1k = 2*(1 * length) / (sr / 1000.0f); // 16k samplerate 1kbin
	int  bin3k = 2*(3 * length) / (sr / 1000.0f); // 16k samplerate 3k bin

	//initialize delta eq(10.5)
	for (unsigned int i = bin3k; i < length; i++) {
		delta(i) = 5;
	}
}


NoiseEstimator::~NoiseEstimator() {

}

void NoiseEstimator::init(const vec &spectrum) {

	 prevSmPower = spectrum;
	 smPower = spectrum; // (2) smoothed noisy speech power spectrum
	 prevMinPower = spectrum;
	 minPower = spectrum; // (3) local minimum of noisy speech power spectrum
	 noiseSpectrum = spectrum; // (8) noise spectrum estimate

//#ifdef DEBUG
	 //matNoisyRatio = join_horiz(matNoisyRatio, noisyRatio);
	 //matSpDecision = join_horiz(matSpDecision, spDecision);
	 //matSpProbability = join_horiz(matSpProbability, spProbability);
	 //matAlpha = join_horiz(matAlpha, alpha_s);
	 //matSmPower = join_horiz(matSmPower, smPower);
	 //matMinPower = join_horiz(matMinPower, minPower);
//#endif
}

void NoiseEstimator::estimateNoise(const vec &spectrum) {
	smPower = (eta * prevSmPower) + ((1 - eta) * spectrum); // eq (2)

    for (unsigned int i = 0; i < minPower.n_rows; i++) { // begin eq (3)
        if (prevMinPower(i) < smPower(i)) {
            minPower(i) = (gamma * prevMinPower(i)) +
               (((1 - gamma) / (1 - beta)) * (smPower(i) - beta * prevSmPower(i)));
        }
        else {
            minPower(i) = smPower(i);
        }
    } // end eq (3)

    noisyRatio = smPower / minPower; // eq (4)

    for (unsigned int i = 0; i < spDecision.n_rows; i++) { // begin eq (5)
        if (noisyRatio(i) > delta(i)) {
            spDecision(i) = 1;
        }
        else {
            spDecision(i) = 0;
        }
    } // end eq (5)

    spProbability = alpha_p * spProbability + (1 - alpha_p) * spDecision; // eq (6)
    alpha_s = repmat(alpha_d, alpha_s.n_rows, 1) + (1 - repmat(alpha_d, alpha_s.n_rows, 1)) % spProbability; // eq (7) alpha_d <= alpha_s <= 1
    noiseSpectrum = alpha_s % noiseSpectrum + (1 - alpha_s) % smPower; // eq (8) //change from spectrum

    // use mean filter smoothing
//    noiseSpectrum = medFilter(noiseSpectrum, 5);
//    noiseSpectrum = meanFilter(noiseSpectrum, 5);

//#ifdef DEBUG
    //matNoisyRatio = join_horiz(matNoisyRatio, noisyRatio);
    //matSpDecision = join_horiz(matSpDecision, spDecision);
    //matSpProbability = join_horiz(matSpProbability, spProbability);
    //matAlpha = join_horiz(matAlpha, alpha_s);
    //matSmPower = join_horiz(matSmPower, smPower);
    //matMinPower = join_horiz(matMinPower, minPower);
//#endif

    prevSmPower = smPower; // update mcra2 parameters
    prevMinPower = minPower;
}

arma::vec NoiseEstimator::getNoiseSpectrum() const {
	return noiseSpectrum;
}

