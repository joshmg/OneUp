// File: Register.h
// Written by:
//    Andrew Groot

#ifndef REGISTER_H
#define REGISTER_H

#include "iRegister.h"
#include "Word.h"

class Register {
  private:
    Word _word;

  public:
    Register();
    Register(const Word w);

    Word GetValue() const;

    void Add(const iWord&);
    Register Add(const iRegister&) const;
    void operator+(const iWord&);
    Register operator+(const iRegister&) const;

    void Subtract(const iWord&);
    Register Subtract(const iRegister&) const;
    void operator-(const iWord&);
    Register operator-(const iRegister&) const;

    void And(const iWord&);
    Register And(const iRegister&) const;
    void Or(const iWord&);
    Register Or(const iRegister&) const;
    void Not();
    Register Not() const;

    void Store(const iWord&);
    void Store(const iRegister&);
    Register& operator=(const iWord&);
    Register& operator=(const Register);

    Register& operator++();
    Register& operator++(int);
};

#endif
