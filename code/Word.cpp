/*! @file Word.cpp
    @author Joshua Green
    @author Andrew Groot
    @brief Implements the delcarations in "Word.h".
*/

#include "Word.h"
#include <string>
#include <cmath> // for pow
using namespace std;

//*** private ***//
bool Word::_HasBit(int i) const {
  // bit i's value
  int pow_of_two = (int)pow(2.0, i);
  // true iff ith bit is one
  return ((_value & pow_of_two) > 0);
}

//*** public ***//
Word::Word() {
  _value = 0;
}

int Word::ToInt() const {
  // always positive
  return (int) _value;
}

int Word::ToInt2Complement() const {
  // create logical overflow to allow for neg numbers
  short temp = (short) _value;
  return temp;
}

string Word::ToStr() const {
  string str;
  for (int i=0;i<WORD_SIZE;i++) {
    // start with highest order bit
    str += '0' + _HasBit((WORD_SIZE-1)-i);
  }
  return str;
}

string Word::ToHex() const {
  string value("");

  for (int i=0;i<WORD_SIZE;i+=4) {
    // bits i to i+3, shifted down i bits
    int hex_bit = (_value & 0x000F * (int)pow(2.0, i)) / (int)pow(2.0, i);
    string temp = value;
    if (hex_bit < 10) {
       value = '0' + hex_bit;
    } else {
      // 'A' = 10, so offset hex_bit by 10
      value = 'A' + (hex_bit - 10);
    }
    value += temp;
  }
  return "0x" + value;  
}

bool Word::FromInt(int value) {
  if (((unsigned short) value) < (int) pow(2.0, WORD_SIZE)) {
    // value is within allowable range
    _value = (short) value;
    return true;
  }
  // value is greater than the range of the architecture
  return false;
}

bool Word::FromStr(const string& value) {
  if (value.length() != WORD_SIZE) {
    // wrong size
    return false;
  }
  for (int i=0; i<WORD_SIZE; i++) {
    if (value[i] != '1' && value[i] != '0') {
      // not binary
      return false;
    }
  }
  
  _value = 0;
  int pow_of_two = 1;
  for (int i=0;i<WORD_SIZE;i++) {
    // accumulate the value of the ith character
    // times the value of it's binary "decimal place." 
    _value += pow_of_two * (value[(WORD_SIZE-1)-i] - '0');
    pow_of_two *= 2;
  }

  return true;
}

bool Word::FromHex(const string& value) {
  string proper = "0xXXXX"; // proper formatting
  if (value.length() != proper.length()) {
    // wrong size
    return false;
  }
  if (value[0] != '0' || value[1] != 'x') {
    // no '0x' prefix
    return false;
  }
  for (int i=2; i<proper.length(); i++) {
    if ( !((value[i] >= '0' && value[i] <= '9') || (value[i] >= 'A' && value[i] <= 'F')) ) {
      // not hex
      return false;
    }
  }

  _value = 0;
  for (int i=2; i<value.length(); i++) {
    int hex_val;
    if (value[i] <= '9') {
      // decimal digit
      hex_val = value[i] - '0';
    } else {
      // capital letter, A = 10
      hex_val = value[i] - 'A' + 10;
    }
    // shift up 4 bits per remaining hex digit
    _value += hex_val * (int)pow(2.0, WORD_SIZE-4*((i+1)-2));
    // (i+1)-2 comes from:
    //  the string accessor starting at zero (+1) and
    //  the index starting at 2 (-2)
  }

  return true;
}

Word Word::Add(const iWord& x) const {
  Word temp;
  temp._value = (*this)._value + x.ToInt();
  return temp;
}

Word Word::operator+(const iWord& x) const {
  Word temp;
  temp._value = (*this)._value + x.ToInt();
  return temp;
}

Word Word::Subtract(const iWord& x) const {
  Word temp;
  temp._value = (*this)._value - x.ToInt();
  return temp;
}

Word Word::operator-(const iWord& x) const {
  Word temp;
  temp._value = (*this)._value - x.ToInt();
  return temp;
}

Word Word::And(const iWord& x) const {
  Word temp;
  temp._value = (*this)._value & x.ToInt();
  return temp;
}

Word Word::Or(const iWord& x) const {
  Word temp;
  temp._value = (*this)._value | x.ToInt();
  return temp;
}

Word Word::Not() const {
  Word temp;
  temp._value = ~(*this)._value;
  return temp;
}

void Word::Copy(const iWord& w) {
  if (this != &w) {
    _value = (short) w.ToInt();
  }
}

Word& Word::operator=(const Word& w) {
  if (this != &w) {
    _value = (short) w.ToInt();
  }
  return (*this);
}

iWord& Word::operator++() {
  _value++;
  return (*this);
}

iWord& Word::operator++(int) {
  _value++;
  return (*this);
}

bool Word::operator[](const int i) const {
  return _HasBit(i);
}

void Word::SetBit(const int i, bool on) {
  if (on) _value |= (int)(pow(2.0f, (float)i)); // logical OR
  else _value &= ~((int)(pow(2.0f, (float)i))); // logical And with inverted integer
}

