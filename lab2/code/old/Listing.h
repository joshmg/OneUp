/*! @file iListing.h
    @author Andrew Groot
    @brief Definition of the private data for the "Listing" class.
*/

#ifndef LISTING_H
#define LISTING_H

#include "iListing.h"
#include "iWord.h"
#include <string>
#include <vector>

class Listing : public iListing {
  private:
    vector<iWord&> _addresses;
    vector<iWord&> _instructions;
    vector<std::string> _code;

  public:
    void Insert(iWord& address, iWord& instruction, std::string text);
    iWord& Address(int index) const;
    iWord& Instruction(int index) const;
    std::string Text(int index) const;
};

#endif
