
#include "Parser.h"
//? #include "Listing.h"
#include "Printer.h"
#include <string>
#include <iostream>
using namespace std;

int main (int argc, char* argv[]) {
  bool TRAP_LABELS;
  string infile, outfile;

  if (argc == 3 || argc == 4) {
    if (argv[1][0] == '-' && argv[1][1] == 't') {
      TRAP_LABELS = true;
    }
    
    infile = argv[2];
    outfile = argv[3];
  } else {
    cout << "Error: Usage: " << argv[0] << " [-t] <input_file> <output_file>\n";
    return 1;
  }

  Parser parser;
  if (!parser.Load(infile, TRAP_LABELS)) {
    cout << "Error: file " << infile << " could not be opened.\n";
    return 2;
  } else {
    Printer printer;
    if (!printer.Open(outfile)) {
      cout << "Error: file " << outfile << " could not be opened.\n";
      return 2;
    } else {
      printer.Print(parser.GetListing()); // need to include Listing here?
    }
  }
  
  return 0;
}
