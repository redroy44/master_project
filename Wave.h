/*
 * Wave.h
 *
 *  Created on: Oct 2, 2014
 *      Author: pbandurs
 */

#ifndef WAVE_H_
#define WAVE_H_

#include <armadillo>
#include <string>

class Wave {
public:
	Wave(const std::string &in, const std::string &out);
	virtual ~Wave();

    void read();
    void process();
    void save();

	//int getNfft();
	//void setNfft(int);
	//int getSamplerate(void);
	//void setSamplerate(int);
	//float getOverlap(void);
	//void setOverlap(float);
	//int getFramelen(void);
	//void setFramelen(int);
	//int getChannels(void);
	//void setChannels(int);
	//int getFormat(void);
	//void setFormat(int);

private:
    std::string inputfile;
    std::string outputfile;
	//static int samplerate;
	//static int nfft;
	//static int format;
	//static int channels;
	//static int framelen; // framelen in frames
	//static float winlen; // framelen in ms
	//static float overlap; // overlap in %

protected:
    //float mag2db(float);
    //float db2mag(float);

    //float pow2db(float);
    //float db2pow(float);

    //arma::vec mag2db(arma::vec);
    //arma::vec db2mag(arma::vec);

    //arma::vec medFilter(arma::vec, int); // median filter of order n; consistent with matlab medfilt1
    //arma::vec meanFilter(arma::vec, int); // mean filter of order n;

};

#endif /* WAVE_H_ */
