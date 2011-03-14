/*! @file iLoader.h
    @author Joshua Green
    @author Andrew Groot
    @brief Definition of the Wi-11 program loader.
*/

#ifndef iLOADER_H
#define iLOADER_H

#include "ResultCodes.h"
#include "iMemory.h"

/*! @brief Defines how the Wi-11 initializes memory.
    
    This class loads the instruction from the object file into memory.
*/

class iLoader {
  private:
    virtual Codes::RESULT _GetLoadAddress(Word& produced_addr, const Word& segment_length) const = 0;

  public:
    /*! @brief Perform the loads to memory (storing the instructions).
        @param[in] filename The name of the object file to be read.
        @param[out] PC_address The value to be stored in the PC to start execution.
        @reutrn SUCCESS or, if something goes wrong, an appropriate error code.

        @note
        Multiple object files can be loaded using this, but the PC will
        be overwritten every time, so only the last End Record will matter
        (HOWEVER: the End Records still need to be present in each file).
    */
    virtual Codes::RESULT Load(const char* filename, iWord& PC_address) const = 0;
};

#endif
