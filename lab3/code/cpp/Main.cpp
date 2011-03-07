// File:        Main.cpp
// Written by:  Andrew Groot

#include "../h/Wi11.h"
#include "../h/Extractor.h"
#include "../h/SymbolTable.h"
#include "../h/Printer.h"
#include "../h/ResultCodes.h"
#include "../h/FileArray.h"
#include "../h/ObjParser.h"
#include "../h/itos.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>

using namespace std;
using namespace Codes;

int Assembler(const string& infile, const string& outfile, int symbol_length, bool trap_labels, bool listing);
int Linker(vector<string>& infiles, string& outfile);
int Simulator(string& infile, bool debug);

void print_usage_error(char * name, bool help = false) {
  cout << "Usage: " << name << " [-t | -s# | -l] -a infile outfile\n"
      <<  "       " << name << " [-t | -s# | -l] -o outfile [-n] infile ...\n"
      <<  "       " << name << " [-t | -s# | -l | -d] -ox outfile [-n] infile ...\n"
      <<  "       " << name << " [-d] -x infile\n\n";
      if (!help) {
        cout <<  "For more information run: " << name << " --help\n";
      }
} 

int main (int argc, char* argv[]) {
/*
  string in;
  cout << "Assemble? (Y/n) ";
  getline(cin, in);
  if (in[0] != 'N' && in[0] != 'n') {
    cout << "Source File Name: ";
    getline(cin, in);

    vector<string> infiles;
    infiles.push_back(in);

    string outfile = "out.o";
    Assembler(infiles, outfile, 1000, true, true);
  }

  cout << "Object File Name: ";
  string fname;

  getline(cin, fname);

  Simulator(fname, false);
  return 0;
*/
  // wi11
  bool assemble = true;
  bool link = true;
  bool execute = true;
  vector<string> infiles;
  string outfile;
  // assembler
  bool trap_labels = false;
  bool listing = false;
  int symbol_length = SYMBOL_TABLE_MAX_SIZE;
  // simulator
  bool debug = false;

  if (argc == 1) {
    // no arguments
    print_usage_error(argv[0]);
    return 1;
  }

  // help message
  if (argc == 2 && string(argv[1]) == "--help") {
    print_usage_error(argv[0], true);
    cout << "Options:\n"
        <<  "  -t:  Include the trap code labels -- out, puts, in, halt, outn, inn, & rnd.\n"
        <<  "  -s#: Increase the symbol limit -- defaults to 1000.\n"
        <<  "  -l:  Print a listing during assembly.\n"
        <<  "  -d:  Print verbose debug information during execution.\n"
        <<  "  --------------------------------------------------------------------------------------\n"
        <<  "  -a:  Assemble \"infile\" into \"outfile\", do not link or execute.\n"
        <<  "  -o:  Assemble and link multiple files, creating a single object file named \"outfile\"\n"
        <<  "  -ox: Assemble, link, and execute; create \"outfile\" as in the '-o' option.\n"
        <<  "  -x:  Execute a pre-linked object file.  Skip assembly and linking steps.\n"
        <<  "  -n:  Specifies that the input files are already assembled.\n"
        <<  "        The preceeding '-o' or '-ox' argument describes what should be done.\n"
        <<  endl
        <<  "Notes:\n"
        <<  "  -- The '-ox' option is the same as using the '-o' option then the '-x' option.\n"
        <<  "  -- The only compounding of arguments allowed is the '-ox' option.  All others\n"
        <<  "       must be printed separately.\n"
        <<  endl
        <<  "Examples:\n"
        <<  "  wi11 -t -s2000 -a file1.s file2.s file3.s\n"
        <<  "      This will create the object files the correspond to each file individually.\n"
        <<  "      They will not be linked or executed.\n"
        <<  endl
        <<  "  wi11 -l -t -o prog.o file1.s file2.s file3.s\n"
        <<  "      This will assemble and link file1.s, file2.s, and file3.s into prog.o.\n"
        <<  endl
        <<  "  wi11 -ox prog.o -n file1.o file2.o file3.o\n"
        <<  "      This will link file1.o, file2.o, and file3.o into prog.o and execute it.\n";
    return 0;
  }

  // get leading -? arguments
  int pos = 0;
  while ( ++pos < argc) {
    // make it a c++ string
    string arg_pos(argv[pos]);

    if (arg_pos.length() < 2 || arg_pos[0] != '-') {
      print_usage_error(argv[0]);
      return 1;
    }

    // check second character.
    switch (arg_pos[1]) {
      case 't': {             // -t -- add trap labels
        trap_labels = true;
      } break;

      case 'd': {             // -d -- execute in debug mode
        debug = true;
      } break;

      case 'l': {             // -l -- print listing
        listing = true;
      } break;

      case 's': {             // -s -- maximum number of symbols
        symbol_length = atoi(argv[pos] + 2);
        const int s_char_limit = 12;
        // 12 = 10 digits in MAX_INT + 2 for '-s'

        if (arg_pos.length() > s_char_limit) {
          cout << "Error: Number following \"-s\" too large.\n";
          return 1;
        }
      } break;

      case 'a': {             // -a -- only assemble
        link = false;
        execute = false;

        // get input files
        // Next argument (infile)
        ++pos;
        if (pos < argc) {
          infiles.push_back(argv[pos]);
        }
        // Next argument (outfile)
        ++pos;
        if (pos < argc) {
          outfile = argv[pos];
        }
        // Next argument (shouldn't be one)
        ++pos;
        if (pos != argc) {
          // extra stuff after outfile name
          print_usage_error(argv[0]);
          return 1;
        }
        // check for valid arguements
        if (debug) {
          print_usage_error(argv[0]);
          return 1;
        }
      } break;

      case 'o': {               // -o(x) -- assemble and link, possibly execute
        if (arg_pos.length() < 3 || arg_pos[2] != 'x') {
          execute = false;
        }
        // next argument
        ++pos;
        if (pos < argc) {
          // get output file
          outfile = argv[pos];
        } else {
          // no output file
          print_usage_error(argv[0]);
          return 1;
        }
        // get input files
        // next argument
        ++pos;
        if (pos < argc) {
          // check for link flag
          if (argv[pos] == "-n") {
            assemble = false;
          }
        }
        while (pos < argc) {
          infiles.push_back(argv[pos++]);
        }
        // check for valid arguements
        if (debug) {
          print_usage_error(argv[0]);
          return 1;
        }
      } break;

      case 'x': {               // -x -- only execute given object file
        assemble = false;
        link = false;
        // next argument
        ++pos;
        if (pos < argc) {
          // get file to execute
          outfile = argv[pos];
        }
        // next argument
        ++pos;
        if (pos != argc) {
          // extra stuff after -x
          print_usage_error(argv[0]);
          return 1;
        }
        // check for valid arguements
        if (trap_labels || listing) {
          print_usage_error(argv[0]);
          return 1;
        }
      } break;

      default: {
        print_usage_error(argv[0]);
        return 1;
      } break;
    }
    // end of switch
  }
  // done parsing arguements

  // Execute desired functionality.
  vector<string> temp_files;
  if (assemble) {
    cout << "Assembling...\n";
    if (!link && !execute) {
      Assembler(infiles[0], outfile, symbol_length, trap_labels, listing);
    } else {
      for (int i=0; i<infiles.size(); i++) {
        char temp_name[L_tmpnam];
        tmpnam(temp_name);
        
        temp_files.push_back(temp_name);

        Assembler(infiles[i], temp_name, symbol_length, trap_labels, listing);
      }
    }
  }
  if (link) {
    cout << "Linking...\n";
    if (temp_files.size() == 0) {
      Linker(infiles, outfile);
    } else {
      Linker(temp_files, outfile);
    }
  }
  if (execute) {
    cout << "Executing...\n";
    return Simulator(outfile, debug);
  }

  // remove temp files
  bool rm_err = false;
  for (int i=0; i<temp_files.size(); i++) {
    if (remove(temp_files[i].c_str()) != 0) {
      rm_err = true;
    }
  }
  if (rm_err) {
    // report failed deletions 
    cout << "WARNING: One or more temp files could not be removed.";
  }
  return 0;
}

int Assembler(const string& infile, const string& outfile, int symbol_length, bool trap_labels, bool listing) {
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
    if (trap_labels) {
      // OUT
      symbols.InsertLabel("out", Word(0x21));
      // PUTS
      symbols.InsertLabel("puts", Word(0x22));
      // IN
      symbols.InsertLabel("in", Word(0x23));
      // HALT
      symbols.InsertLabel("halt", Word(0x25));
      // OUTN
      symbols.InsertLabel("outn", Word(0x31));
      // INN
      symbols.InsertLabel("inn", Word(0x33));
      // RND
      symbols.InsertLabel("rnd", Word(0x43));
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

int Linker(vector<string>& infiles, string& outfile) {
  // open files
  // input files
  FileArray files;
  ResultDecoder decoder;
  for (int i = 0; i < infiles.size(); i++) {
    RESULT result = files.Add( infiles[i] );
    if (result.msg != SUCCESS) {
      cout << decoder.Find(result);
      return 1;
    }
  }
  //output file
  ofstream outs;
  outs.open(outfile.c_str());
  if (!outs.is_open()) {
    RESULT result(FILE_NOT_OPENED, outfile);
    cout << decoder.Find(result);
    return 1;
  }

  // get symbols
  SymbolTable symbols;
  ObjectData line;
  int length = 0;
  for (int i = 0; i < files.Size(); i++) {
    int pos = 0;
    cout << "Parsing " << files.Name(i) << ":\n";

    // Get header record
    line = files[i].GetNext();
    ++pos;
    if (line.type == 'H' || line.type == 'M') {
      // H(M)|NAME  |SIZE

      // Make sure it's not absolute
      if (line.data[1] != RELOCATE_FLAG) {
        RESULT result(LINK_ABS, files.Name(i));
      }
      // store as an entry point
      symbols.InsertLabel(line.data[0], length);
      // keep track of lenght
      Word size;
      size.FromHex("0x" + line.data[2]);
      length += size.ToInt();
    } else {
      // Doesn't start with header record
      cout << decoder.Find(RESULT(INVALID_HEADER_ENTRY, "Line " + itos(pos)));
      return 1;
    }

    line = files[i].GetNext();
    ++pos;
    // wait for E or junk
    while (line.type == 'N' || line.type == 'X' || line.type == 'x' ||
           line.type == 'R' || line.type == 'W' || line.type == 'T') {
      // only N's matter at this point
      // check for errors

      if (line.type == 'N') {
        // N|SYMBOL_NAME|ADDR
        Word value;
        if (!value.FromHex("0x" + line.data[1])) {
          cout << decoder.Find(RESULT(INV_HEX, "Line " + itos(pos)));
          return 1;
        }
        symbols.InsertLabel(line.data[0], value);
      }
      line = files[i].GetNext();
      ++pos;
    }

    // found something that's not a normal text record
    // either an end record or an error
    if (line.type != 'E') {
      cout << decoder.Find(RESULT(INVALID_DATA_ENTRY, "Line " + itos(pos)));
    }
  }
  // done getting symbols
  // all must have valid syntax

  const int PAGE_SIZE = 0x01FF;
  // check length
  if (length > PAGE_SIZE) {
    cout << decoder.Find(RESULT(MEM_FIT, itos(length - PAGE_SIZE) + " extra lines"));
    return 1;
  }
  // Go back to the top of each file
  files.Reset();
  Word full_length(length);
  length = 0;  // the number of data records in outfile

  // print output
  // header record
  line = files[0].GetNext(); // main header record -- for name
  // H|MAIN  |SIZE
  outs << 'H' << line.data[0] << full_length.ToHex().substr(2) << endl;

  int pos = 1; // line number
  string main_end; // keep the main file end record
  for (int i = 0; i<files.Size(); i++) {
    line = files[i].GetNext();
    if (line.type == 'H') { // can't be 'M' because we're already past the main file's header
      // ignore all other headers
      line = files[i].GetNext();
    }
    pos = 1;
    while (line.type == 'N' || line.type == 'X' || line.type == 'x' ||
           line.type == 'R' || line.type == 'W' || line.type == 'T') {
      switch (line.type) {
        case 'N': {
          // ignore
        } break;

        case 'x':
        case 'X': {
          // X(x)|ADDR|DATA(INST)
          // SYMBOL_NAME
          if (!symbols.IsSymbol(line.data[2])) {
            // SYMBOL_NAME is not in the table
            cout << decoder.Find(RESULT(UNRESOLVED_EXTERNAL, "Line " + itos(pos)));
            return 1;
          }
          Word addr, data, value;
          if (!addr.FromHex("0x" + line.data[0])) {
            cout << decoder.Find(RESULT(INV_HEX, "Line " + itos(pos)));
            return 1;
          }
          addr = addr + Word(length); // apply offset to addr
          if (!data.FromHex("0x" + line.data[1])) {
            cout << decoder.Find(RESULT(INV_HEX, "Line " + itos(pos)));
            return 1;
          }
          // get value of symbol
          value = symbols.GetLabelAddr(line.data[2]);

          // addr and data are valid hex, symbol value retrieved
          if (line.type == 'x') {
            // value = pgoffset9
            for (int j = 9; i < WORD_SIZE; j++) {
              // copy page number
              value.SetBit(j, data[j]);
            }
            outs << 'R' << addr.ToHex().substr(2) << value.ToHex().substr(2);
            
          } else { // line.type == 'X'
            // value = word of data
            outs << 'W' << addr.ToHex().substr(2) << value.ToHex().substr(2);
          }
          ++length;
        } break;

        default: {
          // T(R/W)|ADDR|INST
          Word addr, inst;
          addr.FromHex("0x" + line.data[0]);
          addr = addr + Word(length);
          inst.FromHex("0x" + line.data[0]);
          outs << line.type << addr.ToHex().substr(2) << inst.ToHex().substr(2) << endl;
          ++length;
        } break;
      } // end switch
      // Must be end record
      if (i == 0) {
        main_end = line.type + line.data[0];
      }
      // done with file
    }
  }
  // output main's end record
  outs << main_end << endl;

  // done, close outfile
  outs.close();

  return 0;
}

int Simulator(string& infile, bool debug) {
  Wi11 simulator;
/*
  if (debug) cout << "Loading object files... ";
  for (int i=0;i<obj_files.size();i++) {
    if (!simulator.LoadObj(obj_files[i].c_str())) {
      cout << "Aborting." << endl;
      return 1;
    }
  }
  if (debug) cout << "done." << endl;
*/

  simulator.LoadObj(infile.c_str());

  if (debug) {
    // print initial status
    simulator.DisplayRegisters();
    simulator.DisplayMemory();
    cout << endl;

    // prompt to execute
    cout << "Maximum Execution Iterations: (-1 for infinite) ";
    int iterations;
    string iteration_input;
    getline(cin, iteration_input);

    iterations = atoi(iteration_input.c_str());


    cout << "Select Execution Mode: (Trace, Step, Quiet) ";
    string mode;
    getline(cin, mode);
    if (mode[0] == 'T' || mode[0] == 't') {
      cout << "[Trace Mode]" << endl;

      while (iterations != 0 && simulator.ExecuteNext(true)) {
        --iterations;
      }

      cout << endl;
      simulator.DisplayRegisters();
      simulator.DisplayMemory();
    }
    else if (mode[0] == 'S' || mode[0] == 's') {
      cout << "[Step Mode]" << endl;
      while (iterations != 0 && simulator.ExecuteNext(true)) {
        --iterations;
        cout << "Continue? (Y/n/(p)rint) ";
        string temp;
        getline(cin, temp);

        if (temp[0] == 'p' || temp[0] == 'P') {
          cout << "Print? (m)emory/(r)egisters/(B)oth ";
          getline(cin, temp);

          cout << endl;

          if ((temp[0] == 'B' || temp[0] == 'b')) {
            simulator.DisplayRegisters();
            simulator.DisplayMemory();
          }
          else if (temp[0] == 'm' || temp[0] == 'M') simulator.DisplayMemory();
          else if (temp[0] == 'r' || temp[0] == 'R') simulator.DisplayRegisters();
          else {
            simulator.DisplayRegisters();
            simulator.DisplayMemory();
          }

          cout << endl;
        }
        else if (!(temp[0] != 'n' && temp[0] != 'N')) {
          cout << "User Abort." << endl;
          break;
        }
      }

      cout << endl;
      simulator.DisplayRegisters();
      simulator.DisplayMemory();
    }
    else if (mode[0] == 'Q' || mode[0] == 'q') {
      cout << "[Quiet Mode]" << endl;

      while (iterations != 0 && simulator.ExecuteNext(false)) {
        --iterations;
      }
    }
    if (iterations == 0) {
      cout << "Maximum iterations reached. Aborting." << endl;
    }
  }
  else {
    // Not in debug mode:
    while (simulator.ExecuteNext(false));
  }

  return 0;
}

