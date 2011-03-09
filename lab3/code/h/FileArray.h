/*! @file FileArray.h
    @author Andrew Groot
    @brief Definition of the private data for the "FileArray" class.
*/

#ifndef FILEARRAY_H
#define FILEARRAY_H

#include "iFileArray.h"
#include "ObjParser.h"
#include "ResultCodes.h"
#include <fstream>
#include <string>
#include <vector>

//! Implements iFileArray
class FileArray : public iFileArray {
  private:
    //! Keeps all of the files open for the two pass algorithm.
    std::vector<ObjParser*> _files;
    //! Keeps track of the names of the files.
    std::vector<std::string> _names;
    //! True iff a main file has already been found.
    bool _hasMain;

  public:
    //! Sets the _hasMain variable to false.
    FileArray();

    Codes::RESULT Add(std::string name);
    void Reset();
    std::string Name(int index) const;
    ObjParser& operator[](int index);
    int Size() const;
    void Clear();
};

#endif

