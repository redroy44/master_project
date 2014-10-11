/*
 * MfccProcessor.h
 *
 *  Created on: 6 paź 2014
 *      Author: piotrek
 */

#ifndef MFCCPROCESSOR_H_
#define MFCCPROCESSOR_H_

#include <Wave.h>
#include <armadillo>

class MfccProcessor: public Wave {
public:
	MfccProcessor();
	virtual ~MfccProcessor();

	void computeMfcc(void);

private:
	arma::mat filterbank;
	arma::vec logCoeffs;
	arma::vec prevVecMfcc;
	arma::vec vecMfcc;

	// accumulated mfcc matrix
	arma::mat matMfcc;




	void generateFilterbank(void);
	void computeLogCoefficients(void);
	void computeDCT(void);
	void computeDelta(void);
	void computeDeltaDelta(void);


};

#endif /* MFCCPROCESSOR_H_ */
