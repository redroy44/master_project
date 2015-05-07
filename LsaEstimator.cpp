/*
 * LsaEstimator.cpp
 *
 *  Created on: 6 paź 2014
 *      Author: piotrek
 */

#include <LsaEstimator.h>
#include <armadillo>
#include <boost/math/special_functions/expint.hpp>

using namespace arma;

LsaEstimator::LsaEstimator(const unsigned int &nfft) {
	alpha = 0.98;

	gainLower = -60;
	gainUpper = 6;

	prioriLower = -30;
	prioriUpper = 60;

	int length = nfft/2;

	cleanSpectrum = zeros<vec>(length);
	gain = zeros<vec>(length);
	prevSNRposteriori = ones<vec>(length);
	SNRposteriori = ones<vec>(length);

}

LsaEstimator::~LsaEstimator() {
	// TODO Auto-generated destructor stub
}

void LsaEstimator::estimateSpec(const arma::vec &powerSpec, const arma::vec &powerNoise) {
    spectrum = sqrt(powerSpec);
    noise = sqrt(powerNoise);

    SNRposteriori = powerSpec / powerNoise; // A posteriori SNR
    vec SNRpriori;
    snrDD(SNRpriori);  // decision-directed A priori SNR

    // constrain a-priori SNR between lower and upper boundary
    SNRpriori = clamp(SNRpriori, db2pow(prioriLower), db2pow(prioriUpper));

    prevSNRposteriori = SNRposteriori; // save A posteriori SNR

    vec vk = SNRposteriori % (SNRpriori / (1 + SNRpriori)); // equation (8) in MMSE
    expint(vk);
    gain = (SNRpriori / (1 + SNRpriori)) % exp(0.5 * vk); // log-MMSE gain - equation (20) in log-MMSE

    // constrain gain between lower and upper boundary
    gain = clamp(gain, db2mag(gainLower), db2mag(gainUpper));

    cleanSpectrum = gain % spectrum; // lsa-estimate

#ifdef DEBUG
    matGain = join_horiz(matGain, gain);
    matSNRposteriori = join_horiz(matSNRposteriori, SNRposteriori);
    matSNRpriori = join_horiz(matSNRpriori, SNRpriori);
#endif
}

void LsaEstimator::snrDD(vec &SNRpriori) {
    SNRpriori = alpha * square(gain) % prevSNRposteriori
        + (1 - alpha) * max(SNRposteriori - 1, zeros<vec>(SNRposteriori.n_elem));
}

arma::vec LsaEstimator::getCleanSpectrum() const {
	return cleanSpectrum;
}

void LsaEstimator::expint(vec &vector) {
    vector.transform([](double val) { return boost::math::expint(2, val); });
    //for (unsigned int i = 0; i < vector.n_rows; i++) {
        //vector(i) = boost::math::expint(2, vector(i)); // vec(i) cannot be 0
    //}
}
