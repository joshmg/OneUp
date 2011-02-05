/*! @file iLine.h
    @author Andrew Groot
    @brief Definition of a "Line" of Wi-11 assembly code.
*/

#ifndef iLINE_H
#define iLINE_H

#include "ResultCodes.h"
#include <string>

class iLine {
  private:

  public:
    virtual Codes::RESULT ReadLine(std::string line) = 0;

    virtual std::string GetLabel() const = 0;

    virtual int GetLiteral() const = 0;

    virtual std::string OpCode() const = 0;

    virtual std::string operator[] (int index) const = 0;

    virtual int Size() const = 0;

    virtual std::string ToString() const = 0;

    virtual iWord& ToWord() const = 0;
};

#endif
