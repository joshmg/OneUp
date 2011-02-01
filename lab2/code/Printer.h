/*! @file Printer.h
    @author Andrew Groot
    @brief Definition of the private data for the "Printer" class.
*/

#ifndef PRINTER_H
#define PRINTER_H

#include "iPrinter.h"
#include "iListing.h"
#include "ResultCodes.h"
#include <string>
#include <fstream>
#include <map>

class Printer : public iPrinter {
  private:
    std::ofstream _fileStream;
    std::map<std::string, iWord&> _symbols;
    std::map<int, iWord&> _literals;
    iWord& _length;

  public:
    Printer(std::map<std::string, iWord&>& symbols, std::map<int, iWord&>& literals);
    ~Printer();

    bool Open(std::string filename);
    Codes::RESULT Print(iWord& file_length);
};

#endif
