/*! @file Printer.h
    @author Andrew Groot
    @brief Definition of the private data for the "Printer" class.
*/

#ifndef PRINTER_H
#define PRINTER_H

#include "iPrinter.h"
#include "iListing.h"
#include <string>
#include <fstream>
#include <map>

class Printer : public iPrinter {
  private:
    std::ofstream _fileStream;
    std::map<std::string, iWord&> _labels;
    std::map<int, iWord&> _literals;
    iWord& _length;

  public:
    Printer(std::map<std::string, iWord&>* labels, std::map<int, iWord&>* literals, iWord& file_length);
    ~Printer();

    bool Open(std::string filename);
    void Print();
};

#endif
