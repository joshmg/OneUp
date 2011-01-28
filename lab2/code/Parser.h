/*! @file iParser.h
    @author Andrew Groot
    @brief Definition of the private data for the "Parser" class.
*/

#ifndef PARSER_H
#define PARSER_H

#include "iParser.h"
#include "iWord.h"
#include "iLine.h"
#include <string>
#include <map>
#include <fstream>

class Parser : public iParser {
  private:
    std::ifstream _fileStream;
    bool _traps;

  public:
    ~Parser();

    bool Load(std::string filename, bool include_traps);
    std::map<iWord&, iLine&> GetLines();
};

#endif
