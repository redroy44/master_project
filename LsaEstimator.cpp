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

LsaEstimator::LsaEstimator() {
	alpha = 0.97;

	gainLower = -60;
	gainUpper = 0;

	prioriLower = -15;
	prioriUpper = 15;

	int length = getNfft();

	cleanSpectrum = zeros<vec>(length);
	gain = zeros<vec>(length);
	prevSNRposteriori = ones<vec>(length);
	SNRposteriori = ones<vec>(length);

}

LsaEstimator::~LsaEstimator() {
	// TODO Auto-generated destructor stub
}

void LsaEstimator::estimateSpec(arma::vec powerSpec, arma::vec powerNoise) {
    spectrum = sqrt(powerSpec);
    noise = sqrt(powerNoise);

    SNRposteriori = powerSpec / powerNoise; // A posteriori SNR
    vec SNRpriori = snrDD();  // decision-directed A priori SNR

    // constrain a-priori SNR between lower and upper boundary
    gain = clamp(gain, db2mag(prioriLower), db2mag(prioriUpper));

    prevSNRposteriori = SNRposteriori; // save A posteriori SNR

    vec vk = SNRposteriori % (SNRpriori / (1 + SNRpriori)); // equation (8) in MMSE

    gain = (SNRpriori / (1 + SNRpriori)) % exp(0.5 * expint(vk)); // log-MMSE gain - equation (20) in log-MMSE

    // constrain gain between lower and upper boundary
    gain = clamp(gain, db2mag(gainLower), db2mag(gainUpper));

    // use armadillo clamp function !
    for (unsigned int i = 0; i < gain.n_rows; i++) {
        if (gain(i) < 0.001){
            gain(i) = 0.001;
        } else if (gain(i) > 1){
        	gain(i) = 1;
        }
    }

    cleanSpectrum = gain % spectrum; // lsa-estimate

#ifdef DEBUG
    matGain = join_horiz(matGain, gain);
    matSNRposteriori = join_horiz(matSNRposteriori, SNRposteriori);
    matSNRpriori = join_horiz(matSNRpriori, SNRpriori);
#endif
}

arma::vec LsaEstimator::snrDD(void) {
    vec SNRpriori = alpha * square(gain) % prevSNRposteriori
        + (1 - alpha) * max(SNRposteriori - 1, zeros<vec>(SNRposteriori.n_elem));

    return SNRpriori;
}

arma::vec LsaEstimator::getCleanSpectrum() {
	return cleanSpectrum;
}

arma::vec LsaEstimator::expint(vec vector) {
    for (unsigned int i = 0; i < vector.n_rows; i++) {
        vector(i) = boost::math::expint(2, vector(i)); // vec(i) cannot be 0
    }
    return vector;
}
