/*! @file iWord.h
    @author Joshua Green
    @author Andrew Groot
    @brief Definition of a "word" of data.
    
    @details Defines the operations and signatures by which a "word" class should operate.
    The signatures, while intended to be coded to the interface, are done as to this as C++ allows.
*/

#ifndef iWORD_H
#define iWORD_H

#include <string>

class Word;

/*! @brief Defines a "word" of data on the Wi-11 Machine.

    The methods present in this inteface are meant to mimic the functionality of the Wi-11
    machine, allowing for simplified execution of the instructions therein.
    As the size of a "word" depends on the architecture, classes implementing this
    interface should define the word length to be 16 bits in length.
*/
class iWord {
  private:
    
  public:
    /*! @brief "To non-negative Integer"
        @post The value of the word is not changed.
        @return The bits of the word interpreted as a positive integer value.
    */
    virtual int ToInt() const = 0;

    /*! @brief "To Integer as 2's Complement"
        @post The value of the word is not changed.
        @return The bits of the word interpreted as a signed (2's complement) integer value.
    */
    virtual int ToInt2Complement() const = 0;

    /*! @brief "To String"
        @post The value of the word is not changed.
        @return 16 characters: each either a 1 or 0
        
        @par Examples:
        If the object holds a (2's comp.) value 4:  "0000000000000100"\n
        If the object holds a (2's comp.) value -1: "1111111111111111"
    */
    virtual std::string ToStr() const = 0;

    /*! @brief "To Hexadecimal"
        @post The value of the word is not changed.
        @return "0x" + <4 characters in the range [0-9],[A-F]>
        
        @par Examples:
        If the object holds (2's comp.) value 8:  "0x0008"\n
        If the object holds (2's comp.) value -2: "0xFFFE"
    */
    virtual std::string ToHex() const = 0;

    /*! @brief "From Integer"
        @param[in] value
          The value to be stored into the word.
        @post "value" is not changed.
        @return True if and only if "value" can be represented in 16 bits
        
        When this function returns "False", the value of the word is unchanged.\n
        Otherwise, the word now holds the value "value".
    */
    virtual bool FromInt(int value) = 0;

    /*! @brief "From String"
        @param[in] str
          A string of characters meant to represent a "word" to be stored.
        @post "str" is not changed.
        @return True if and only if "str" is well-formed (as defined in #toStr()).
        
        When this function returns "False", the value of the word is unchanged.\n
        Otherwise, the word now holds the value "str".
    */
    virtual bool FromStr(const std::string& str) = 0;

    /*! @brief "From Hexadecimal"
        @param[in] str
          A string of characters meant to represent a "word" to be stored.
        @post "str" is not changed.
        @return True if and only if "str" is well-formed (as defined in #toHex()).
        
        When this function returns "False", the value of the word is unchanged.\n
        Otherwise, the word now holds the value "str".
    */
    virtual bool FromHex(const std::string& str) = 0;

    /*! @brief Adds two words.
        @param[in] w
          A word value to be added.
        @post Both "w" and the calling object do not change.
        @return A new "Word" object containing result of adding "w" and the calling object.

        @note
        The addition is carried out with no regard to logical overflow.
    */
    virtual Word Add(const iWord& w) const = 0;

    /*! @brief A standard addition operator.
        
        @note
        "result = p + w" is equivalent to "result = p.Add(w)".
    */
    virtual Word operator+(const iWord& w) const = 0;

    /*! @brief Subtracts two words.
        @param[in] w
          A word value to be subtracted.
        @post Both "w" and the calling object do not change.
        @return A new "Word" object containing the result of subtracting "w" from the calling object.
        
        @note
        The subtraction is carried out with no regard for logical overflow.
    */
    virtual Word Subtract(const iWord& w) const = 0;

    /*! @brief A standard subtraction operator.
        
        @note
        "result = p - w" is equivalent to "result = p.Subtract(w)".
    */
    virtual Word operator-(const iWord& w) const = 0;

    /*! @brief "And"s the bits of two words.
        @param[in] w
          A word value to be "and"ed.
        @post Both "w" and the calling object do not change.
        @return A new "Word" object containing the result of performing a bit-wise and on "w" and the calling object.
    */
    virtual Word And(const iWord& w) const = 0;

    /*! @brief "Or"s the bits of two words.
        @param[in] w
          A word value to be "or"ed.
        @post Both "w" and the calling object do not change.
        @return A new "Word" object containing the result of performing a bit-wise or on "w" and the calling object.
    */
    virtual Word Or(const iWord& w) const = 0;

    /*! @brief "Not"s the bits of a word.
        @post The calling object do not change.
        @return A new "Word" object containing the result of performing a bit-wise not on the calling object.
    */
    virtual Word Not() const = 0;

    /*! @brief Copies a word.
        @param[out] w
          The value to be copied.
        @post The caller equals that parameter.

        Equivalent to the assignment "caller = parameter".
    */
    virtual void Copy(const iWord& w) = 0;

    /*! @brief A standard assignment operator.
        @param[in] w
          The value to be copied.
        @return A copy of the parameter.

        The return value and parameter here must be declared as "Word"s
        as C++ does not work well with polymorphic assignment operators.
    */ 
    virtual Word& operator=(const Word w) = 0;

    /*! @brief A standard pre-increment operator.
        @returns A reference to itself.

        The object increments its value BEFORE the execution of the current line.
    */
    virtual iWord& operator++() = 0;

    /*! @brief A standard post-increment operator.
        @returns A reference to itself.

        The object increments its value AFTER the execution of the current line.
    */
    virtual iWord& operator++(int) = 0;

    /*! @brief An accessor to the 'i'th bit of the value.
        @param[in] i
          The index of the bit in question.
        @pre The index must be less than the size of a word, ie. 16.
        @return True <=> 1, False <=> 0.

        The number of the bits starts at zero and rises into the more significant bits.
        @par Examples:
          If the object holds a value of 4 (0...100 in binary): num[2] = 1.\n
          If it holds a value of 1 (0...001 in binary): num[0] = 1.\n
          If it holds a negative value (Starting with a 1 in 2's complement): num[15] = 1.
    */
    virtual bool operator[](const int i) const = 0;
    virtual void SetBit(const int i) = 0;
};

#endif
