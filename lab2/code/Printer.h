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

/*! @brief Implements the iPrinter interface.

    This implementation shows the complexity of
    a Printer component.  Much of the redundancy
    inherent in the concept of this class is
    accounted for through the use of its many private functions.
    However, the method chosen for returning error messages makes
    it very difficult to truly optimize this code.  Therefore,
    even much of the code specific to each type of operation is just a 
    variant of another component.  The handling of different instructions
    is done through a while loop containing a large
    if-elseif-elseif... statement.  The components are separated by
    their argument pattern in an attempt to compactify the code as
    much as possible while still maintaining its correctness and
    readability.
*/
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

    /*! @briefs Get a 16 value from a string.
        @param[in] op The string to be parsed.
        @param[in] symbols A table of symbols in case it is necessary.
        @returns The value contained in op, whether it is a constant or a symbol.  If the symbol is not defined that
will be handled outside of this function.
    */
    Word _ParseWord(const std::string& op, const SymbolTable& symbols);

    /*! @brief Checks a Pgoffset9 value.
        @param[in] value The value to be checked.
        @param[in] PC The value of the PC for value to be checked against.
        @returns True iff the highest order 7 bits of value and PC are the same.
    */
    bool _Check9(Word value, Word PC);

    /*! @brief Checks a 6-bit index.
        @param[in] value The value to be checked.
        @returns True iff value can be expressed in 6 bits.
    */
    bool _Check6(Word value);

    /*! @brief Checks a 5-bit immediate value.
        @param[in] value The value to be checked.
        @returns True iff value can be expressed in 5 bits.
    */
    bool _Check5(Word value);

    /*! @brief Prints to the console to make error messages more readable.
        @param[in] line The line in which the error was found.

        Shows the user the line with the error and formats the output to make
        error messages more readable.
    */
    void _PreError(const std::string& line);

    /*! @brief Prints the listing entry for a single non-pseudo-op instruction.
        @param[in] current_address The address of the instruction.
        @param[in] value The value to be store at current_address.
        @param[in] current_line The line that has been translated.
        @param[in] pos The current line number in the input file.
    */
    void _LineListing(const Word& current_address, const Word& value, const Line& current_line, const int& pos);

    /*! @brief Prints to the console to make listings more readable.
        @param[in] line_number The current line in the file.
        @param[in] current_line The line that has been translated.
    */
    std::string _InFileData(const int line_number, const Line& current_line);

  public:
    //! @brief Closes the input and output files, if necessary.
    ~Printer();

    Codes::RESULT Open(std::string infile, std::string outfile);
    Codes::RESULT Print(SymbolTable& symbols, Word& file_length);
};

#endif
