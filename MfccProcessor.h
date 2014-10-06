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



private:
	arma::vec filterbank;

	void generateFilterbank(void);


};

#endif /* MFCCPROCESSOR_H_ */
