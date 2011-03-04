/*! @file ObjParser.h
    @author Ryan Paulson
    @brief Definition of private data for the "ObjParser" class.
*/

#ifndef OBJPARSER_H
#define OBJPARSER_H

#include "iObjParser.h"
#include "Word.h"
#include <fstream>

//! Implements iObjParser.
class ObjParser : public iObjParser {
  private:
    //! Maintains an input stream from the object file specified by the "name" parameter to Initialize.
    std::ifstream _fileStream;

  public:
    //! Closes a file, if necessary, when an iObjParser object goes out of scope.
    ~ObjParser();

    Codes::RESULT Initialize(const char* name);

    ObjectData GetNext();
};

#endif
