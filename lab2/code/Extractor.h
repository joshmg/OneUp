/*! @file Extractor.h
    @author Andrew Groot
    @brief Definition of the private data for the "Extractor" class.
*/

#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include "iExtractor.h"
#include "Word.h"
#include "SymbolTable.h"
#include "ResultCodes.h"
#include <string>
#include <map>
#include <fstream>

class Extractor : public iExtractor {
  private:
    //! The input file
    std::ifstream _fileStream;
    //! Keep length after SymbolTable is filled
    Word _length;

  public:
    //! Closes the input file, if necessary.
    ~Extractor();

    bool Open(std::string filename);
    Codes::RESULT GetSymbolTable(SymbolTable& symbols);
    Word GetLength() const;
};

#endif
