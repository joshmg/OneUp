// File: Register.h
// Written by:
//    Andrew Groot

#ifndef REGISTER_H
#define REGISTER_H

#include "iRegister.h"
#include "Word.h"

class Register : public iRegister {
  private:
    Word _word;

  public:
    Register();
    Register(const Word w);

    Word GetValue() const;

    void Add(const iWord& w);
    Register Add(const iRegister& r) const;
    Register operator+(const iRegister& r) const;

    void Subtract(const iWord& w);
    Register Subtract(const iRegister& r) const;
    Register operator-(const iRegister& r) const;

    void And(const iWord& w);
    Register And(const iRegister& r) const;
    void Or(const iWord& w);
    Register Or(const iRegister& r) const;
    void Not();
    Register Not() const;

    void Store(const iWord& w);
    void Store(const iRegister& r);
    Register& operator=(const iWord& w);
    Register& operator=(const Register r);

    Register& operator++();
    Register& operator++(int);
};

#endif
