// File: Main.cpp
// Written by Andrew Groot

#include "Extractor.h"
#include "SymbolTable.h"
#include "Printer.h"
#include "ResultCodes.h"
#include <string>
#include <iostream>
using namespace std;
using namespace Codes;

int main (int argc, char* argv[]) {
  bool TRAP_LABELS;
  string infile, outfile;

  if (argc == 3) {
    // get the file names
    infile = argv[2];
    outfile = argv[3];

    TRAP_LABELS = false;

  } else if (argc == 4) {
    // check out '-t'
    if (argv[1][0] == '-' && argv[1][1] == 't') {
      TRAP_LABELS = true;

      infile = argv[2];
      outfile = argv[3];
    }
  } else {
    // Wrong number of arguments.
    cout << "Error: Usage: " << argv[0] << " [-t] <input_file> <output_file>\n";
    return 1; // usage error
  }

  Extractor extract;
  ResultDecoder results;
  if (!extract.Open(infile)) {
    cout << "Error: file " << infile << " could not be opened.\n";
    return 2; // i/o error
  } else {
    // First pass of file, get symbols
    SymbolTable symbols;
    RESULT result = extract.GetSymbols(symbols);
    if (result != SUCCESS) {
      cout << results.Find(result);
      return 3; // syntax error
    }
    Word length = extract.Length();

    // add trap codes if desired
    if (TRAP_LABELS) {
      // OUT
      symbols.InsertLabel("OUT", Word(0x21));
      // PUTS
      symbols.InsertLabel("PUTS", Word(0x22));
      // IN
      symbols.InsertLabel("IN", Word(0x23));
      // HALT
      symbols.InsertLabel("HALT", Word(0x25));
      // OUTN
      symbols.InsertLabel("OUTN", Word(0x31));
      // INN
      symbols.InsertLabel("INN", Word(0x33));
      // RND
      symbols.InsertLabel("RND", Word(0x43));
    }

    Printer printer;
    RESULT result = printer.Open(infile, outfile);
    if (result.msg != SUCCESS) {
      cout << results.Find(result);
      return 2; // i/o error
    } else {
      RESULT result = printer.Print(symbols, length);
      if (result.msg != SUCCESS) {
        cout << results.Find(result);
        return 3; // syntax error
      }
    }
  }
  
  return 0;
}
