/*! @file iEncoder.h
    @author Andrew Groot
    @brief Definition of the encoding method of the Wi-11 assembler.
*/

#ifndef iENCODER_H
#define iENCODER_H

#include "iListing.h"
#include "iLine.h"
#include <vector>

class iEncoder {
  private:

  public:
    virtual iListing& Encode(vector<iLine&> lines) const = 0;
};

#endif
