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

void remove_temps(const vector<string>& files);
int Assembler(const string& infile, const string& outfile, int symbol_length, bool trap_labels, bool verbose);
int Linker(const vector<string>& infiles, const string& outfile);
int Simulator(const string& infile, bool debug);

void print_usage_error(char * name, bool help = false) {
  cout << "Usage: " << name << " [-t | -s# | -v] -a infile outfile\n"
      <<  "       " << name << " [-t | -s# | -v] -o outfile [-n] infile ...\n"
      <<  "       " << name << " [-t | -s# | -v | -d] -ox outfile [-n] infile ...\n"
      <<  "       " << name << " [-d] -x infile\n\n";
      if (!help) {
        cout <<  "For more information run: " << name << " --help\n";
      }
} 

int main (int argc, char* argv[]) {
  // wi11
  bool assemble = true;
  bool link = true;
  bool execute = true;
  vector<string> infiles;
  string outfile;
  // assembler
  bool trap_labels = false;
  bool verbose = false;
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
        <<  "  -v:  Verbose: Print a description of what is being done during assembly and linking.\n"
        <<  "  -d:  Print debug information during execution.\n"
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
        <<  "      They will not be linked or executed.  The maximum number of symbols is 2000\n"
        <<  "      and the trap codes will be added for the user.\n"
        <<  endl
        <<  "  wi11 -v -o prog.o file1.s file2.s file3.s\n"
        <<  "      This will assemble and link file1.s, file2.s, and file3.s into prog.o.\n"
        <<  "      Messages corresponding to the current phase of assembly or linking will\n"
        <<  "      be printed to the console.\n"
        <<  endl
        <<  "  wi11 -ox prog.o -n file1.o file2.o file3.o\n"
        <<  "      This will link file1.o, file2.o, and file3.o into prog.o and execute it.\n"
        <<  endl
        <<  "  wi11 -d -x prog.o\n"
        <<  "      This will execute prog.o but allow the user to input parameters by which\n"
        <<  "      the execution will take place as well as view the state of the machine\n"
        <<  "      after each individual instruction is executed.\n";
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

      case 'v': {             // -v -- verbose mode
        verbose = true;
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
          arg_pos = argv[pos];
          // check for link flag
          if (arg_pos == "-n") {
            assemble = false;
            ++pos;
          }
        }
        while (pos < argc) {
          infiles.push_back(argv[pos++]);
        }
        // check for valid arguements
        if (!execute && debug) {
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
        if (trap_labels || verbose) {
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
  int ret;  // return value for each of the sections

  // assemble
  if (assemble) {
    if (verbose) {
      cout << "Assembling...\n";
    }
    if (!link && !execute) {
      // say which file
      if (verbose) {
        cout << "-- " << infiles[0] << ":\n";
      }

      ret = Assembler(infiles[0], outfile, symbol_length, trap_labels, verbose);
      if (verbose) {
        cout << endl;
      }
    } else {
      // not assembling a single file
      for (int i=0; i<infiles.size(); i++) {
        // if listings will be printed, name each file on assemble
        if (verbose) {
          cout << "-- " << infiles[i] << ":\n";
        }

        char temp_name[L_tmpnam];
        tmpnam(temp_name);
        
        temp_files.push_back(temp_name);

        ret = Assembler(infiles[i], temp_name, symbol_length, trap_labels, verbose);
        if (verbose) {
          cout << endl;
        }
      }
    }
    if (ret != 0) {
      remove_temps(temp_files);
      return ret;
    }
  }
  // link
  if (link && infiles.size() > 1) {
    //        ^ don't link only one file
    if (verbose) {
      cout << "Linking... ";
    }
    if (temp_files.size() == 0) {
      ret = Linker(infiles, outfile);
    } else {
      ret = Linker(temp_files, outfile);
    }
    if (ret != 0) {
      remove_temps(temp_files);
      return ret;
    }
    if (verbose) {
      cout << "Done." << endl;
    }
  }
  // remove temp files, they are no longer need
  remove_temps(temp_files);

  // execute
  if (execute) {
    if (verbose) {
      cout << "Executing...\n";
    }
    return Simulator(outfile, debug);
  }
  // everyting went well
  return 0;
}

void remove_temps(const vector<string>& files) {
  // remove them one at a time
  for (int i=0; i<files.size(); i++) {
    // don't worry about failed removes,
    // it's probably just the assembler not
    // having been able to make it.
    remove(files[i].c_str());
  }
}

int Assembler(const string& infile, const string& outfile, int symbol_length, bool trap_labels, bool verbose) {
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
      cout << "In file: " << infile << ":\n";
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

    Printer printer(verbose);
    result = printer.Open(infile, outfile);
    if (result.msg != SUCCESS) {
      cout << "In file: " << infile << ":\n";
      cout << results.Find(result);
      return 2; // i/o error
    } else {
      result = printer.Print(symbols, length);
      if (result.msg != SUCCESS) {
        cout << "In file: " << infile << ":\n";
        cout << results.Find(result);
        return 3; // syntax error
      }
    }
  }

  return 0;
}

int Linker(const vector<string>& infiles, const string& outfile) {
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
    cout << decoder.Find(RESULT(FILE_NOT_OPENED, outfile));
    return 1;
  }

  // get symbols
  SymbolTable symbols;
  ObjectData line;
  int length = 0;
  const int PAGE_SIZE = 0x01FF;
  for (int i = 0; i < files.Size(); i++) {
    int pos = 0;

    // Get header record
    line = files[i].GetNext();
    ++pos;
    Word size; // length of this segment
    if (line.type == 'H' || line.type == 'M') {
      // H(M)|NAME  |SIZE

      // Make sure it's not absolute
      if (line.data[1] != RELOCATE_FLAG) {
        cout << decoder.Find(RESULT(LINK_ABS, files.Name(i)));
        return 1;
      }
      // store as an entry point
      symbols.InsertLabel(line.data[0], length);
      // keep track of length -- needs to be added to length later
      size.FromHex("0x" + line.data[2]);

      // check length
      if ((length + size.ToInt()) > PAGE_SIZE) {
        cout << decoder.Find(RESULT(MEM_FIT, itos(length - PAGE_SIZE) + " extra lines after " + itos(i) + " files"));
        return 1;
      }
    } else {
      // Doesn't start with header record
      cout << decoder.Find(RESULT(INVALID_HEADER_ENTRY, files.Name(i) + ": Line " + itos(pos)));
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
        // shift address
        value = value + Word(length);
        // store symbol
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

    length += size.ToInt();
  }
  // done getting symbols
  // all must have valid syntax

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
  int prev_length = 0;  // length at the end of the previous file

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
          --length; // to counteract the increment below.
        } break;

        // relocatables
        case 'R':
        case 'W':
        case 'x':
        case 'X': {
          // X(x)|ADDR|DATA(INST)
          // SYMBOL_NAME
          if (line.type == 'x' || line.type == 'X') {
            if (!symbols.IsSymbol(line.data[2])) {
              // SYMBOL_NAME is not in the table
              cout << decoder.Find(RESULT(UNRESOLVED_EXTERNAL, "Line " + itos(pos)));
              return 1;
            }
          }
          Word addr, data, value;
          // addr in new program
          if (!addr.FromHex("0x" + line.data[0])) {
            cout << decoder.Find(RESULT(INV_HEX, "Line " + itos(pos)));
            return 1;
          }
          addr = addr + Word(prev_length);
          // get data
          if (!data.FromHex("0x" + line.data[1])) {
            cout << decoder.Find(RESULT(INV_HEX, "Line " + itos(pos)));
            return 1;
          }

          // get offset value
          if (line.type == 'x' || line.type == 'X') {
            // store in pass one
            value = symbols.GetLabelAddr(line.data[2]);
          } else {
            // defined by relocatble property
            value = data + Word(prev_length);
          }

          // addr and data are valid hex, symbol value retrieved
          if (line.type == 'x' || line.type == 'R') {
            // value = pgoffset9
            for (int j = 9; j < WORD_SIZE; j++) {
              // copy offset
              value.SetBit(j, data[j]);
            }

            outs << 'R' << addr.ToHex().substr(2) << value.ToHex().substr(2) << endl;
            
          } else { // line.type == 'X'|'W'
            // value = word of data
            outs << 'W' << addr.ToHex().substr(2) << value.ToHex().substr(2) << endl;
          }
        } break;

        default: { // T
          // T|ADDR|INST
          // Echo the line with new addr
          Word addr;
          if (!addr.FromHex("0x" + line.data[0])) {
            cout << decoder.Find(RESULT(INV_HEX, "Line " + itos(pos)));
            return 1;
          }
          addr = addr + Word(prev_length);
          outs << line.type << addr.ToHex().substr(2) << line.data[1] << endl;
        } break;
      } // end switch

      line = files[i].GetNext();
      ++length;
    } // end while
    // Must be end record
    if (i == 0) {
      main_end = line.type + line.data[0];
    }
    prev_length = length;
  } // end for
  // output main's end record
  outs << main_end << endl;

  // done, close outfile
  outs.close();

  return 0;
}

int Simulator(const string& infile, bool debug) {
  Wi11 simulator;

  if (!simulator.LoadObj(infile.c_str())) {
    // Error in file, already printed
    return 1;
  }

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

