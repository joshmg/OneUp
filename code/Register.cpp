// File: Register.cpp
// Written by:
//    Andrew Groot

#include "Register.h"

// public
Register::Register () {
  _word.fromInt(0);
}

Register::Register (Word w) {
  _word = w;
}

Word Register::getValue() const {
  Word temp(_word);
  return temp;
}

void Register::Add(const iWord& w) {
  _word = _word + w;
}

Register Register::Add(const iRegister& r) const {
  Register temp;
  temp._word = (*this)._word + r.getValue();
  return temp;
}

void Register::operator+(const iWord& w) {
  _word = _word + w;
}

Register Register::operator+(const iRegister& r) const {
  Register temp;
  temp._word = (*this)._word + r.getValue();
  return temp;
}

void Register::Subtract(const iWord& w) {
  _word = _word - w;
}

Register Register::Subtract(const iRegister& r) const {
  Register temp;
  temp._word = (*this)._word - r.getValue();
  return temp;
}

void Register::operator-(const iWord& w) {
  _word = _word - w;
}

Register Register::operator-(const iRegister& r) const {
  return (*this)._word - r.getValue();
}

void Register::And(const iWord& w) {
  _word = _word.And(w);
}

Register Register::And(const iRegister& r) const {
  Register temp;
  temp._word = (*this)._word.And(r.getValue());
  return temp;
}

void Register::Or(const iWord& w) {
  _word = _word.Or(w);
}

Register Register::Or(const iRegister& r) const {
  Register temp;
  temp._word = (*this)._word.Or(r.getValue());
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
  if (this != &w) {
    _word = w;
  }
}

void Register::Store(const iRegister& r) {
  if (this != &w) {
    _word.copy(r.getValue());
  }
}

void Register::operator=(const iWord& w) {
  _word = w;
}

void Register::operator=(const iRegister& r) {
  _word = r.getValue();
}

Register& Register::operator++() {
  _word++;
  return (*this);
}

Register& Register::operator++(int) {
  _word++;
  return (*this);
}

