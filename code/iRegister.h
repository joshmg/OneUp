// File: iRegister.h
// Written by:
//    Joshua Green

#ifndef iREGISTER_H
#define iREGISTER_H

#include "iWord.h"

class Word;
class Register;

class iRegister {
    /*  iRegister is a component to mimic the functionality of Registers
        found within the Wi-11 Machine. iRegister is the base class, which
        will be derived General Purpose registers as well as a Program
        Counter class. Registers are expected to modify the CCR when
        applicable. iRegister provides basic functionality, including 
        addition, subtraction, logical and/or/not, and storing. The direct
        value contained within iRegister is accessible.
    */
  private:
    
  public:
    virtual Word getValue() const = 0;
    /*  getValue() copies and returns the value stored within iRegister.
        The value stored within iRegister, as we as the object itself
        remain unchanged and are therefore the function call is const.
    */

    virtual void Add(const iWord&) = 0;
    virtual Register Add(const iRegister&) const = 0;
    /*  Add(iRegister) mathematically adds the value of the given
        parameter's value with its current value and returns the result.
        Neither the parameter nor the current instance of iRegister is
        changed, therefore the function call is const.
    */

    virtual void operator+(const iWord&) = 0;
    virtual Register operator+(const iRegister&) const = 0;
    /*  The addition operator contains the same functionality as Add()
        and therefore is merely an alias.
    */

    virtual void Subtract(const iWord&) = 0;
    virtual Register Subtract(const iRegister&) const = 0;
    /*  Subtract(iRegister) mathematically subtracts the value of the given
        parameter's value from the current value of this iRegister and
        returns the result. Neither the parameter nor the current instance
        of iRegister is changed, therefore the function call is const.
    */

    virtual void operator-(const iWord&) = 0;
    virtual Register operator-(const iRegister&) const = 0;
    /*  The subtraction operator contains the same functionality as
        Subtract() and therefore is merely an alias.
    */

    virtual void And(const iWord&) = 0;
    virtual Register And(const iRegister&) const = 0;
    /*  And(iRegister) logically ands the value of the given parameter's
        value from the current value of this iRegister and returns the
        result. Neither the parameter nor the current instance of iRegister
        is changed, therefore the function call is const.
    */

    virtual void Or(const iWord&) = 0;
    virtual Register Or(const iRegister&) const = 0;
    /*  Or(iRegister) logically or's the value of the given parameter's value
        from the current value of this iRegister and returns the result.
        Neither the parameter nor the current instance of iRegister is
        changed, therefore the function call is const.
    */

    virtual void Not() = 0;
    virtual Register Not() const = 0;
    /*  Not() logically inverts the value of the current value of this
        iRegister and returns the result. The current instance of iRegister
        is unchanged, therefore the function call is const.
    */

    virtual void Store(const iWord&) = 0;
    virtual void Store(const iRegister&) = 0;
    virtual void operator=(const iWord&) = 0;
    virtual void operator=(const iRegister&) = 0;

    virtual Register& operator++() = 0;
    virtual Register& operator++(int) = 0;
};

#endif
