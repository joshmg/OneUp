/*! @file: Word.h
    @author Joshua Green
    @author Andrew Groot
    @brief Definition of private data for the "Word" class.
*/

#ifndef WORD_H
#define WORD_H

#include "iWord.h"
#include <string>

#define WORD_SIZE 16

class Word : public iWord {
  private:
    /*! @brief Used to store the "word" of data.
        
        The type "unsigned short" was chosen because in c++, shorts are 16bits (the same size as our words)
        and having it unsigned allows for easy "reading" as a positive int or a 2's complement int.
    */
    unsigned short _value;

    /*! @brief Tests for powers of two in binary representation.
        @param i The index of the digit desired from the binary representation of _word.
        @return True if and only if the 'i'th bit is 1.
        
        The indexing of the bits works as defined in #operator[]().
    */
    bool _HasBit(int) const;

  public:
    Word();

    int ToInt() const;
    int ToInt2Complement() const;
    std::string ToStr() const;
    std::string ToHex() const;

    bool FromInt(int value);
    bool FromStr(const std::string& str);
    bool FromHex(const std::string& str);

    Word Add(const iWord& w) const;
    Word operator+(const iWord& w) const;
    
    Word Subtract(const iWord& w) const;
    Word operator-(const iWord& w) const;

    Word And(const iWord& w) const;
    Word Or(const iWord& w) const;
    Word Not() const;

    void Copy(const iWord& w);
    Word& operator=(const Word w);

    iWord& operator++();
    iWord& operator++(int);

    bool operator[](const int i) const;
};

#endif

