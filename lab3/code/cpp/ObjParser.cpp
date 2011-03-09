/*! @file ObjParser.cpp
    @author Ryan Paulson
    @author Joshua Green
    @author Andrew Groot
    @brief Implements the declarations in "../h/ObjParser.h".
*/

#include "../h/ObjParser.h"
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

  string line;

  //  Checks if the file is ready to be read and reads the line if ready or returns a 'dummy' ObjectData struct if not.
  if (!(getline(_fileStream, line))) {
    _object.type = 0;
    _object.data.push_back(string());
    return _object;
  }

  //  Determines what type of line has been read, then parses the line accordingly.
  switch (line[0]) {

    // Header line
    case 'M':
    case 'H':
      //                    && (Relocatable case)
      if (line.size() != 15 && line.size() != 11) { // improper header entry length
        _object.type = 0;
        _object.data.push_back(string());
        return _object;
      }

      _object.type = line[0];

      if (line.size() == 11) {
        _object.data.push_back(line.substr(1,6));
        _object.data.push_back(string(RELOCATE_FLAG));       // Relocatable
        _object.data.push_back(line.substr(7,4));        
      }
      else {
        _object.data.push_back(line.substr(1,6));
        _object.data.push_back(line.substr(7,4));
        _object.data.push_back(line.substr(11,4));
      }
      break;

    case 'N':
      if (line.size() < 6) { // NADDR (no symbol)
        _object.type = 0;
        _object.data.push_back(string());
        return _object;
      }

      // N
      _object.type = line[0];
      // symbol    N|FOO|XXXX  size = 8, -1 for N, -4 for hex; = 3.
      _object.data.push_back(line.substr(1, line.size() - 5));
      // addr      N|FOO|XXXX  size = 8, -4 for addr, 4, 4; = XXXX
      _object.data.push_back(line.substr(line.size()-4, 4));
      break;

    // Text line
    case 'X':
    case 'x':
    case 'R':
    case 'W':
    case 'T':
      if (line.size() < 9) { // improper text entry length
        _object.type = 0;
        _object.data.push_back(string());
        return _object;
      }

      _object.type = line[0];
      _object.data.push_back(line.substr(1,4));
      _object.data.push_back(line.substr(5,4));

      if (line[0] == 'x' || line[0] == 'X') {
        // Get the next line
        if (!(getline(_fileStream, line))) {
          _object.type = 0;
          _object.data.clear();
          _object.data.push_back(string());
          return _object;
        }
        // add symbol name
        _object.data.push_back(line);
      }
      break;

    // End line
    case 'E':
      if (line.size() < 5) { // improper end entry length
        _object.type = 0;
        _object.data.push_back(string());
        return _object;
      }

      _object.type = 'E';
      _object.data.push_back(line.substr(1,4));
      break;

    // Incorrect line
    default:
      _object.type = 0;
      _object.data.push_back(line.substr(1));
  }

  return _object;
}

