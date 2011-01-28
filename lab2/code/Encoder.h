/*! @file Encoder.h
    @author Andrew Groot
    @brief Definition of the private data of the "Encoder" class. (none)
*/

#ifndef ENCODER_H
#define ENCODER_H

#include "iEncoder.h"
#include "iListing.h"
#include "iLine.h"
#include <vector>

class Encoder : public iEncoder {
  private:

  public:
    iListing& Encode(vector<iLine&> lines);
}

#endif
