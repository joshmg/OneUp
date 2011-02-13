/*! @file Printer.h
    @author Andrew Groot
    @brief Definition of the private data for the "Printer" class.
*/

#ifndef PRINTER_H
#define PRINTER_H

#include "iPrinter.h"
#include "SymbolTable.h"
#include "ResultCodes.h"
#include <string>
#include <fstream>
#include <map>

class Printer : public iPrinter {
  private:
    //! The input file.
    std::ifstream _inStream;
    //! The output file.
    std::ofstream _outStream;

    /*! @brief Set 0 or more bits of a word.
        @param[in:out] w The word whose bits are to be set.
        @param[in] value An integer corresponding to a bit mask.
        @param[in] index The starting position of bit-setting.
        @pre "index" is less than the size of the word.
        @pre "value" can be contained in |size of word| - "index" bits.

        @note
        This is essentially a localized bit-wise or.

        @par Examples
        Word=8, value=1, index=0 => Word=9.\n
        Word=1F, value=14, index=11 => Word=E01F.
    */
    void _SetBits(Word& w, int value, int index);


    /*! @brief Tests a string as a valid register.
        @param[in] reg The string to be tested.
        @return SUCCESS if reg is in the form RX where x is a number from 0 to 7.
        Otherwise, an appropriate error is returned.
    */
    Codes::RESULT _IsReg(string reg);


    /*! @brief Get the index of register from a string.
        @param[in] reg The register string.
        @pre reg is a valid register.
        @return The index of register alluded to by reg
    */
    int _RegNum(string reg);


    /*! @brief Get the index of register from a string.
        @param[in] reg The register string.
        @pre reg is a valid register.
        @return The index of register alluded to by reg
    */
    int _RegNum(string reg);


    /*! @brief Converts a Line into a Word.
        @param[in] line The Line to be converted.
        @param[in] symbols Any symbols that might show up.
        @param[out] w The Word representation of "line" given "symbols".
        @return SUCCESS or, if something went wrong, an appropriate error message.
    */
    Codes::RESULT _LineToWord(Line line, SymbolTable& symbols, Word& w);

  public:
    //! @brief Closes the input and output files, if necessary.
    ~Printer();

    Codes::RESULT Open(std::string infile, std::string outfile);
    Codes::RESULT Print(SymbolTable& symbols, Word file_length);
};

#endif
