/*! @file iMemory.h
    @author Joshua Green
    @author Andrew Groot
    @brief Definition of Wi-11 memory.
*/

#ifndef iMEMORY_H
#define iMEMORY_H

#include "iWord.h"
#include "ResultCodes.h"
#include <vector>

class Word;

/*! @brief Defines the functionality of memory in the Wi-11 machine.
    
    Its size is limited only by addressability (2^16-1 16-bit words).
    It is meant to be implemented in such a way that the memory
    initialized for instructions can be accessed in constant time
    while addresses outside this range are accessed in nlogn time.
*/
class iMemory {
  private:
    
  public:
    /*! @brief Reserves an initial section of memory for instructions.
        @param[in] initial_address The smallest address for the instruction memory.
        @param[in] length The number of addresses to reserve.
        @return SUCCESS or, if something goes wrong, an appropriate error code.

        The memory reserved here is dynamically allocated and provides
        constant-time access to addresses "initial_address" through
        "initial_address"+"length"-1.
    */
    virtual Codes::RESULT Reserve(const iWord& initial_address, const iWord& length) = 0;


    /*! @brief Performs a load.
        @param[in] w The address from which to load data.
        @return The data stored a address "w".

        @note
        If "w" is in the range created by ::Reserve(), it can be accessed
        in constant time.  Otherwise, a maximum of nlogn time is required
        if n is the size of memory initialized outside of these boundaries.
    */
    virtual Word Load(const iWord& w) const = 0;


    /*! @brief Peforms a store.
        @param[in] address The address to store the data.
        @param[in] value The data to store at "address".
        @return SUCCESS or, if something went wrong, an appropriate error code.

        @note
        The efficiency constraints in ::Load() apply here as well.
    */
    virtual Codes::RESULT Store(const iWord& address, const Word& value) = 0;


    /*! 
    */
    virtual std::vector<std::vector<Word>> GetUsedMemory() const = 0;
};

#endif
