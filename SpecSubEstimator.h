#ifndef SPECSUBESTIMATOR_H_
#define SPECSUBESTIMATOR_H_

#include <armadillo>

class SpecSubEstimator {
public:
    SpecSubEstimator(const unsigned int &nfft);
    virtual ~SpecSubEstimator();

    void estimateSpec(const arma::vec &, const arma::vec &);
    void estimateNoise(const arma::mat &);
	arma::vec getCleanSpectrum(void) const;
	//arma::vec getNoiseEstimate(void) const;

private:
    float ssAlpha;
    float ssFloor;

    arma::vec cleanSpectrum;
    arma::vec noise;
}
#endif
