// File: iWord.h
// Written by:
//    Joshua Green

#ifndef iWORD_H
#define iWORD_H

#include <string>

class Word;

class iWord {
    /*  iWord is a data container designed to be representative of the data
        length and functionality found within the Wi-11 Machine. Its size
        can be variable but in the case of this architecture it is 16 bits
        in length.
    */
  private:
    
  public:
    virtual int toInt() const = 0;
    /*  toInt() ("to integer") returns a positive integer value which is 
        representative of the data iWord is maintaining as a data
        structure. A call to this function does not modify the instance
        of iWord and is therefore const.
    */
    virtual int toInt2Complement() const = 0;
    /*  toInt() ("to integer") returns an integer value which is 
        representative of the data iWord is maintaining as a data
        structure as interpretted by two's complement. A call to this
        function does not modify the instance of iWord and is therefore
        const.
    */
    virtual std::string toStr() const = 0;
    /*  toStr() ("to string") returns a string representation of the data
        iWord is maintaing as a data structure. The string value is a
        series of 1 and 0 characters which directly represent the binary
        value contained within iWord. The string representation does not
        have any prefix prefacing the series of 1 and 0 characters. A call
        to this function does not modify the instance of iWord and is 
        therefore const.
    */
    virtual std::string toHex() const = 0;
    /*  toHex() ("to hex") returns a string representation of the data
        iWord is maintaing as a data structure. The string value is a
        series of 0 through 9, A through F characters which directly 
        represent the hexidecimal value contained within iWord. The string
        representation is prefixed with the characters, "0x". A call
        to this function does not modify the instance of iWord and is 
        therefore const.
    */

    virtual bool fromInt(int) = 0;
    /*  fromInt(int) ("from integer") produces a new value to be represented
        by the instance of iWord. The integer parameter should be capable of
        being represented by 16 bits or as modelled architecture demands. A
        value of true indicates the procedure was performed without error;
        a value of false indicates an error has occured, such as attempting
        to represent a value outside the bounds of the architecture.
    */

    virtual bool fromStr(const std::string&) = 0;
    /*  fromStr(string) ("from string") produces a new value to be
        represented by the instance of iWord. The string parameter should
        be capable of being represented by 16 bits or as modelled 
        architecture demands. The string parameter is expected to be a
        series of 1 and 0 characters representing a binary value with no
        prefix. A return value of true indicates the procedure was performed
        without error; a value of false indicates an error has occured, such
        as attempting to represent a value outside the bounds of the
        architecture.
    */

    virtual bool fromHex(const std::string&) = 0;
    /*  fromHex(string) ("from Hexidecimal") produces a new value to be
        represented by the instance of iWord. The string parameter should
        be capable of being represented by 16 bits or as modelled 
        architecture demands. The string parameter is expected to be a
        series of 0 through 9, A through F characters representing a
        hexidecimal value with the prefix, "0x". A return value of true
        indicates the procedure was performed without error; a value of
        false indicates an error has occured, such as attempting to
        represent a value outside the bounds of the architecture.
    */

    virtual Word Add(const iWord&) const = 0;
    /*  Add() produces a new instance of Word which is formed to represent
        the value of the current instance of iWord mathematically added to
        the value of the parameter. Both the parameter and the current
        instance of iWord are preserved and are therefore const.
    */

    virtual Word operator+(const iWord&) const = 0;
    /*  The addition operator contains the same functionality of Add
        and is merely an alias.
    */

    virtual Word Subtract(const iWord&) const = 0;
    /*  Subract() produces a new instance of Word which is formed to
        represent the value of the iWord parameter mathematically
        subtracted from the value of the current instance. Both the
        parameter and the current instance of iWord are preserved and
        are therefore const.
    */

    virtual Word operator-(const iWord&) const = 0;
    /*  The subtraction operator contains the same functionality of
        Subtract and is merely an alias.
    */

    virtual Word And(const iWord&) const = 0;
    /*  And() produces a new instance of Word which is formed to
        represent the value of the iWord parameter logically/bitwise
        and'ed with the value of the current instance. Both the
        parameter and the current instance of iWord are preserved and
        are therefore const.
    */

    virtual Word Or(const iWord&) const = 0;
    /*  Or() produces a new instance of Word which is formed to
        represent the value of the iWord parameter logically/bitwise
        or'ed with the value of the current instance. Both the
        parameter and the current instance of iWord are preserved and
        are therefore const.
    */

    virtual Word Not() const = 0;
    /*  Not() produces a new instance of Word which is formed to
        represent the value of the current instance logically/bitwise
        inverted. The current instance of iWord is preserved and is
        therefore const.
    */

    virtual iWord& operator++() = 0;
    /*  Pre-increment operator incriments the value of the current instance
        and then returns a reference to itself.
    */

    virtual iWord& operator++(int) = 0;
    /*  Post-increment operator incriments the value of the current instance
        and then returns a duplicate of itself before its incriment.
    */

    virtual bool operator[](int) = 0;
    /*  Accessor to get the value of a certain bit.
        True <=> 1, False <=> 0.
    */
};

#endif
