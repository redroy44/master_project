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

	int getSamplerate(void);
	int getNfft(void);
	int getFramelen(void);
	int getOverlap(void);

private:
	static const int samplerate = 16000;
	static const int nfft = 256;
//	static const int framelen = 30; // framelen in ms
//	static const int overlap = 50; // overlap in %
};

#endif /* WAVE_H_ */
