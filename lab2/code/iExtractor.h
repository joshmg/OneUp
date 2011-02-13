/*! @file iExtractor.h
    @author Andrew Groot
    @brief Definition of the Extractor for the Wi-11 assembler.
*/

#ifndef iEXTRACTOR_H
#define iEXTRACTOR_H

#include "SymbolTable.h"
#include "ResultCodes.h"
#include <string>
#include <map>

/*! @brief Extracts symbols from a source file to be used on a second read.

    This class provides a simple interface to fill a SymbolTable object
    and retrieve the size in memory this program would require.
*/
class iExtractor {
  private:

  public:
    /*! @brief Opens the input file to be read.
        @param[in] filename The source file.
        @return True iff the file was successfully opened.
    */
    virtual bool Open(std::string filename) = 0;

    /*! @brief Extracts the symbols and literals from the source file.
        @param[in:out] symbols The object into which the symbols and literals will be stored.
        @return SUCCESS iff the file syntax is sound, otherwise an appropriate error code.

        This opeeration acts as the first of the two passes on the source file.
        Everything that can be checked on this first pass will, all other possible errors
        will be left until the second pass.
    */
    virtual Codes::RESULT GetSymbols(SymbolTable& symbols) = 0;

    /*! @pre #GetSymbols() has already been successfully run.
        @return The size of the space in memory needed to hold the source file.
    */
    virtual Word GetLength() const = 0;
};

#endif
