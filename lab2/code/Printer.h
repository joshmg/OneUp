/*! @file Printer.h
    @author Andrew Groot
    @brief Definition of the private data for the "Printer" class.
*/

#ifndef PRINTER_H
#define PRINTER_H

#include "iPrinter.h"
#include "Line.h"
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
    Codes::RESULT _IsReg(std::string reg);


    /*! @brief Get the index of register from a string.
        @param[in] reg The register string.
        @pre reg is a valid register.
        @return The index of register alluded to by reg
    */
    int _RegNum(std::string reg);

    /*! @brief Sets the bits of initial_mem for registers.
        @param[in] reg The register string.
        @param[in] initial_mem The Word object to be changed.
        @param[in] bit_offset The location of the first bit to be set (leftmost being 15). Passed by reference for cascade effect.
        @pre bit_offset > 2
        @post initial_mem has bits [bit_offset:bit_offset-2] appropriately set
    */
    void _SetBits(std::string reg, Word& initial_mem, int& bit_offset);

    Word _ParseWord(const std::string& op, const SymbolTable& symbols);

    bool _Check9(Word value, Word PC);
    bool _Check6(Word value);
    bool _Check5(Word value);

    void _LineListing(const Word& current_address, const Word& value, const Line& current_line, const int& pos);

    std::string _InFileData(const int line_number, const Line& current_line);

  public:
    //! @brief Closes the input and output files, if necessary.
    ~Printer();

    Codes::RESULT Open(std::string infile, std::string outfile);
    Codes::RESULT Print(SymbolTable& symbols, Word& file_length);
};

#endif
