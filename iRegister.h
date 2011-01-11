// File: iRegister.h
// Written by:
//    Joshua Green

#ifndef iREGISTER_H
#define iREGISTER_H

#include "iWord.h"

class iRegister {
  private:
    
  public:
    virtual iWord getValue() const = 0;

    virtual iRegister Add(const iRegister&) const = 0;
    virtual iRegister operator+(const iRegister&) const = 0;
    
    virtual iRegister Subtract(const iRegister&) const = 0;
    virtual iRegister operator-(const iRegister&) const = 0;

    virtual iRegister And(const iRegister&) const = 0;
    virtual iRegister Or(const iRegister&) const = 0;
    virtual iRegister Not(const iRegister&) const = 0;

    virtual void Store(const iRegister&) const = 0;
    virtual void Store(const iWord&) const = 0;

    virtual iRegister linkCCR(iRegister* const) const = 0;

    virtual iRegister operator++() const = 0;
    virtual iRegister operator++(int) const = 0;

    virtual iRegister operator=(const iWord&) const = 0;
    virtual iRegister operator=(const iRegister&) const = 0;
};

#endif
