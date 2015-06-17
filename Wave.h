/*
 * Wave.h
 *
 *  Created on: Oct 2, 2014
 *      Author: pbandurs
 */

#ifndef WAVE_H_
#define WAVE_H_

#include "WaveProcessor.h"
#include <armadillo>
#include <string>


class Wave {
public:
	Wave(const std::string &in, const std::string &out, const int &samplerate, const int &channels, const bool &ss);
	virtual ~Wave();

    void read();
    void process();
    void save();

private:
	static const int BUFFER_LEN;
	int samplerate;
	int format;
	int channels;
    bool spec_sub;

    WaveProcessor waveProcessor;

    std::string inputfile;
    std::string outputfile;

    arma::vec inWave;
    arma::vec outWave;

    arma::mat inSpectrum;
    arma::mat outSpectrum;

    void readWave();
    void writeWave() const;
};

#endif /* WAVE_H_ */
