/*! @file iExtractor.h
    @author Andrew Groot
    @brief Definition of the private data for the "Extractor" class.
*/

#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include "iExtractor.h"
#include "iWord.h"
#include "ResultCodes.h"
#include <string>
#include <map>
#include <fstream>

class Extractor : public iExtractor {
  private:
    std::ifstream _fileStream;
    std::map<std::string, iWord&>* _symbols;
    std::map<int, iWord&>* _literals;

  public:
    ~Extractor();

    bool Load(std::string filename);
    Codes::RESULT Read();
    std::map<std::string, iWord&>& GetSymbols() const;
    std::map<int, iWord&>& GetLiterals() const;
    iWord& GetLength() const;
};

#endif
