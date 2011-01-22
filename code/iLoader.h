/*! @file iLoader.h
    @author Joshua Green
    @author Andrew Groot
    @brief Definition of the Wi-11 program loader.
*/

#ifndef iLOADER_H
#define iLOADER_H

#include "ResultCodes.h"
#include "iMemory.h"

/*! @brief Initializes memory.
    
    This class loads the instruction from the object file into memory.
*/
class iLoader {
  private:
  
  public:
    /*! @brief Set which Memory object is to be initialized by this object.
        @param[in] mem The address where memory is located.
        
        @note
        Without this there would be nowhere to load the instructions.
    */
    virtual iLoader(iMemory* mem) = 0;


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
