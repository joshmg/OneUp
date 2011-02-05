#include "Line.h"
#include "iWord.h"
#include "Word.h"
#include "ResultCodes.h"
#include <string>
using namespace std;

bool isWS(char ch) {
  return (ch == ' ' || ch == '\t' || ch == '\n');
}

string Line::_GetNext(string& str) {
  while ( isWS(str[0]) ) {
    str = str.substr(1);
  }

  string next = "";
  while ( !isWS(str[0]) ) {
    next += str[0];
    str = str.substr(1);
  }
  
  return next;
}

Codes::RESULT Line::ReadLine (string line) {
  _code = line;

  if ( !isWS(line[0]) ) {
    _label = _GetNext(line);
  } 

  _inst = _GetNext(line);

  string args = _GetNext(line);
  while (args.length() > 0) {
    string arg;
    while (args[0] != ',' && args.length > 0) {
      arg += args[0];
      args.substr(1);
    }
    _args.push_back(arg);
  }

  // argument types
  Word w;
  if (_inst == "ADD" || _inst == "AND") {
    // OP CODE
    if (_inst == "ADD") {
      w.Or(Word(1*pow(2.0, 11)));
    } else {
      // AND
      w.Or(3*pow(2.0, 11));
    }

    // Arg 1
    w.Or(_args[0][1]*pow(2.0, 8));
    // Arg 2
    w.Or(_args[1][1]*pow(2.0, 5));

    // immediate bit
    if (_args[2][0] == 'R') {
      w.SetBit(5, true);
      if ()
    } else {
      if (arg[2] < 16 && arg[2] > -17) {
        w.Or()
  }

  return SUCCESS;
}

string Line::Label() const {
  return _label;
}

string Line::Instruction() const {
  return _inst;
}

string Line::operator[] (int index) const {
  return _args[index];
}

int Line::Size() const {
  return _args.size();
}

string Line::ToString() const {
  return _code;
}

iWord& ToWord() const {
  return _word;
}

