// File: Word.h
// Written by:
//    Joshua Green
// Edited by:
//    Andrew Groot

#ifndef WORD_H
#define WORD_H

#include "iWord.h"
#include <string>

#define WORD_SIZE 16

class Word : public iWord {
  private:
    unsigned short _value;
    bool _hasBit(int) const;

  public:
    Word();

    int toInt() const;
    int toInt2Complement() const;
    std::string toStr() const;
    std::string toHex() const;

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

    void copy(const iWord&);
    iWord& operator=(const iWord&);

    iWord& operator++();
    iWord& operator++(int);

    bool operator[](const int) const;

    void print() const;
};

#endif

