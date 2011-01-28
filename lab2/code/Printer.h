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

class Printer : public iPrinter {
  private:
    std::ofstream _fileStream;

  public:
    ~Printer();

    bool Open(std::string filename);
    void Print(iListing& listing);
}

#endif
