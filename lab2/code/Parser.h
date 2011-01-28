/*! @file iParser.h
    @author Andrew Groot
    @brief Definition of the private data for the "Parser" class.
*/

#ifndef PARSER_H
#define PARSER_H

#include "iParser.h"
#include "iLine.h"
#include <string>
#include <vector>
#include <fstream>

class Parser : public iParser {
  private:
    std::ifstream _fileStream;

  public:
    ~Parser();

    bool Load(std::string filename);
    std::vector<iLine&> GetLines();
}

#endif
