// File: Word.h
// Written by:
//    Joshua Green
// Edited by:
//    Andrew Groot

#ifndef WORD_H
#define WORD_H

#include "iWord.h"
#include <string>
#include <iostream> // for print()
#include <cmath> // for pow()
using namespace std;

#define WORD_SIZE 16

class Word : public iWord {
  private:
    unsigned short _value;
    bool hasBit(int) const;

  public:
    Word();

    int toInt() const;
    int toInt2Complement() const;
    string toStr() const;
    string toHex() const;

    bool fromInt(int);
    bool fromStr(const std::string&);
    bool fromHex(const std::string&);

    Word Add(const iWord&) const;
    Word operator+(const iWord&) const;
    
    Word Subtract(const iWord&) const;
    Word operator-(const iWord&) const;

    Word And(const iWord&) const;
    Word Or(const iWord&) const;
    Word Not() const;
    
    iWord& operator++();
    iWord& operator++(int);
    bool operator[](int);

    void print() const;
};

#endif
