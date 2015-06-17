#include "SpecSubEstimator.h"
#include <armadillo>

using namespace arma;

SpecSubEstimator::SpecSubEstimator(const unsigned int &nfft) {
    ssAlpha = 2;
    ssFloor = 0.5;

    arma::vec noise = zeros<vec>(nfft/2);
    arma::vec cleanSpectrum = zeros<vec>(nfft/2);
}

LsaEstimator::~LsaEstimator() {
	// TODO Auto-generated destructor stub
}

void SpecSubEstimator::estimateSpec(const arma::vec &powerSpec, const arma::vec &powerNoise) {
}

void SpecSubEstimator::estimateNoise(const arma::mat &noiseRegion) {
}

arma::vec getCleanSpectrum(void) const {
    return cleanSpectrum;
}
