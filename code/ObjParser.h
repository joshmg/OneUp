/*! @file ObjParser.h
    @author Ryan Paulson
    @brief Definition of private data for the "ObjParser" class.
*/

#ifndef OBJPARSER_H
#define OBJPARSER_H

#include "iObjParser.h"
#include "Word.h"
#include <ifstream>

class ObjParser : iObjParser {
  private:
    //! Maintains an input stream from the object file specified by the "name" parameter to Initialize.
    ifstream _fileStream;

  public:
    ObjParser();

    ~ObjParser();

    Codes::Result Initialize(const char* name);

    ObjectData GetNext();
};

#endif
