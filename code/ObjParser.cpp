/*! @file ObjParser.cpp
    @author Ryan Paulson
    @brief Implements the declarations in "ObjParser.h".
*/

#include "ObjParser.h"
#include <string>
#include <fstream> // Necessary for reading a file.
#include <iostream>
using namespace std;

//*** public ***//
//  Closes _fileStream if it is currently open.
ObjParser::~ObjParser() {
  if (_fileStream.is_open()) {
    _fileStream.close();
  }
}

/*! @brief Closes _fileStream if necessary, then opens the file defined by "name".
    @param name The name of the file to be opened, including extension.
    @return Codes::SUCCESS if the file is successfully opened, Codes::FILE_NOT_FOUND otherwise.
*/
Codes::RESULT ObjParser::Initialize(const char* name) {
  //  Closes _fileStream if it is currently open.
  if (_fileStream.is_open()) {
    _fileStream.close();
  }

  //  Opens the file defined by "name".
  _fileStream.open(name, ifstream::in);

  //  Determine whether the file was successfully opened and return the appropriate result code.
  if (_fileStream.is_open()) {
    return Codes::SUCCESS;
  }
  else {
    return Codes::FILE_NOT_FOUND;
  }
}

/*! @brief Reads the next line from the current object file and parses it into an ObjectData struct for use by the loader.
    @pre Initialize(name) has been called and _fileStream is currently open.
    @post The get pointer within _fileStream has been advanced to the next line.
    @return A well-formed ObjectData struct if a valid line is received, a 'dummy' ObjectData struct otherwise.
*/
ObjectData ObjParser::GetNext() {
  ObjectData _object;

  string _line;

  //  Checks if the file is ready to be read and reads the line if ready or returns a 'dummy' ObjectData struct if not.
  if (_fileStream.good()) {
    getline(_fileStream, _line);
  }
  else {
    _object.type = 0;
    _object.data.push_back(string());
    return _object;
  }

  //  Determines what type of line has been read, then parses the line accordingly.
  switch (_line[0]) {

    // Header line
    case 'H':
      if (_line.size() < 15) { // improper header entry length
        _object.type = 0;
        _object.data.push_back(string());
        return _object;
      }

      _object.type = 'H';
      _object.data.push_back(_line.substr(1,6));
      _object.data.push_back(_line.substr(7,4));
      _object.data.push_back(_line.substr(11,4));
      break;

    // Text line
    case 'T':
      if (_line.size() < 9) { // improper text entry length
        _object.type = 0;
        _object.data.push_back(string());
        return _object;
      }

      _object.type = 'T';
      _object.data.push_back(_line.substr(1,4));
      _object.data.push_back(_line.substr(5,4));
      break;

    // End line
    case 'E':
      if (_line.size() < 5) { // improper end entry length
        _object.type = 0;
        _object.data.push_back(string());
        return _object;
      }

      _object.type = 'E';
      _object.data.push_back(_line.substr(1,4));
      break;

    // Incorrect line
    default:
      _object.type = 0;
      _object.data.push_back(_line.substr(1));
  }

  return _object;
}