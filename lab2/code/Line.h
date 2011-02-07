/*! @file Line.h
    @author Andrew Groot
    @brief Definition of the private data for the "Line" class.
*/

#ifndef LINE_H
#define LINE_H

#include "iLine.h"
#include "iWord.h"
#include "ResultCodes.h"
#include <string>
#include <vector>

class Line : public iLine {
  private:
    //! Label, if any.  Empty string otherwise.
    std::string _label;
    //! Instruction, if any. Empty string otherwise.
    std::string _inst;
    //! Holds each argument to the instruction, if any.
    vector<std::string> _args;
    //! Holds a literal value, if any.
    int _literal;
    //! True iff line contained a literal.
    bool _hasLiteral;
    //! True iff line was a comment.
    bool _comment;

    /*! @brief Tests a character for whitespace.
        @param[in] ch The character to be tested.
        @return True iff ch is a space or a tab.
    */
    bool _IsWS(char ch) const;
    /*! @brief Get the next whitespace-free sub-string.
        @param[in] str The string from which to obtain the substring.
        @returns The first whitespace-free substring of str.

        If str contains only whitespace, the empty string is returned.
    */
    std::string _GetNext(std::string& str) const;
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
    void _SetBits(iWord& w, int value, int index);
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
    /*! @brief Tests a string as a valid constant and store any literals.
        @param[in] constant The string representation of the constant.
        @param[in] size The number of bits allowed for this constant.
        @return SUCCESS if the string is valid; an appropriate error otherwise.
        
        @par Acceptable formats
        Decimal numbers should be preceeded with a "#" symbol.
        @arg Positive and negative numbers are allowed.
        Hexadecimal number should be preceeded with an "x".
        @arg No leading zeros are necessary.
    */
    int _ReadConstant(string constant);

  public:
    Codes::RESULT ReadLine(std::string line);

    std::string Label() const;
    std::string Instruction() const;
    std::string operator[] (int index) const;
    int Size() const;

    std::string ToString() const;
    iWord& ToWord() const;

    bool HasLiteral() const;
    bool IsComment() const;
};

#endif
