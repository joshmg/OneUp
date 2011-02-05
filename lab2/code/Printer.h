/*! @file Printer.h
    @author Andrew Groot
    @brief Definition of the private data for the "Printer" class.
*/

#ifndef PRINTER_H
#define PRINTER_H

#include "iPrinter.h"
#include "SymbolTable.h"
#include "ResultCodes.h"
#include <string>
#include <fstream>
#include <map>

class Printer : public iPrinter {
  private:
    std::ofstream _fileStream;

  public:
    ~Printer();

    bool Open(std::string filename);
    Codes::RESULT Print(SymbolTable& symbols, iWord& file_length);
};

#endif
