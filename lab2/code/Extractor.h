/*! @file iExtractor.h
    @author Andrew Groot
    @brief Definition of the private data for the "Extractor" class.
*/

#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include "iExtractor.h"
#include "SymbolTable.h"
#include "iWord.h"
#include "ResultCodes.h"
#include <string>
#include <map>
#include <fstream>

class Extractor : public iExtractor {
  private:
    std::ifstream _fileStream;

  public:
    ~Extractor();

    bool Open(std::string filename);
    Codes::RESULT GetSymbolTable(SymbolTable symbols);
    iWord& GetLength() const;
};

#endif
