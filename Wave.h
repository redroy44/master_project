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
	static int framelen; // framelen in frames
	static int winlen; // framelen in ms

	static float overlap; // overlap in %
};

#endif /* WAVE_H_ */
