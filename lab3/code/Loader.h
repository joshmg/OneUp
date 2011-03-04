/*! @file Loader.h
    @author Logan Coulson
    @author Joshua Green
    @author Andrew Groot
    @brief Definition of the private data for the "Loader" class.
*/

#ifndef LOADER_H
#define LOADER_H

#include "iLoader.h"
#include "iMemory.h"
#include "ResultCodes.h"

//! Implements iLoader.
class Loader : public iLoader {
  private:
    //! The reference to Memory.
    iMemory* _memory;
    
  public:
    /*! @brief Set which Memory object is to be initialized by this object.
        @param[in] mem The address where memory is located.

        @note
        Without this there would be nowhere to load the instructions.
    */
    Loader(iMemory* mem);

    Codes::RESULT Load(const char* filename, iWord& PC_address) const;
};

#endif
