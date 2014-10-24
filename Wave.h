/*
 * Wave.h
 *
 *  Created on: Oct 2, 2014
 *      Author: pbandurs
 */

#ifndef WAVE_H_
#define WAVE_H_

#include <armadillo>

class Wave {
public:
	Wave();
	virtual ~Wave();
	int getNfft();
	void setNfft(int);
	int getSamplerate(void);
	void setSamplerate(int);
	float getOverlap(void);
	void setOverlap(float);
	int getFramelen(void);
	void setFramelen(int);
	int getChannels(void);
	void setChannels(int);
	int getFormat(void);
	void setFormat(int);

	float HzTodB(float);
	float DbToHz(float);

	arma::vec HzTodB(arma::vec);
	arma::vec DbToHz(arma::vec);

private:
	static int samplerate;
	static int nfft;
	static int format;
	static int channels;
	static int framelen; // framelen in frames
	static float winlen; // framelen in ms
	static float overlap; // overlap in %
};

#endif /* WAVE_H_ */
