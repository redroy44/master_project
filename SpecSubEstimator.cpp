#include "SpecSubEstimator.h"
#include <armadillo>

using namespace arma;

SpecSubEstimator::SpecSubEstimator(const unsigned int &nfft) {
    ssAlpha = 2;
    ssFloor = 0.5;

    arma::vec noise = zeros<vec>(nfft/2);
    arma::vec cleanSpectrum = zeros<vec>(nfft/2);
}

SpecSubEstimator::~SpecSubEstimator() {
	// TODO Auto-generated destructor stub
}

void SpecSubEstimator::estimateSpec(const arma::vec &spectrum) {
    vec gain = (square(spectrum) - ssAlpha * square(noise)) / square(spectrum);

    for (unsigned int i = 0; i < gain.n_elem; i++) {
        if (gain[i] < 0) {
            gain[i] = ssFloor;
        }
    }
    gain = sqrt(gain);

    cleanSpectrum = gain % spectrum;
}

void SpecSubEstimator::estimateNoise(const arma::mat &noiseRegion) {
    noise = arma::mean(noiseRegion, 1);
}

arma::vec SpecSubEstimator::getCleanSpectrum(void) const {
    return cleanSpectrum;
}
