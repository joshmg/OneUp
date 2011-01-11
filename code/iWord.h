// File: iWord.h
// Written by:
//    Joshua Green

#ifndef iWORD_H
#define iWORD_H

#include <string>

class Word;

class iWord {
  private:
    
  public:
    virtual int toInt() const = 0;
    virtual std::string toStr() const = 0;
    virtual std::string toHex() const = 0;

    virtual bool fromInt(int) = 0;
    // returns false if an error occured

    virtual bool fromStr(const std::string&) = 0;
    // returns false if an error occured

    virtual bool fromHex(const std::string&) = 0;
    // returns false if an error occured

    virtual Word Add(const iWord&) const = 0;
    virtual Word operator+(const iWord&) const = 0;
    
    virtual Word Subtract(const iWord&) const = 0;
    virtual Word operator-(const iWord&) const = 0;

    virtual Word And(const iWord&) const = 0;
    virtual Word Or(const iWord&) const = 0;
    virtual Word Not() const = 0;

    virtual iWord& operator++() = 0;
    virtual iWord& operator++(int) = 0;
};

#endif
