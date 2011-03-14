/*! @file iFileArray.h
    @author Andrew Groot
    @brief Definition of a file-handling system for the Wi-11 linker.
*/

#ifndef iFILEARRAY_H
#define IFILEARRAY_H

#include "ObjParser.h"
#include "ResultCodes.h"
#include <string>

/*! @brief Keeps an array of files in with a simple ordering property to aid the linking process.

    Using this, the linker doesn't have to worry about checking for multiple main files,
    nor does it have to bother opening the files or separating the lines into their parts
    because of the ObjectData objects returned by ObjParser's GetNext() method.
*/
class iFileArray {
  private:

  public:
    /*! @brief Adds a file to the array.
        @param[in] name The name of the file to add.
        @return SUCCESS or, if something went wrong, an appropriate error code.

        Normally, the file is opened and added to the back of the array.
        If the file has a "main" entry, it is put at the beginning of the array.
        If there is already another main file at the front, an error is returned.
    */
    virtual Codes::RESULT Add(std::string name) = 0;

    /*! @brief Sets all of the files in the array back to the beginning of input.
    */
    virtual void Reset() = 0;

    /*! @brief Returns the name of a file in the array.
        @param[in] index The index of the desired file.
        @return The name of the file.
    */
    virtual std::string Name(int index) const = 0;

    /*! @brief Access a particular file.
        @param[in] index The index of the desired file.
        @return A reference to the ObjParser object that opened the file.
    */
    virtual ObjParser& operator[](int index) = 0;

    /*! @return The number of files in the array.
    */
    virtual int Size() const = 0;

    /*! @breif Closes and removes all the file in the array.
    */
    virtual void Clear() = 0;
};

#endif
