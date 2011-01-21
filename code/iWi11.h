// File: iWi11.h
// Written by:
//    Joshua Green

#ifndef iWI11_H
#define iWI11_H

#include "iWord.h"
#include "iRegister.h"
#include "iMemory.h"
#include "iDecoder.h"
#include "ResultCodes.h"

class iWi11 {
  /*  The Wi11 must have one instance of an iMemory class and needs
      multiple (7) instances of the Register class to serve as general
      purpose registers. Each register object should have an associated
      REGISTER_ID enum token. Wi11 should also contain some sort of CCR
      structure (unprovided) which must be updated appropriately.
  */

  private:
    virtual iRegister& _GetRegister(const Decoder::REGISTER_ID&) = 0;
    /*  [This may prove useful. It is essentially a C Switch which returns
        a reference to the appropriate instance of a Register object which
        corresponds to the provided REGISTER_ID token.]
    */

    virtual Codes::RESULT _Add(const Decoder::REGISTER_ID DR, const Decoder::REGISTER_ID SR1, const Decoder::REGISTER_ID SR2) = 0;
    /*  Adds the registers associated with SR1 and SR2 together,
        overwriting the register associated with DR with the result.
        Updates the CCR. If all operations are successful, returns the
        SUCCESS code. Upon a failure, the appropriate code is returned.
        Registers SR1 and SR2 are unchanged by this operation.
    */

    virtual Codes::RESULT _Add(const Decoder::REGISTER_ID DR, const Decoder::REGISTER_ID SR1, const iWord& immediate) = 0;
    /*  Adds the register associated with SR1 and immediate together,
        overwriting the register associated with DR with the result.
        Updates the CCR. If all operations are successful, returns the
        SUCCESS code. Upon a failure, the appropriate code is returned.
        Register SR1 is unchanged by this operation.
    */

    virtual Codes::RESULT _And(const Decoder::REGISTER_ID DR, const Decoder::REGISTER_ID SR1, const Decoder::REGISTER_ID SR2) = 0;
    /*  Logically ands the registers associated with SR1 and SR2 together,
        overwriting the register associated with DR with the result.
        Updates the CCR. If all operations are successful, returns the
        SUCCESS code. Upon a failure, the appropriate code is returned.
        Registers SR1 and SR2 are unchanged by this operation.
    */

    virtual Codes::RESULT _And(const Decoder::REGISTER_ID DR, const Decoder::REGISTER_ID SR1, const iWord& immediate) = 0;
    /*  Logically ands the register associated with SR1 and immediate
        together, overwriting the register associated with DR with the
        result. Updates the CCR. If all operations are successful, returns
        the SUCCESS code. Upon a failure, the appropriate code is returned.
        Register SR1 is unchanged by this operation.
    */

    virtual Codes::RESULT _Branch(const iWord& address) = 0;
    /*  Overwrites the PC's last nine bits with the last nine bits of address.
        If the operation is successful, the function returns the SUCCESS
        code. Upon a failure, the appropriate code is returned.
    */

    virtual Codes::RESULT _Debug() = 0;
    /*  Does nothing.
    */

    virtual Codes::RESULT _JSR(const iWord&) = 0;
    /*  [I forget what this operation does and if the CCR is updated.]
        If the operation was successful, the SUCCESS code is returned.
        Upon failure, the respective code is returned.
    */

    virtual Codes::RESULT _JSRR(const iWord& baseR, const iWord& address) = 0;
    /*  [I forget what this operation does and if the CCR is updated.]
        If the operation was successful, the SUCCESS code is returned.
        Upon failure, the respective code is returned.
    */

    virtual Codes::RESULT _Load(const Decoder::REGISTER_ID DR, const iWord& address) = 0;
    /*  Accesses the data stored within Memory at the given address and
        copies that value into the register associated with DR, overwriting
        DR's previous value. If the operation was successful, the SUCCESS
        code is returned. Upon failure, the respective code is returned.
        Memory is unchanged by this operation. [I forget if the CCR is
        updated by this operation.]
    */

    virtual Codes::RESULT _LoadI(const Decoder::REGISTER_ID DR, const iWord& address) = 0;
    /*  [I forget the specifics of this operation.]
        If the operation was successful, the SUCCESS code is returned.
        Upon failure, the respective code is returned. Memory is unchanged
        by this operation. [I forget if the CCR is updated by this
        operation.]
    */

    virtual Codes::RESULT _LoadR(const Decoder::REGISTER_ID DR, Decoder::REGISTER_ID baseR, const iWord& address) = 0;
    /*  [I forget the specifics of this operation. I believe it loads the
        address at the address which is formed by overwriting baseR's last
        six bits with the provided iWord address]. If the operation was
        successful, the SUCCESS code is returned. Upon failure, the
        respective code is returned. Memory is unchanged by this operation.
        [I forget if the CCR is updated by this operation.]
    */

    virtual Codes::RESULT _Not(const Decoder::REGISTER_ID DR, const Decoder::REGISTER_ID SR) = 0;
    /*  Not() stores the logical inversion of the value stored in register
        SR1 into the register DR. The register associated with SR1 is
        unchanged. Updates the CCR.
    */

    virtual Codes::RESULT _Ret() = 0;
    /*  [I forget the specifics of this operation.]
        If the operation was successful, the SUCCESS code is returned.
        Upon failure, the respective code is returned. Memory is unchanged
        by this operation.
    */

    virtual Codes::RESULT _Store(const Decoder::REGISTER_ID SR1, const iWord& address) = 0;
    /*  Stores the value in the register associated with SR1 in Memory at
        the address. SR1 and the CCR remain unchanged. If the operation
        was successful, the SUCCESS code is returned. Upon failure, the
        respective code is returned.
    */

    virtual Codes::RESULT _STI(const Decoder::REGISTER_ID SR1, const iWord& address) = 0;
    /*  [I forget the specifics of this operation.]
        If the operation was successful, the SUCCESS code is returned.
        Upon failure, the respective code is returned. Memory is unchanged
        by this operation.
    */

    virtual Codes::RESULT _STR(const Decoder::REGISTER_ID SR1, const Decoder::REGISTER_ID baseR, const iWord& address) = 0;
    /*  [I forget the specifics of this operation. I believe it stores the
        value at the address which is formed by overwriting baseR's last
        six bits with the provided iWord address]. If the operation was
        successful, the SUCCESS code is returned. Upon failure, the
        respective code is returned.
    */

    virtual Codes::RESULT _Trap(const iWord& code) = 0;
    /*  [Performs the TRAP associated with the provided code. See page
        4 of "The Wi11 Machine Characteristics". In the case of writing
        to the console, write to C standard out.]
    */
    
  public:
    virtual iWi11() = 0;
    /*  The iWi11 constructor must initialize its instantiation of iLoader
        with the address of the iWi11's Memory component. Additionally, the
        CCR and General Purpose Registers must be initialized.
    */

    // virtual bool Reset() = 0;
    /*  This will prove to be a useful command, but is not necessary at this
        time.
    */

    virtual bool LoadObj(const char*) = 0;
    /*  LoadObj() invokes iLoader's Load() with the provided filename.
        iLoader's Load() function effectively parses the *.obj file.
        If the Object file is valid, space is reserved within the
        Memory component and the obj data is stored within iWi11's
        Memory component. If no errors are encountered, LoadObj() sets the
        returns PC to the value returned by iLoader's Load() and then
        returns true. Otherwise, the appropriate RESULT code is looked up
        and printed to standard out and then returns false. This function
        can be invoked multiple times; multiple calls continue to
        overwrite the PC.
    */

    virtual void DisplayMemory() const = 0;
    /*  Debug function that prints the state of Memory to standard out.
        Does not modify the iWi11 component.
    */

    virtual void DisplayRegisters() const = 0;
    /*  Debug function that prints the state of all General Purpose
        Registers, CCR, and PC to standard out. Does not modify the iWi11
        component.
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
