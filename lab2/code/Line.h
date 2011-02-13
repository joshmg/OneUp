/*! @file Line.h
    @author Andrew Groot
    @brief Definition of the private data for the "Line" class.
*/

#ifndef LINE_H
#define LINE_H

#include "iLine.h"
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
    std::vector<std::string> _args;
    //! Holds a copy of the line of code.
    std::string _code;
    //! Holds a literal value, if any.
    int _literal;
    //! True iff line contained a label.
    bool _hasLabel;
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


    /*! @brief Tests the arguments extracted from the line for validity.
        @return SUCCESS if the arugments are valid; an appropriate error otherwise.
        
        @par Criteria for an arugment
        A valid argument fits into one of the following categories:
        <ul>
        <li>Register</li>
          <ul>
          <li>Something of the form RX, 0 < X < 7.</li>
          </ul>
        <li>Constant</li>
          <ul>
          <li>A decimal or hexadecimal value.</li>
          <li>A decimal value is denoted with a preceding '#'.
            <ul>
            <li>Example: #14 = 14 in decimal.</li>
            </ul>
          <li>A hexadecimal value is denoted with a preceding 'x'.
            <ul>
            <li>Example: x14 = 20 in decimal.<li>
            <li>Example: xFFFF = -1 in decimal (for 16-bit constant).
            </ul>
          <li>Either of these can be a literal (denoted with an '=').
            <ul>
            <li>Literals are actually stored in the memory of the wi-11 machine.</li>
            <li>To do this, the '=' is simply placed in front of the '#' or 'x'.</li>
            </ul>
          </ul>

        <li>Label</li>
          <ul>
          <li>A string beginning with an alphanumeric character that is no 'R' or 'x'.</li>
          </ul>
          </ul>
        <ul>
    */
    Codes::RESULT _CheckArgs();

  public:
    Codes::RESULT ReadLine(std::string line);

    std::string Label() const;
    std::string Instruction() const;
    std::string operator[] (int index) const;
    int Size() const;
    int Literal() const;

    std::string ToString() const;

    bool HasLabel() const;
    bool IsPseudoOp() const;
    bool HasLiteral() const;
    bool IsComment() const;
};

#endif
