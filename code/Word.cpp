// File: Word.cpp
// Written by:
//    Joshua Green

#include "Word.h"
#include <string>
#include <iostream> // for print()
using namespace std;

Word::Word() {
  for (int i=0;i<WORD_SIZE;i++) { _data[i] = 0; }
}

int Word::toInt() const {
  int value = 0, power_of_2 = 1;
  for (int i=0;i<WORD_SIZE;i++) {
    value += power_of_2 * (int)_data[i];
    power_of_2 *= 2;
  }

  return value;
}

int Word::toInt2Complement() const {
  Word value = this->Not();
  ++value;
  return value;
}

string Word::toStr() const {
  string value;

  for (int i=0;i<WORD_SIZE;i++) {
    value += '0' + (int)_data[i];
  }

  return value;
}

string Word::toHex() const {
  string value("0x");

  for (int i=0;i<WORD_SIZE;i+=4) {
    int hex_bit = _data[(i)] + _data[(i+1)]*2 + _data[(i+2)]*4 + _data[(i+3)]*8;
    if (hex_bit < 10) value += '0' + hex_bit;
    else value += 'A' + hex_bit;
  }

  return value;  
}

bool Word::fromInt(int value) {
  // handle negative -> Two's Complement
  if (value < 0) {
    Word negative;
    bool success = negative.fromInt(-value);
    negative = negative.Not();
    ++negative;
    (*this) = negative;
    return success;
  }

  // value is non-negative:
  int power_of_2 = (int)pow((double)2.0, WORD_SIZE-1);

  for (int i=0;i<WORD_SIZE;i++) {
    if (value < 1) {
      _data[i] = 0;
      continue;
    }

    if (value/power_of_2 > 0) {
      _data[i] = 1;
      value -= power_of_2;
    }

    power_of_2 /= 2.0;
  }

  return true;
}

bool Word::fromStr(const string& value) {
  bool correctly_formatted = true;

  string reversed_value;
  for (int i=value.length()-1;i>=0;i--) {
    if (value[i] == ' ' || value[i] == '\n' || value[i] == '\r' || value[i] == '\t') { // leading or trailing whitespace does not produce an invalid formatted warning
      if (reversed_value.length() > 0) break;
      else continue;
    }
    if (value[i] == '1' || value[i] == '0') reversed_value += value[i];
    else {
      correctly_formatted = false;
      break;
    }
  }
  // if (reversed_value.length() < WORD_SIZE) return false; // value does not contain then proper length
  for (int i=0;i<WORD_SIZE;i++) {
    if (i < reversed_value.length()) {
      if (reversed_value[i] != '0' && reversed_value[i] != '1') return false;
      _data[WORD_SIZE-1-i] = ((reversed_value[i]-'0') == 1); // fill in data from right to left to enable data lengths less than WORD_SIZE to be valid
    }
    else _data[WORD_SIZE-1-i] = 0;
  }

  return correctly_formatted;
}

bool Word::fromHex(const string& value) {
  bool correctly_formatted = true;

  bool beginning_whitespace = true;
  string reversed_value;
  if (value.length() > 2 && value[0] == '0' && value[1] == 'x') {
    for (int i=value.length()-1;i>=2;i--) {
      if (beginning_whitespace && (value[i] == ' ' || value[i] == '\n' || value[i] == '\r' || value[i] == '\t')) continue; // ignore leading whitespace
      else {
        beginning_whitespace = false;
        if ((value[i] >= '0' && value[i] <= '9') || (value[i] >= 'A' && value[i] <= 'F')) reversed_value += value[i];
        else {
          correctly_formatted = false;
          break;
        }
      }
    }
  }
  else correctly_formatted = false;

  for (int i=0;i<WORD_SIZE-1;i+=4) {
    if (i/4 < reversed_value.length()) {
      if (!(reversed_value[i/4] >= '0' && reversed_value[i/4] <= '9') && !(reversed_value[i/4] >= 'A' && reversed_value[i/4] <= 'F')) return false;

      int decimal_digit_value;
      if (reversed_value[i/4] <= '9') decimal_digit_value = reversed_value[i/4] - '0';
      else decimal_digit_value = reversed_value[i/4] - 'A' + 10;

      int power_of_2 = 8;
      for (int j=3;j>=0;j--) {
        if (decimal_digit_value/power_of_2 > 0) {
          _data[WORD_SIZE-1-i-j] = 1;
          decimal_digit_value -= power_of_2;
        }
        else _data[WORD_SIZE-1-i-j] = 0;
        power_of_2 /= 2;
      }

    }
    else _data[WORD_SIZE-1-i] = 0;
  }

  return correctly_formatted;
}

Word Word::Add(const iWord& x) const {
  Word value;
  value.fromInt(this->toInt() + x.toInt());
  return value;
}

Word Word::operator+(const iWord& x) const { return this->Add(x); }

Word Word::Subtract(const iWord& x) const {
  Word value;
  value.fromInt(this->toInt() - x.toInt());
  return value;
}
Word Word::operator-(const iWord& x) const { return this->Subtract(x); }

Word Word::And(const iWord& x) const {
  Word value;
  value.fromInt((*this).toInt() & x.toInt());
  return value;
}

Word Word::Or(const iWord& x) const {
  Word value;
  value.fromInt((*this).toInt() | x.toInt());
  return value;
}

Word Word::Not() const {
  Word value;
  for (int i=0;i<WORD_SIZE;i++) {
    value._data[i] = (!_data[i]);
  }
  return value;
}


iWord& Word::operator++() {
  if (_data[WORD_SIZE-1] == 0) _data[WORD_SIZE-1] = 1;
  else {
    int index = WORD_SIZE-1;
    while (_data[index] && index < WORD_SIZE) {
      _data[index] = false;
      index--;
    }
  }

  return (*this);
}

iWord& Word::operator++(int) {
  Word value(*this);
  ++(*this);
  return value;
}

void Word::print() const {
  cout << "[";
  for (int i=0;i<WORD_SIZE;i++) {
    cout << _data[i];
  }
  cout << "]";
}
