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

/*! @brief Implements the iLine interface.

    This class store redundant amounts of information
    separated in different ways to allow the client
    to recreate and reformat the original text with ease.
*/
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
        
        Here, valid arguments are as defined in the \ref operands sections.
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
