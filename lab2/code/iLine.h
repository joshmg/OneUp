/*! @file iLine.h
    @author Andrew Groot
    @brief Definition of a "Line" of Wi-11 assembly code.
*/

#ifndef iLINE_H
#define iLINE_H

#include "Word.h"
#include "ResultCodes.h"
#include <string>

/*! @brief Stores information about a Line of Wi-11 assembly code.

    This class defines an interface for pulling relevent data
    from a line of assembly code without having to handle whitespace
    or test for various syntactic properties.
*/
class iLine {
  private:

  public:
    /*! @brief Parses and tokenizes a string.
        @param[in] line The line to be parsed.
        @return SUCCESS iff the line is valid wi-11 assembly code,
                otherwise an appropriate error code.
    */
    virtual Codes::RESULT ReadLine(std::string line) = 0;


    /*! @pre HasLabel() returns true.
        @return The label found at the beginning of the line.
    */
    virtual std::string Label() const = 0;


    /*! @pre HasLiteral() returns true.
        @return The literal found in the line.
    */
    virtual int Literal() const = 0;


    /*! @return The instruction found in the line.
    */
    virtual std::string Instruction() const = 0;


    /*! @brief Provides access to the arguments of the instruction.
        @param[in] index The index of the argument desired, starting at 0 for the first one.
        @return Argument number "index".
    */
    virtual std::string operator[] (int index) const = 0;


    /*! @return The number of arguments to the instruction.
    */
    virtual int Size() const = 0;


    /*! @return The line of code as found.
    */
    virtual std::string ToString() const = 0;


    /*! @return True iff a label was found in the line.
    */
    virtual bool HasLabel() const = 0;


    /*! @return True iff the instruction is a pseudo-op (.ORIG, .END, .EQU, etc.).
    */
    virtual bool IsPseudoOp() const = 0;


    /*! @return True iff a literal was found in the line.
    */
    virtual bool HasLiteral() const = 0;


    /*! @return True iff he line only contained a comment or whitespace.
    */
    virtual bool IsComment() const = 0;
};

#endif
