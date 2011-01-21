/*! @file: iRegister.h
    @author Joshua Green
    @author Andrew Groot
    @brief Definition of a "register" in the Wi-11 machine.
*/

#ifndef iREGISTER_H
#define iREGISTER_H

#include "iWord.h"

class Word;
class Register;

/*! @brief Defines a "register" in the Wi-11 machine.
    
    The methods present in this inteface are meant to mimic the functionality of the Wi-11
    machine, allowing for simplified execution of the instructions therein.
    This interace class will serve as a base from which the general purpose
    registers and program counter of the Wi-11 can be defined.
*/
class iRegister {
  private:
    
  public:
    /*! @brief Retrieves a copy of the word of data store in the register.
        @post The value of the calling object is not changed.
        @return A new Word object holding the value that is stored in the register.
    */
    virtual Word GetValue() const = 0;

    /*! @brief Adds a word of data to the calling object.
        @param[in] w
          The value to be added.
        @post The calling object equals its previous value plus the value of "w"; "w", however, will remain unchanged.
    */
    virtual void Add(const iWord& w) = 0;

    /*! @brief Adds a word of data to the calling object.
        @param[in] r
          The value to be added.
        @post Both the calling object and "r" will not be changed.
        @return A new Register object holding the value of the calling object plus the value in "r".
    */
    virtual Register Add(const iRegister& r) const = 0;

    /*! @brief A standard add operator.
    
        @note    
        "result = p + r" is equivalent to "result = p.Add(r)".
    */
    virtual Register operator+(const iRegister& r) const = 0;

    /*! @brief Subtracts a word of data from the calling object.
        @param[in] w
          The value to be subtracted.
        @post The calling object equals its previous value minus the value of "w"; "w", however, will remain unchanged.
    */
    virtual void Subtract(const iWord& w) = 0;

    /*! @brief Subtracts a word of data from the calling object.
        @param[in] r
          The value to be subtracted.
        @post Both the calling object and "r" will not be changed.
        @return A new Register object holding the value of the calling object minus the value in "r".
    */
    virtual Register Subtract(const iRegister& r) const = 0;

    /*! @brief A standard subtraction operator.
        
        @note
        "result = p - r" is equivalent to "result = r.Subtract(w)".
    */
    virtual Register operator-(const iRegister& r) const = 0;

    /*! @brief Performs a bit-wise and.
        @param[in] w
          The value to be "and"ed.
        @post The calling object equals its previous value bit-wise and'ed with w.
    */
    virtual void And(const iWord& w) = 0;

    /*! @brief Performs a bit-wise and.
        @param[in] r
          The value to be "and"ed.
        @post Both the calling object and r are not changed.
        @return A new Register object holding the value of the calling object bit-wise and'ed with r.
    */
    virtual Register And(const iRegister& r) const = 0;

    /*! @brief Performs a bit-wise "or".
        @param[in] w
          The value to be "or"ed.
        @post The calling object equals its previous value bit-wise or'ed with w.
    */
    virtual void Or(const iWord& w) = 0;

    /*! @brief Performs a bit-wise or.
        @param[in] r
          The value to be "or"ed.
        @post Both the calling object and r are not changed.
        @return A new Register object holding the value of the calling object bit-wise or'ed with r.
    */
    virtual Register Or(const iRegister& r) const = 0;

    /*! @brief Performs a bit-wise not.
        @post The calling object's bits are all flipped (e.g. 1001 -> 0110).
    */
    virtual void Not() = 0;
    
    /*! @brief Performs a bit-wise not.
        @post The calling object is not changed.
        @return A new Register object holding the bit-wise not of the calling object.
    */
    virtual Register Not() const = 0;

    /*! @brief Stores a word of data.
        @param[in] w
          The value to be store.
        @post The calling object's value is now "w".
    */
    virtual void Store(const iWord& w) = 0;

    /*! @brief Stores a copy of another register.
        @param[in] r
          The register to be copied.
        @post The calling object's value is now "r".
    */
    virtual void Store(const iRegister& r) = 0;

    /*! @brief A standard assignment operator.
        
        @note
        "r = w" is equivalent to "r.Store(w)"
    */
    virtual Register& operator=(const iWord& w) = 0;

    /*! @brief A standard assignment operator.

        @note
        "r1 = r2" is equivalent to "r1.Store(r2)"
    */
    virtual Register& operator=(const Register r) = 0;

    /*! @brief A standard pre-increment operator.
        @returns A reference to itself.

        The object increments its value BEFORE the execution of the current line.
    */
    virtual Register& operator++() = 0;

    /*! @brief A standard post-increment operator.
        @returns A reference to itself.

        The object increments its value AFTER the execution of the current line.
    */
    virtual Register& operator++(int) = 0;
};

#endif
