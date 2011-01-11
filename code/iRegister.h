// File: iRegister.h
// Written by:
//    Joshua Green

#ifndef iREGISTER_H
#define iREGISTER_H

#include "iWord.h"

class Word;
class Register;

class iRegister {
  private:
    
  public:
    virtual Word getValue() const = 0;

    virtual Register Add(const iRegister&) const = 0;
    virtual Register operator+(const iRegister&) const = 0;
    
    virtual Register Subtract(const iRegister&) const = 0;
    virtual Register operator-(const iRegister&) const = 0;

    virtual Register And(const iRegister&) const = 0;
    virtual Register Or(const iRegister&) const = 0;
    virtual Register Not(const iRegister&) const = 0;

    virtual void Store(const iRegister&) const = 0;
    virtual void Store(const iWord&) const = 0;

    virtual Register linkCCR(iRegister* const) const = 0;

    virtual Register operator++() const = 0;
    virtual Register operator++(int) const = 0;

    virtual Register operator=(const iWord&) const = 0;
    virtual Register operator=(const iRegister&) const = 0;
};

#endif
