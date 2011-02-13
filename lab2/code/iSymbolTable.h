/*! @file iSymbolTable.h
    @author Andrew Groot
    @brief Definition of the symbol table for the Wi-11 assembler.
*/

#ifndef iSYMBOLTABLE_H
#define iSYMBOLTABLE_H

#include "Word.h"
#include <string>

/*! @brief Stores symbols and literals extracted from a source file.

    This class defines an interface for storing various mappings
    vital to the two-pass algorithm used in this assembler.
*/
class iSymbolTable {
  private:

  public:
    /*! @brief Add a label to the table.
        @param[in] label The label to the stored.
        @param[in] addr The address or value to associate it with.
        @param[in] relocate Whether or not the program is relocatable.
        @pre Contains(label) returns false.
    */
    virtual void InsertLabel(std::string label, Word addr, bool relocate=false) = 0;

    
    /*! @brief Add a literal to the table.
        @param[in] value The value to be stored.
        @param[in] addr The address whether the literal will be stored.
    */
    virtual void InsertLiteral(int value, Word addr) = 0;

    /*! @param[in] symbol The symbol to look for.
        @returns True iff "symbol" is in the table.
    */
    virtual bool Contains(std::string symbol) const = 0;


    /*! @brief Look up the address for a symbol.
        @param[in] symbol The symbol to look up.
        @pre Contains(symbol) returns true.
        @return The address of the symbol.
    */
    virtual Word GetLabelAddr(std::string symbol) const = 0;


    /*! @brief Look up the address for a literal.
        @param[in] value The literal value to look up.
        @return The address of the literal.

        A literal should always be present as it will be
        found where it was declared.  Any errors must
        be assembler related, not user related.
    */
    virtual Word GetLiteralAddr(int value) const = 0;


    /*! @param[in] label The label to look up.
        @pre Contains(label) returns true.
        @returns True iff the symbol is relocatable.
    */
    virtual bool IsRelocatable(std::string label) const = 0;
};

#endif
