/*! @file iParser.h
    @author Andrew Groot
    @brief Definition of the Parser for the Wi-11 assembler.
*/

#ifndef iPARSER_H
#define iPARSER_H

#include "iLine.h"
#include <string>
#include <vector>

class iParser {
  private:

  public:
    virtual bool Load(std::string filename) = 0;

    virtual std::vector<iLine&> GetLines() = 0;
}

#endif
