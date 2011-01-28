/*! @file iLine.h
    @author Andrew Groot
    @brief Definition of a "Line" of Wi-11 assembly code.
*/

#ifndef iLINE_H
#define iLINE_H

#include <string>

class iLine {
  private:

  public:
    virtual ReadLine(std::string line) = 0;

    virtual std::string Label() const = 0;

    virtual std::string Instruction() const = 0;

    virtual std::string ToString() const = 0;

    virtual operator[] (int index) const = 0;

    virtual int Size() const = 0;
};

#endif
