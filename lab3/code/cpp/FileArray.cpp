// File:        FileArray.cpp
// Written by:  Andrew Groot

#include "../h/FileArray.h"
#include "../h/ResultCodes.h"
#include "../h/ObjParser.h"
#include <string>
#include <vector>
using namespace std;
using namespace Codes;

FileArray::FileArray() {
  _hasMain = false;
}

RESULT FileArray::Add(string name) {
  // create a pointer to a file
  ObjParser parser;

  // open file
  RESULT result = parser.Initialize(name.c_str());
  if (result != SUCCESS) {
    return RESULT(result.msg, name);
  }

  // check first char for main function
  ObjectData header = parser.GetNext();
  if ( header.type == 'M') {
    if (_hasMain) {
      return RESULT(MULTI_MAIN, name);
    }
    _hasMain = true;
    // first main file - put at beginning
    _files.insert(_files.begin(), parser);
    _names.insert(_names.begin(), name);
    return RESULT(SUCCESS);
  }
  // header file
  _files.push_back(parser);
  _names.push_back(name);
  return RESULT(SUCCESS);
}


void FileArray::Reset() {
  for (int i = 0; i < _files.size(); i++) {
    _files[i].Initialize(_names[i].c_str());
  }
}

string FileArray::Name(int index) const {
  return _names[index];
}

ObjParser& FileArray::operator[](int index) {
  return _files[index];
}

int FileArray::Size() const {
  return _files.size();
}

void FileArray::Clear() {
  _files.clear();
  _names.clear();
}

