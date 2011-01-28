/*! @file Encoder.h
    @author Andrew Groot
    @brief Definition of the private data of the "Encoder" class. (none)
*/

#ifndef ENCODER_H
#define ENCODER_H

#include "iEncoder.h"
#include "iListing.h"
#include "iWord.h"
#include "iLine.h"
#include <string>
#include <vector>

class Encoder : public iEncoder {
  private:
    map<std::string, iWord&> _labels;

  public:
    Encoder(map<std::string, iWord&> labels);

    iListing& Encode(vector<iLine&> lines);
};

#endif
