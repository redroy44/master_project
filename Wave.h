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
	Wave(const std::string &in, const std::string &out, const int &samplerate, const int &channels);
	virtual ~Wave();

    void read();
    void process();
    void save();

private:
	static const int BUFFER_LEN;

    std::string inputfile;
    std::string outputfile;
    WaveProcessor waveProcessor;
    arma::vec inWave;
    arma::vec outWave;

    arma::mat inSpectrum;
    arma::mat outSpectrum;

    void readWave();
    void writeWave() const;
	int samplerate;
	int format;
	int channels;
};

#endif /* WAVE_H_ */
