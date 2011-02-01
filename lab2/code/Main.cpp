// File: Main.cpp
// Written by Andrew Groot

#include "Extractor.h"
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
  if (!extract.Load(infile)) {
    cout << "Error: file " << infile << " could not be opened.\n";
    return 2; // i/o error
  } else {
    RESULT result = extract.Read();
    if (result != SUCCESS) {
      cout << results.Find(result);
      return 3; // syntax error
    }

    // get everything from first pass though the file
    map<string, Word> symbols = extract.Symbols();
    map<int, Word> literals = extract.Literals();
    Word length = extract.Length();

    // add trap codes if desired
    if (TRAP_LABELS) {
      Word temp;
      temp.FromHex("0x0021");
      symbols["OUT"] = temp;
      temp.FromHex("0x0022");
      symbols["PUTS"] = temp;
      temp.FromHex("0x0023");
      symbols["IN"] = temp;
      temp.FromHex("0x0025");
      symbols["HALT"] = temp;
      temp.FromHex("0x0031");
      symbols["OUTN"] = temp;
      temp.FromHex("0x0033");
      symbols["INN"] = temp;
      temp.FromHex("0x0043");
      symbols["RND"] = temp;
    }

    Printer printer(symbols, literals);

    if (!printer.Open(outfile)) {
      cout << "Error: file " << outfile << " could not be opened.\n";
      return 2; // i/o error
    } else {
      RESULT result = printer.Print(length);
      if (result != SUCCESS) {
        cout << results.Find(result);
        return 3; // syntax error
      }
    }
  }
  
  return 0;
}
