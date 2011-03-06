/*! @file iFileArray.h
    @author Andrew Groot
    @brief Definition of a file-handling system for the Wi-11 linker.
*/

#ifndef iFILEARRAY_H
#define IFILEARRAY_H

#include "ObjParser.h"
#include <string>

class iFileArray {
  private:

  public:
    virtual bool Add(std::string name) = 0;

    virtual void Restart() = 0;

    virtual ObjParser& operator[](int index) = 0;

    virtual int Size() const = 0;

    virtual void Clear() = 0;
}

#endif
