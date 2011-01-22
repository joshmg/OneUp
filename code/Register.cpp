// File: Register.cpp
// Written by:
//    Andrew Groot

#include "Register.h"

//*** public ***//
Register::Register () {
  _word.FromInt(0);
}

Register::Register (const iWord& w) {
  _word.Copy(w);
}

Word Register::GetValue() const {
  Word temp = _word;
  return temp;
}

void Register::Add(const iWord& w) {
  _word = _word + w;
}

Register Register::Add(const iRegister& r) const {
  Register temp;
  temp._word = (*this)._word + r.GetValue();
  return temp;
}

Register Register::operator+(const iRegister& r) const {
  Register temp;
  temp._word = (*this)._word + r.GetValue();
  return temp;
}

void Register::Subtract(const iWord& w) {
  _word = _word - w;
}

Register Register::Subtract(const iRegister& r) const {
  Register temp;
  temp._word = (*this)._word - r.GetValue();
  return temp;
}

Register Register::operator-(const iRegister& r) const {
  return (*this)._word - r.GetValue();
}

void Register::And(const iWord& w) {
  _word = _word.And(w);
}

Register Register::And(const iRegister& r) const {
  Register temp;
  temp._word = (*this)._word.And(r.GetValue());
  return temp;
}

void Register::Or(const iWord& w) {
  _word = _word.Or(w);
}

Register Register::Or(const iRegister& r) const {
  Register temp;
  temp._word = (*this)._word.Or(r.GetValue());
  return temp;
}

void Register::Not() {
  _word = _word.Not();
}

Register Register::Not() const {
  Register temp;
  temp._word = (*this)._word.Not();
  return temp;
}

void Register::Store(const iWord& w) {
  _word.Copy(w);
}

void Register::Store(const iRegister& r) {
  if ((iRegister*)this != &r) {
    _word.Copy(r.GetValue());
  }
}

Register& Register::operator=(const iWord& w) {
  _word.Copy(w);
  return (*this);
}

Register& Register::operator=(const Register r) {
  if (this != &r) {
    _word.Copy(r.GetValue());
  }
  return (*this);
}

Register& Register::operator++() {
  _word++;
  return (*this);
}

Register& Register::operator++(int) {
  _word++;
  return (*this);
}

