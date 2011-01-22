/*! @file iWi11.h
    @author Joshua Green
    @author Andrew Groot
    @brief Definition of the Wi-11 machine simulator.
*/

#ifndef iWI11_H
#define iWI11_H

#include "iWord.h"
#include "iRegister.h"
#include "iMemory.h"
#include "iDecoder.h"
#include "ResultCodes.h"

/*! @brief Defines the internal logic of the Wi-11.
    
    @par
    The methods present in this interface are meant to
    simulate the Wi-11's fetch-execute loop.
    Any implementation of this will be expected to house
    8 private instances of the Register class as general
    purpose registers and each of these should have an
    associated REGISTER_ID enum token.  A reference to
    an iMemory class is also necessary.

    @par
    The implementers of a super class will also have to
    incorporate some sort of interaction with a CCR structure.
    An interface for this interaction is not provided.
*/
class iWi11 {
  private:
    /*! @brief Retrieves a reference to the register corresponding to "id".
        @param[in] id A REGISTER_ID corresponding to one of the private registers.
        @return A reference to the id'd register.
    */
    virtual iRegister& _GetRegister(const Decoder::REGISTER_ID& id) = 0;
    //  [This may prove useful. It is essentially a C Switch which returns
    //  a reference to the appropriate instance of a Register object which
    //  corresponds to the provided REGISTER_ID token.]
    

    /*! @brief Adds two registers and stores the result in a third.
        @param[out] DR The destination register.
        @param[in] SR1 The first source register.
        @param[in] SR2 The second source register.
        @post SR1 and SR2 are not changed.
        @return SUCCESS or, if something went wrong, an appropriate error code.

        @note
        Updates the CCR.
    */
    virtual Codes::RESULT _Add(const Decoder::REGISTER_ID DR, const Decoder::REGISTER_ID SR1, const Decoder::REGISTER_ID SR2) = 0;


    /*! @brief Adds a constant to a register and stores the result in another.
        @param[out] DR The destination register.
        @param[in] SR1 The source register.
        @param[in] immediate The immediate value.
        @post SR1 and "immediate" are not changed.
        @return SUCCESS or, if something went wrong, an appropriate error code.

        @note
        Updates the CCR.
    */
    virtual Codes::RESULT _Add(const Decoder::REGISTER_ID DR, const Decoder::REGISTER_ID SR1, const iWord& immediate) = 0;


    /*! @brief Bit-wise ands two registers and stores the result in a third.
        @param[out] DR The destination register.
        @param[in] SR1 The first source register.
        @param[in] SR2 The second source register.
        @post SR1 and SR2 are not changed.
        @return SUCCESS or, if something went wrong, an appropriate error code.

        @note
        Updates the CCR.
    */
    virtual Codes::RESULT _And(const Decoder::REGISTER_ID DR, const Decoder::REGISTER_ID SR1, const Decoder::REGISTER_ID SR2) = 0;


    /*! @brief Bit-wise ands a register with a constant and stores the result in another register.
        @param[out] DR The destination register.
        @param[in] SR1 The source register.
        @param[in] immediate The immediate value.
        @post SR1 and "immediate" are not changed.
        @return SUCCESS or, if something went wrong, an appropriate error code.

        @note
        Updates the CCR.
    */
    virtual Codes::RESULT _And(const Decoder::REGISTER_ID DR, const Decoder::REGISTER_ID SR1, const iWord& immediate) = 0;


    /*! @brief Changes the last 9 bits of the PC.
        @param[in] address The 9 bits to become the end of the PC.
        @post "address" is not changed.
        @return SUCCESS or, if something went wrong, an appropriate error code.
    */
    virtual Codes::RESULT _Branch(const iWord& address) = 0;


    /*! @brief Deprecated?

        Does nothing.
    */
    virtual Codes::RESULT _Debug() = 0;


    /*! @brief Initiate a jump to a subroutine (alter the PC).
        @param[in] w A 9 bit offset for the PC.
        @post The PC has "w" as its 9 least significant bits.
        @return SUCCESS or, if something went wrong, an appropriate error code.

        @note
        If the link bit was set for this instruction, R7 will hold the old value of the PC.
        However, the CCR will not be altered for this instruction, depite R7 being altered.
    */
    virtual Codes::RESULT _JSR(const iWord& w) = 0;


    /*! @brief Initiate a jump to a subroutine (alter the PC).
        param[in] baseR A register whose value acts as a base address.
        @param[in] address A 6 bit offset to the base address.
        @post The PC is the value in baseR plus the value in address.
        @return SUCCESS or, if something went wrong, an appropriate error code.

        @note
        If the link bit was set for this instruction, R7 will hold the old value of the PC.
        However, the CCR will not be altered for this instruction, depite R7 being altered.
    */
    virtual Codes::RESULT _JSRR(const iWord& baseR, const iWord& address) = 0;


    /*! @brief Loads a word in memory into a register.
        @param[out] DR The destination register.
        @param[in] address When concatenated with the PC, forms address in memory from which to load.
        @post Memory and "address" have not changed.
        @return SUCCESS or, if something went wrong, an appropriate error code.

        @note
        Updates the CCR.
    */
    virtual Codes::RESULT _Load(const Decoder::REGISTER_ID DR, const iWord& address) = 0;


    /*! @brief Performs an indirect load.
        @param[out] DR The destination register.
        @param[in] address A 9-bit offset to the PC.
        @post Memory and "address" have not changed.
        @return SUCCESS or, if something went wrong, an appropriate error code.
        
        Works similar to ::_Load() but when memory is read, it uses the address found
        to again access memory.  In this indirect way, a load can be made from anywhere
        in Memory.
        
        @note
        Updates the CCR.
    */
    virtual Codes::RESULT _LoadI(const Decoder::REGISTER_ID DR, const iWord& address) = 0;


    /*! @brief Performs a register-relative load.
        @param[out] DR The destination register.
        @param[in] baseR A register whose value works as a base address.
        @param[in] address An 6-bit index from the base address.
        @post Memory, "baseR", and "address" have no changed.
        @return SUCCESS or, if something went wrong, an appropriate error code.

        Loads from "baseR" plus "address".
        
        @note
        Updates the CCR.
    */ 
    virtual Codes::RESULT _LoadR(const Decoder::REGISTER_ID DR, Decoder::REGISTER_ID baseR, const iWord& address) = 0;


    /*! @brief Bit-wise nots a register and stores the result in another.
        @param[out] DR The destination register.
        @param[in] SR The source register.
        @return SUCCESS or, if something went wrong, an appropriate error code.
        
        @note
        Updates the CCR.
    */
    virtual Codes::RESULT _Not(const Decoder::REGISTER_ID DR, const Decoder::REGISTER_ID SR) = 0;


    /*! @brief Return from a subroutine.
        @post The PC now holds the value that was (and still is) in R7.
        @return SUCCESS or, if something went wrong, an appropriate error code.

        @note
        @par
        This can be used to jump anywhere in memory.
        However, this is not the intended usage.
        @par
        Updates the CCR.
    */        
    virtual Codes::RESULT _Ret() = 0;


    /*! @brief Stores a register's value into memory at a specified address.
        @param[in] SR1 The source register (holds the data to be stored).
        @param[in] address When concatenated with the PC, forms the address for the store.
        @post SR1 and "address" are not changed.
        @return SUCCESS or, if something went wrong, an appropriate error code.
    */
    virtual Codes::RESULT _Store(const Decoder::REGISTER_ID SR1, const iWord& address) = 0;


    /*! @brief Performs an indirect store.
        @param[in] SR1 The source register (holds the data to be stored).
        @param[in] address A 9-bit offset to the PC.
        @post "SR1" and "address" are not changed.
        @return SUCCESS or, if something went wrong, an appropriate error code.

        Works similar to ::_Store() but when memory is read, it uses the address found
        to again access memory.  In this indirect way, a store can be made to anywhere
        in Memory.
    */
    virtual Codes::RESULT _STI(const Decoder::REGISTER_ID SR1, const iWord& address) = 0;


    /*! @brief Perfroms a register-relative store.
        @param[in] SR1 The source register (holds the data to be stored).
        @param[in] baseR A register whose value acts as a base address.
        @param[in] address A 6-bit index from the base address.
        @post SR1, baseR, and "address" are not changed.
        @return SUCCESS or, if something went wrong, an appropriate error code.
    */  
    virtual Codes::RESULT _STR(const Decoder::REGISTER_ID SR1, const Decoder::REGISTER_ID baseR, const iWord& address) = 0;


    /*! @brief Branches to a trap vector.
        @param[in] code The trap code.
        @post "code" is not changed.
        @return SUCCESS or, if something went wrong, an appropriate error code.

        @par
        The traps are as follows:
        @arg 0x21 - OUT   - Write the character formed from the eight
          least significant bits of R0 to standard out.
        @arg 0x22 - PUTS  - Write the a string to standard out starting at
          the address pointed to by R0 and ending at a null character.
        @arg 0x23 - IN    - Prompt for, and read, a single character from standard in.
          Re-print it and store its ascii value in R0 (with leading zeros).
        @arg 0x25 - HALT  - End execution and print an appropriate message to standard out.
        @arg 0x31 - INN   - Prompt for, and read, a positive decimal number from standard in.
          Re-print it and store it in R0 (the number must in 16-bit range).
        @arg 0x43 - RND   - Generate a random number and store it in R0.

        @note
        @par
        Traps 0x23, 0x31, and 0x43 all update the CCR.
        @par
        Standard in is the keyboard.\n
        Stardard out is the console.
    */
    virtual Codes::RESULT _Trap(const iWord& code) = 0;
   
   public:
    /*! @brief Creates and organizes the componts of the Wi11 machine.
        
        Initializes the general purpose registers, CCR, and memory.
    */
    virtual iWi11() = 0;

    /*  This will prove to be a useful command, but is not necessary at this
        time.
    */
    // virtual bool Reset() = 0;


    /*! @brief Loads the object file and sets up memory as it describes.
        @param[in] filename The name of the object file.
        @post "filename" is not changed.
        @return True if and only if the load was successful.

        If "false" is returned, prints an appropriate error message to the user.

        @note
        This fucntion can be called multiple times.
        Each time the PC is overwritten.
    */
    virtual bool LoadObj(const char* filename) = 0;


    /*! @brief Prints the state of memory to standard out.
        @post The calling object is not changed.
    */
    virtual void DisplayMemory() const = 0;


    /*! @brief Prints the state of every register to standard out.
        @post The calling object is not changed.
        
        The values of all 8 general purpose registers, the CCR, and PC
        are all printed.
    */
    virtual void DisplayRegisters() const = 0;


    /*! @brief Executes the instruction pointed to by the PC.
        @param[in] verbose If true, machine state information is displayed after each step.
        @return True if and only if the end of the program have been reached.

        @par
        This function is the brains of the operation, so to speak.
        Almost the entire fetch-execute loop of the Wi-11 is present here.
        In particular, this function must interpret the instructions and
        manage the CCRs.
        @par
        For a complete list of the instructions, see \ref instructions "Wi-11 Instructions".
    */
    virtual bool ExecuteNext(bool verbose=false) = 0;
    /*  Data is loaded at the address contained within the PC from Memory,
        then the PC is incremented by one. The loaded data is sent to the
        iDecoder component which returns an Instruction struct. The type
        field within the struct is used to determine the appropriate
        private function to execute (in the case of branching, ExecuteStep()
        determines if the branch should be executed. The combination of the
        type field coupled with the size of the data field determines the
        appropriate parameters. If the end of the program has been reached,
        ExecuteStep() returns true. [In the official spec, the list of the
        commands and how they are determined must be included in this 
        section. For now, reference page 3 of "The Wi11 Machine
        Characteristics" handout.]
    */
};

#endif
