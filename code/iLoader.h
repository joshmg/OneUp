// File: iLoader.h
// Written by Joshua Green

#ifndef iLOADER_H
#define iLOADER_H

#include "ResultCodes.h"
#include "iMemory.h"

class iLoader {
  private:
  
  public:
    virtual iLoader(iMemory*) = 0;
    /*  Instantiation of a child of iLoader requires a pointer to the
        memory object the iLoader will be store the object file's data.
    */

    virtual Codes::RESULT Load(const char* filename, iWord& PC_address) = 0;
    /*  Requires the filename of the object file. Returns the respective
        result code upon success/failure. This function can be invoked
        multiple times if loading multiple obj files. Load() opens the
        filename, and checks for success. If the file is opened successfully
        it begins parsing the file, ensuring the format of the *.obj is
        valid. iLoader should utilize iObjParser but is not required.
        Load() obtains the first line, ensuring it is a proper header;
        upon success it reserves the requested space within its pointer to
        iMemory. Load() then obtains every Text entry, ensuring validity,
        then storing its value at its respective address. Once an End entry
        is reached, it is checked for validity, and then PC_address is set
        to that value.
    */
};

#endif
