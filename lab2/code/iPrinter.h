/*! @file iPrinter.h
    @author Andrew Groot
    @brief Definition of the output of the Wi-11 assembler.
*/

#ifndef iPRINTER_H
#define iPRINTER_H

#include "iListing.h"
#include <string>

class iPrinter {
  private:

  public:
    virtual bool Open(std::string filename) = 0;

    virtual void Print() = 0;
};

#endif
