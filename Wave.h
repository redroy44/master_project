/*
 * Wave.h
 *
 *  Created on: Oct 2, 2014
 *      Author: pbandurs
 */

#ifndef WAVE_H_
#define WAVE_H_

class Wave {
public:
	Wave();
	virtual ~Wave();
	int getNfft();
	void setNfft(int);
	int getSamplerate();
	void setSamplerate(int);

	static int samplerate;
	static int nfft;
	static int format;
	static int channels;
//	static const int framelen = 30; // framelen in ms
//	static const int overlap = 50; // overlap in %
};

#endif /* WAVE_H_ */
