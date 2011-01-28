/*! @file iParser.h
    @author Andrew Groot
    @brief Definition of the Parser for the Wi-11 assembler.
*/

#ifndef iPARSER_H
#define iPARSER_H

#include "iWord.h"
#include "iLine.h"
#include <string>
#include <map>

class iParser {
  private:

  public:
    virtual bool Load(std::string filename, bool include_traps) = 0;

    virtual std::map<iWord&, iLine&> GetLines() = 0;
};

#endif
