// File: iWord.h
// Written by:
//    Joshua Green

#ifndef iWORD_H
#define iWORD_H

#include <string>

class iWord {
  private:
    
  public:
    virtual std::int toInt() const = 0;
    virtual std::string toStr() const = 0;
    virtual std::string toHex() const = 0;

    virtual void fromInt(const int) = 0;
    virtual void fromStr(const std::string&) = 0;
    virtual void fromHex(const std::string&) = 0;

    virtual iWord Add(const iWord&) const = 0;
    virtual iWord operator+(const iWord&) const = 0;
    
    virtual iWord Subtract(const iWord&) const = 0;
    virtual iWord operator-(const iWord&) const = 0;

    virtual iWord And(const iWord&) const = 0;
    virtual iWord Or(const iWord&) const = 0;
    virtual iWord Not(const iWord&) const = 0;
    

    virtual iWord operator++() const = 0;
    virtual iWord operator++(int) const = 0;
};

#endif
