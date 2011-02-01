/*! @file iListing.h
    @author Andrew Groot
    @brief Definition of the output of the Wi-11 assembler.
*/

#ifndef iLISTING_H
#define iLISTING_H

#include "iWord.h"
#include <string>

class iListing {
  private:

  public:
    virtual void Insert(iWord& address, iWord& instruction, std::string text) = 0;

    virtual iWord& Address(int index) const = 0;

    virtual iWord& Instruction(int index) const = 0;

    virtual iWord& Text(int index) const = 0;
};

#endif
