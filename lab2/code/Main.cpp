// File:        Main.cpp
// Written by:  Andrew Groot

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
  int symbol_length = SYMBOL_TABLE_MAX_SIZE;
  TRAP_LABELS = false;

  if (argc == 3) {
    // get the file names
    infile = argv[1];
    outfile = argv[2];
  } else {
    // Wrong number of arguments.
    cout << "Error: Usage: " << argv[0] << " [-t] <input_file> <output_file>\n";
    return 1; // usage error
  }

  for (int i=3;i<argc;i++) {
    if (argv[i][0] == '-' && argv[i][0] == 's') {
      string size_flag(argv[i]);
      string ascii_size = size_flag.substr(1);
      symbol_length = atoi(ascii_size.c_str());
    }
    else if (argv[i][0] == '-' && argv[i][0] == 't') {
      TRAP_LABELS = true;

      infile = argv[2];
      outfile = argv[3];
    }
  }

  if (symbol_length < 1) {
    cout << "Invalid maximum symbol length size. Usage: " << argv[0] << " [-s<max_size>]" << endl;
    return 1;
  }

  Extractor extract(symbol_length);

  ResultDecoder results;
  if (!extract.Open(infile)) {
    cout << "Error: file " << infile << " could not be opened.\n";
    return 2; // i/o error
  } else {
    // First pass of file, get symbols
    SymbolTable symbols;
    RESULT result = extract.GetSymbols(symbols);
    if (result.msg != SUCCESS) {
      cout << results.Find(result);
      return 3; // syntax error
    }
    Word length = extract.GetLength();

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
    result = printer.Open(infile, outfile);
    if (result.msg != SUCCESS) {
      cout << results.Find(result);
      return 2; // i/o error
    } else {
      result = printer.Print(symbols, length);
      if (result.msg != SUCCESS) {
        cout << results.Find(result);
        return 3; // syntax error
      }
    }
  }
  
  return 0;
}
