#ifndef Configuration_HH
#define Configuration_HH

// Std include
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "TString.h"
#include "TRegexp.h"
using namespace std;

// This class reads a configuration file and extracts polarity, attenuation,
// and amplification factors for each digitizer channel.
//
// Configuration file lines should be of the following form (lines beginning with a '#' are ignored):
// baseline i_start n_samples
// CH  POLARITY  AMPLIFICATION  ATTENUATION (dB)  ALGORITHM  FILTER_WIDTH
// with:
// CH - channel number (integer)
// POLARITY - sign of the pulse ('+' or '-'). Pulses are supposed to have the peak below the baseline
// AMPLIFICATION - in dB, amount of amplification that was applied to the input (float)
// ATTENUATION - in dB, amount of attenuation that was applied to the input (float)
// ALGORITHM - indicates the algorithm to run to extract pulse times (string).Usa a '+' to separate them
//      G: gaussian fit, G##
//      Re: linear fit at the rising edge, Re##-## possible in %
//      LP#: Local polinomial fit. For the moment min 1, max 3. e.g LP2
//      else --> No action
// FILTER_WIDTH - gaussian kernel width for Weierstrass transform (gaussian filter).
//      If 0, no Weierstrass transform will be applied.

class Configuration {
    public:
      struct Channel {
        unsigned int N = 0;
        int polarity = +1;
        float amplification = 0;
        float attenuation = 0;
        TString algorithm = "";
        float gaus_fraction = 0.4;
        float re_bounds[2] = {0.15, 0.75};
        vector<int> PL_deg;

        float weierstrass_filter_width = 0;
      };

      // Read and initilize from the configuration file
      Configuration(string config_file);

      map<unsigned int,Channel> channels;

      bool isValid();

      // get overall multiplier including polarity, amplification, and attenuation
      float getChannelMultiplicationFactor(unsigned int ch);

      // returns true if the specified channel is present in the config
      bool hasChannel(unsigned int ch);

      unsigned int baseline[2] = {5, 150};
      // must be between 0.1 and 0.9
      vector<float> constant_fraction = {0.15, 0.3, 0.45};

    private:
      // process one line of the config file
      void parseConfigurationLine(std::string line);
      // get next non-space token from config
      int nextConfigurationElement(std::stringstream &ss, std::string &item);
};

#endif
