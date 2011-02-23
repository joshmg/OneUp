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
#define SYMBOL_TABLE_MAX_SIZE 1000

/*! @brief Implements the iExtractor interface.

    This implementation is very nearly trivial.
    In this file the default maximum symbol table size is declared
    and store the run-time value of the max will be stored in the
    Extractor object.  The private variable keeping track of the
    object file length is an int to facilitate the reporting of
    a file whose length has exceeded the upper boundary of memory.
*/
class Extractor : public iExtractor {
  private:
    //! The input file
    std::ifstream _fileStream;
    //! Keep length after SymbolTable is filled
    int _length;
    //! The maximum size of the symbol table
    int _max_size;

    /*! @brief Creates a string "Line n", with n = pos.
        @param[in] pos The line number.
        @return A string as described above.
    */
    std::string _LineNumber(int pos);

  public:
    //! Gets the default max number of symbols, defaults on 1000.
    Extractor(int size = SYMBOL_TABLE_MAX_SIZE);
    //! Closes the input file, if necessary.
    ~Extractor();

    bool Open(std::string filename);
    Codes::RESULT GetSymbols(SymbolTable& symbols);
    Word GetLength() const;
};

#endif
