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

/*! @brief The Simulator function parses the object file and uses the Wi11 class to simulate the Wi11 machine.
    @author Andrew Groot
*/
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


///////////////////////////////////////
//*** Project-wide documentation ***///
///////////////////////////////////////
/*! @mainpage Introduction
    Primary Authors: Joshua Green\n
                     Logan Coulson\n
                     Andrew Groot\n
    Edited by:       Andrew Canale\n

    @section intro Introduction
    The "Wi-11 Machine" is a simulated 16-bit computer architecture. It has 8 general purpose registers, 3 condition
code registers (CCRs), and a program counter (PC). The Wi-11 Simulator emulates the execution of the Wi-11 processor
and memory components. However, programming for the Wi-11 Simulator can be difficult as it only accepts encoded
object files as input.  To better enable the user to more easily produce executable code, this package includes an
assembler that translates assembly language into object files usable by the Wi-11 Simulator. Additionally, a linker
has been added to this package to further enable the user. The linker provides a lateral level of functionality,
combining multiple source files into one cohesive object file. Before alterations are made to the programming of this
machine simulator package, a basic understanding of the coded environment is required. In particular, it is necessary
to understand the formats of the assembly source files as well as the produced object file formats. It is also wise
to be aware how the components interact with one another, and be aware of the programming conventions used.

    @section asm_input Assembler Input
    @subsection format Format
    The input file provided to the assembler has a limitation on the maximum size of the symbol table, the .o
file, and the literal table. When the user runs the program, he/she may add a -s# to the end of their command; this
overrides the default value of 1000 symbols. The maximum number of literals allowed is one half the maximum number of
symbols specified by the user. Additionally, the number of records (the effective memory footprint (excluding literals))
within the object file is limited to twice the number of symbols specified by the user. Therefore, with the default
value, the maximum number of source records is is two thousand, one thousand symbols, and five hundred literals. Each
line of the input file must conform to the following structural organization:

    @subsubsection labels Labels
    With the exception of .ORIG and EQU, labels are optional per line. Labels may only be defined from the beginning
of a line until the first whitespace. Labels must not begin with an uppercase ‘R’, nor begin with a lowercase ‘x’, as
these indicate a register or a hexadecimal number respectively. Labels are case-sensitive, and may contain only
alphanumeric ASCII characters. The first character in a label must be alphabetic. If a label is defined, it can be
used as a reference in place of any Operand in Operation/Pseudo-Operation calls, except within operands in the same
line on which the label is defined. Additionally, if a label is being used to replace a register operand, it must be a
number zero through seven. Labels start on the leftmost position of the text file, and continue until a whitespace is
encountered. When a symbol is used as the last argument for ADD or AND instructions, it is always interpreted as an
imm5 operand rather than a source register.

    @subsubsection whitespace Whitespace
    Whitespace is any number of spaces and tabs in a row. Outside of comments, whitespace must exist before an
operation (but after a label, if present), between an operation and its operands, and after the last operand to the
end of the line or the start of a comment.

    @subsubsection operation Operations
    The operation field must be preceded on the same line by whitespace, or a label followed by whitespace. Operation
and pseudo-operation names must be uppercase letters. Pseudo-operations (for use only by the compiler) may take the
place of an operation, and are specified by beginning with a period. A list of all operations and pseudo-operations
can be found in the ‘Instructions’ section of this document. There must be whitespace between the operation field, and
its operands.

    @subsubsection operands Operands
    Operands continue to whitespace/end of line, or a semicolon. The types and numbers of operands are restricted by
the specific operation or pseudo-operation invoked. To see these limitations, refer to section 1.2.2.  A comma must be
present between any two operands. No whitespace is permitted between or within operands. This assembler supports the
following operands:

    @par Registers
    @arg Registers are specified by an uppercase ‘R’, followed by the number of the specific register. For example,
register one is written as ‘R1’. The registers available to the user range from R0 – R7.

    @par Constants
    @arg Constants must be either decimal or hexadecimal. Decimal constants used as an operand must be immediately
preceded by a pound sign ‘#’. Hexadecimal constants are preceded by a lowercase ‘x’. (i.e. Decimal 10 is written as
#10. Hexadecimal 2C is written as x2C). An imm5 operand must be in the range #-16..#15, or x0..x1F. An address operand
must be in the range #0..#65535, or x0..xFFFF. Note that only the least significant 9 bits of this value are used in
the machine code encoding. An index6 operand must be in the range #0...#63, or x0...x3F. A trapvect8 operand must be
in the range #0..#255, or x0..xFF.

    @subsubsection comments Comments
    Comments start with a semi-colon ‘;’, and may only come after code present on the same line, if any code is present
on the line.There must be whitespace between the operand and any comments. Alternatively, a comment may be present on
a line by itself. If a comment spans multiple lines, a semicolon must be present at the beginning of every line.

    @subsubsection sampleinput Sample Input
<pre>
; Should execute properly
; Example Program
Lab2EG   .ORIG   x30B0
count    .FILL   #4
Begin    LD      ACC,count       ;R1 <- 4
         LEA     R0,msg
loop     TRAP    x22             ;print "hi! "
         ADD     ACC,ACC,#-1     ;R1--
         BRP     loop
         JMP     Next
msg      .STRZ   "hi! "
Next     AND     R0,R0,x0        ;R0 <- 0
         NOT     R0,R0           ;R0 <- xFFFF
         ST      R0,Array        ;M[Array] <- xFFFF
         LEA     R5,Array
         LD      R6,=#100        ;R6 <= #100
         STR     R0,R5,#1        ;M[Array+1] <= xFFFF
         TRAP    x25
ACC      .EQU    #1
; ----- Scratch Space -----
Array    .BLKW   #3
         .FILL   x10
         .END    Begin
</pre>

    @subsection instructions Instructions
    This assembler supports many different functions and a healthy understanding of the different functions is advisable.
DR (Destination Register) is the location where the final value is stored. SR (Source Register) is the unchanged register
containing the value used by the operation. imm5 (Immediate) is 5 bits, and is sign extended to 16 bits when used. 
pgoffset9 (Page Offset Nine) is used to form the last 7 bits for memory access. The first 7 bits originate from the PC
(Program Counter), the remaining 9 bits are concatenated by the pgoffset9. index6 (Index Six) is used as a six bit number
that is added to a register(BaseR) to determine an offset. In addition to the operations provided by the simulator, there
are several pseudo ops that the assembler provides. A pseudo op is recognizable by the period ‘.’ at the beginning of the
command. Pseudo ops are not always directly translated to the register transfer language, but may often be directives to
the linker or assembler.

    @subsubsection orig .ORIG
    .ORIG x0-xFFFF\n
Excluding .MAIN, .ORIG must be the first non-comment record in the source program. The optional operand indicates the
absolute address the program is to be stored in. If the operand is absent, the program may be "Relocatable", that is may
be stored anywhere (determined at load time). If there is no operand, the entire program must fit in one page of memory
(512  16-bit words). It is also required to have a 6 character label, which is used to generate the header record in the
.o file. Additionally, the provided label is automatically declared as if it were declared as an Entry Point (via .ENT).

    @subsubsection main .MAIN
    .MAIN
.MAIN has no arguments and must be placed before the .ORIG statement and is not a required psuedo op if the current source
file is not linked (See Linker, .EXT and .ENT). This pseudo op declares the current source file as the
only starting point of execution and causes all other linked files' .END values to be ignored. A file with .MAIN does not
affect its ability to be assembled stand-alone. If multiple linked files contain the .MAIN pseudo op, then the linker will
report this condition as an error; remove all but one instance of .MAIN from all linked object files before linking. If the
linker is given multiple source files and is unable to find an instance of .MAIN then the first source file's .END record is
used as the start of execution.


    @subsubsection end .END
    .END x0-xFFFF\n
This operation indicates the end of the input program. The operand is optional, and it indicates where the program
will start executing. If the operand is not present, it means that execution begins at the first address in the
segment, and the assembler assumes an address of x0. There must be a .END command in every program. 

    @subsubsection equ .EQU
    .EQU\n
.EQU equates the label to the operand, creating a constant. This constant can be any previously defined symbol or
constant. .EQU requires a label and an operand to be present.

    @subsubsection fill .FILL
    .FILL #-32768-32768 or x0-xFFFF\n
.FILL defines a one word quantity, whose contents is the value of the operand. This operand can be in hexadecimal,
decimal, or a label. If a label is used, its value must be within acceptable ranges (i.e. #-32768..xFFFF). This value
is stored by the assembler in the word of memory that the .FILL occupies. Additionally, the assembler location counter
is moved forward one word.

    @subsubsection strz .STRZ
    .STRZ "a string with escaped \"'s"\n
.STRZ defines a null-terminated block of words to hold a string of the characters in the operand field (in this case,
the next 6 memory locations would hold the ASCII for "words\0"). ASCII representations are used to store these
characters in memory.

    @subsubsection blkw .BLKW
    .BLKW x1-xFFFF\n
.BLKW creates a block of storage. It occupies a number words of memory as indicated by the operand. The block of
storage is not initialized.

    @subsubsection ext .EXT
    .EXT \n
.EXT declares a single, or a list (separated by commas), of symbols. Symbols declared by .EXT must be defined via .ENT,
or via .ORIG, by a separate object file; using .EXT renders the current object unable to be assembled stand-alone and
must be linked with the file containing the symbol definitions via the Linker.

    @subsubsection ent .ENT
    .ENT \n
.ENT declares a single symbol name as for use outside the scope of the current object file. However, .ENT symbols may
also be used within the current object file. Declaring a symbol via .ENT does not affect its ability to be assembled
stand-alone.

    @subsubsection addop ADD
    ADD DR SR1 SR2 or ADD DR SR1 imm5\n
ADD takes the contents of SR1 and SR2 or imm5, adds them, and places the result in DR. 

    @subsubsection andop AND
    AND DR SR1 SR2 or AND DR SR1 imm5\n
AND performs a logical ‘and’ between SR1 and either SR2 or imm5, and places the result in DR.

    @subsubsection brop BRNZP
    BRNZP pgoffset9\n
BRNZP checks to see if the CCR matches the NZP component of the branch command. If it does, it sets the PC to be a
concatenation of bits [15:9] of the PC with bits [8:0] (pgoffset9) of the instruction.  Any combination of N's, Z's,
and P's are acceptable.

    @subsubsection dbugop DBUG
    DBUG\n
DBUG prints out the contents of all of the machine registers to the console.

    @subsubsection jsrop JSR
    JSR pgoffset9\n
JSR sets the PC to be a concatenation of bits [15:9] of the PC with bits [8:0] (pgoffset9) of the instruction.  JSR
also stores the current PC into R7, so that the RET instruction can restore the PC and cause execution to jump back to
the instruction following the JSR.

    @subsubsection jmpop JMP
    JMP pgoffset9\n
JMP behaves the same as JSR, except that it does not store the current program counter into R7. 

    @subsubsection jssrop JSRR
    JSRR BaseR index6\n
JSRR sets the PC equal to the contents of memory at location BaseR+index6. The original PC is stored in R7.

    @subsubsection jmprop JMPR
    JMPR BaseR index6\n
JMPR behaves the same as JSRR, except that it does not store the PC in R7.

    @subsubsection ldop LD
    LD DR pgoffset9\n
LD loads DR with the contents of memory at the concatenation of [15:9] of the PC with bits [8:0] (pgoffset9) of the
instruction.

    @subsubsection ldiop LDI
    LDI DR pgoffset9\n
LD loads DR with the contents of the memory address in the contents of memory at the address indicated by
concatenation of [15:9] of the PC with bits [8:0] (pgoffset9) of the instruction.

    @subsubsection ldrop LDR
    LDR DR  BaseR pgoffset9\n
DR is loaded with the operand at the address that is formed by adding the zero-extended six bit offset (index6) to the
specified base register (BaseR). Thus, the index is always interpreted as a positive quantity (in the range #0-#63).

    @subsubsection notop NOT
    NOT DR SR\n
NOT inverts the bits of the SR and stores the results in DR.

    @subsubsection retop RET
    RET\n
RET copies the contents of R7 to the PC.

    @subsubsection stop ST
    ST SR pgoffset9\n
ST stores the contents of SR in memory at the concatenation of [15:9] of the PC with bits [8:0] (pgoffset9) of the
instruction.

    @subsubsection stiop STI
    STI SR pgoffset9\n
STI stores SR in the memory address in the contents of memory at the address indicated by concatenation of [15:9] of
the PC with bits [8:0] (pgoffset9) of the instruction.

    @subsubsection strop STR
    STR SR BaseR index6\n
SR is saved at the address that is formed by adding the zero-extended six bit offset (index6) to the specified base
register (BaseR).  Thus, the index is always interpreted as a positive quantity (in the range 0-63).

    @subsubsection trapop TRAP
    TRAP trapvect8\n
    Depending on the trapvect8, trap does one of the following:
      @arg x21 out Write the character in R0[7:0] to the console.
      @arg x22 puts  Write the null-terminated string pointed to by R0 to the console.
      @arg x23 in  Print a prompt on the screen and read a single character from the keyboard.  The character is copied to the screen and its ASCII code is copied to R0.  The high 8 bits of R0 are cleared.
      @arg x25 halt  Halt execution and print a message to the console.
      @arg x31 outn  Write the value of R0 to the console as a decimal integer.
      @arg x33 inn Print a prompt on the screen and read a decimal number from the keyboard.  The number is echoed to the screen and stored in R0. One may place specific requirements on the size, formatting, etc. of the input.
      @arg x43 rnd Store a random number in R0.

    @section output Output
    The assembler has two distinct outputs. The first outputs an object file usable by a simulator of the Wi-11
machine, and the second is a human readable listing for the user.  The assembler will also display error codes on the
console if necessary.

    @subsection The Object File
    The object file consists of a header record, text records, and an end record as described in the Programmers guide
for the OneUp Wi-11 simulator. These records are generated by the assembler from the program taken in as input.

    @subsection The User's Listing
    The second output by the assembler is a more human readable output. It contains the source program, as well as its
assembly. The current format for this data is (Address in hexadecimal) contents hexadecimal contents binary (line
number) Label instruction operands. This allows for a easy to read output that has all of the necessary information in
it.

    @subsubsection Listing Example
<pre>
                              (   2) Lab2EG     .ORIG   x30B0
(30B0) 0004  0000000000000100 (   3) count      .FILL   #4
(30B1) 22B0  0010001010110000 (   4) Begin      LD      ACC,count       ;R1 <- 4
(30B2) E0B7  1110000010110111 (   5)            LEA     R0,msg
(30B3) F022  1111000000100010 (   6) loop       TRAP    x22             ;print "hi! "
(30B4) 127F  0001001001111111 (   7)            ADD     ACC,ACC,#-1     ;R1--
(30B5) 02B3  0000001010110011 (   8)            BRP     loop
(30B6) 40BC  0100000010111100 (   9)            JMP     Next
(30B7) 0068  0000000001101000 (  10) msg                .STRZ   "hi! "
(30B8) 0069  0000000001101001 (  10) 
(30B9) 0021  0000000000100001 (  10) 
(30BA) 0020  0000000000100000 (  10) 
(30BB) 0000  0000000000000000 (  10) 
(30BC) 5020  0101000000100000 (  11) Next       AND     R0,R0,x0      ;R0 <- 0
(30BD) 9000  1001000000000000 (  12)            NOT     R0,R0         ;R0 <- xFFFF
(30BE) 30C3  0011000011000011 (  13)            ST      R0,Array      ;M[Array] <- xFFFF
(30BF) EAC3  1110101011000011 (  14)            LEA     R5,Array
(30C0) 2CC7  0010110011000111 (  15)            LD      R6,=#100      ;R6 <= #100
(30C1) 7141  0111000101000001 (  16)            STR     R0,R5,#1      ;M[Array+1] <= xFFFF
(30C2) F025  1111000000100101 (  17)            TRAP    x25
                              (  18) ACC                .EQU    #1
(30C3)                        (  20) Array      .BLKW   #3
(30C6) 0010  0000000000010000 (  21)            .FILL   x10
(30C7) 0064  0000000001100100 ( lit) <100>
                              (  22)            .END    Begin
</pre>

    @subsection Errors
    The Assembler can also output errors as included in the ResultCodes.h file.  Additional errors can be added if
needed, but adding superfluous errors should generally be avoided. Any error message is always returned, and the
caller function either evaluates that result, or reports an error state.

    @section interaction Interaction
    The interaction portion of this document describes how each function works, and what other functions that function
calls.

    @subsection Main
    The main function parses the command line arguments and ensures their validity.
Additionally, the main function handles integration of the three core components of
the simulator package: assembler, linker, and simulator. It is possible to break
execution after each phase as well as begin execution at later phases.
    @subsubsection cmd_flags Commandline Flags
    - Assembler, Linker, and Simulator Shared Flags:\n    
-t:  Predefine the trap code labels -- out, puts, in, halt, outn, inn, & rnd. Optional.\n
-s#: Increase the symbol limit -- defaults to 1000. Optional.\n
-v:  Verbose: Print a description of what is being done during assembly and linking. Optional.\n

    - Assembler-Specific Flags:\n
-a:  Assemble "infile" into "outfile", do not link or execute.\n

    - Linker-Specific Flags:\n
-o:  Assemble and link multiple files, creating a single object file named "outfile"\n
-n:  Specifies that the input files are already assembled.\n

    - Simulator-Specific Flags:\n
-ox: Assemble, link, and execute; create "outfile" as in the '-o' option. Either 'ox' or 'x' flags required.\n
-x:  Execute a pre-linked object file.  Skip assembly and linking steps. Either 'ox' or 'x' flags required.\n
-d:  Print debug information during execution. Optional.\n
-n:  Specifies that the input files are already assembled. Optional.\n

    @subsection Assembler
    The Assembler uses the extractor class to generate the symbol tables. This assembler uses a 2-pass method. The symbol
table is generated during the first pass. The second pass uses the extractor and line class. After that, it calls the 
result function to generate the users listing. It then uses the printer class to print the resulting .o file.

    @subsection Extractor
    The extractor uses the SymbolTable class to create a symbol table for the labels and literals as it parses through
the input program. It also uses the line class to generate the rest of the .o file as it goes through.

    @subsection FileArray
    FileArray keeps an array of ObjectParser instances with a simple ordering property to aid the linking process. The
    ObjectParser designated with the .MAIN attribute is first in the array to enable the linker to use the proper start
    of execution address.
    
    @subsection itos
    itos compliments design by providing easy access to a function that converts an integer to an std::string.

    @subsection Line
    The line class is responsible for converting each line into the corresponding text record in the .o file. It uses
the symbol table and word to do this.

    @subsection Linker
    The Linker combines multiple source files into one cohesive file. Linking does not modify any of the input files. 
The output file's .EXT and .ENT records are evaluated and defined and the produced file is ready for simulation.
The produced file is does not contain any .EXT or .ENT records and therefore is impractical to attempt to re-link
the produced file. The linker can support multiple relocatable source files but is unable to link a non-relocatable
file to another file.

    @subsection objparser Object Parser
    The ObjParser component handles all file input used to interface with the object files. The ObjParser component
is capable of reading on object file line entry at a time and returns an ObjectData struct token. The ObjectData.type
value is gaurenteed to be a null character if an invalid line entry was obtained.

    @subsection Symbol Table
    The symbol table keeps track of all of the symbols, and their values, by creating a map that maps each symbol to
its value using the word class.

    @subsection Printer
    The printer is responsible for printing the output generated by the main. It uses both word and line to do this.

    @subsection Word
    Word is used to keep track of memory values, and to convert between hexadecimal, binary, and decimal.

    @section codeconv Coding Conventions
    The following is a list of coding conventions to follow while maintaining this project.
    @arg Use comments to ensure that what you are doing is understandable.
    @arg Specifications are stored as low in the dependency tree as possible.
    @arg All interface .h files start with an i.
    @arg Do not use namespaces in header files, as they will apply to oter components as well, which could cause problems.
    @arg The first letter in each word in a method name is capitalized.  All other letters in the name are lowercase
    @arg All constants names are all-caps.
    @arg The header files define the classes and functions, the .cpp files execute them.
    @arg Private functions begin with a '_' character.
    @arg Always use braces while using if, while, and for commands, even if they are not needed for what you were using it for.

    @section depend Dependency Diagrams
    In the pages to come, there are several dependancy diagrams that display the interactions of files and components.  This section serves as a key for use in understanding their conventions.
    @arg A \b white box indicates a class. A \b marker in the lower right corner of the box indicates that the class has base classes that are hidden. If the box has a \b dashed border this indicates virtual inheritance. 
    @arg A \b solid arrow indicates public inheritance. 
    @arg A \b dashed arrow indicates protected inheritance. 
    @arg A \b dotted arrow indicates private inheritance.
 
    @section syntax Object Files
    @par
    The object files (usually file_name.o) that this simulator accepts
    are ASCII text files with the following structure:
    @arg One \ref header "Header Record"
    @arg Zero or more \ref text "Text Records"
    @arg One \ref end "End Record"
    
      @subsection header The Header Record
      @par
      The Header Record is a single line that prepares the system for
      storing the instructions to come.
      @par
      <b>Components</b>
      @arg A capital 'H'.  This designates that it is the Header Record.
      @arg A 6 character "segment name" (anything will do).
      @arg A 4-digit Hexadecimal value that corresponds to the "load address" of the program.
      Instructions can be written starting at this address.
      @arg A second 4-digit Hexadecimal value that denotes the length of the program-load segment
      (the size of memory into which the instructions will be loaded).
      @par
      <b>At a glance:</b> There is an 'H', a segment name, the first location where instructions can be written,
      and the number of memory locations for instructions.

      @subsection text Text Records
      @par
      Following the Header Record are several Text Records.
      Each Text Record corresponds to a single machine instruction
      and, like the header record, is on a single line.
      @par
      <b>Components</b>
      @arg A capital 'T'.  This designates that it is a Text Record.
      @arg A 4-digit hexadecimal value -- The location in memory at which the instruction will be stored.
      @arg A second 4-digit Hexadecimal value -- The encoding of the instruction to be stored.
      @par
      <b>At a glance:</b> There is a 'T', the location to store the instruction, and the instruction itself. 

      @subsection end The End Record
      @par
      The End Record is, as the name would suggest, the last line of the file.
      Its purpose is to denote the end of instructions to be written and to give an initial value for the PC.\n\n
      @par
      <b>Components</b>
      @arg The End Record begins with a capital 'E'.\n
      @arg Next, and last, a 4-digit hexadecimal value to be put into the PC.
      @par
      <b>At a glance:</b> There is an 'E', and the location in memory from which the first instruction should be fetched.

    @section Component Interaction
    The components described in this document are, for the most part,
    representative of the actual hardware components that would be present
    in the Wi-11 machine.  The following section describes these components
    and their interactions.  After that, a list of the \ref instructions "instructions"
    that the Wi-11 can execute (along with their encodings) completes the introduction
    to this simulator.  The rest of the document details the workings of each
    component and provides the reader with the knowledge necessary for
    altering, fixing, or even just understanding the code itself.

    @subsection components Components
    The Wi-11 Simulator uses 5 major components (for a visual, see \ref interactions).
    The main function, however, is only aware of one: Wi11.  It creates one Wi11
    object and uses it to parse object files, decode the instructions, and execute them.
    In order to perform these tasks it first creates Loader, Memory, Decoder, and Register
    objects.  The Register objects correspond to all those mentioned in the \ref intro,
    with the exception of the CCRs which are declared as their own entity.
    
    @note The Word class is not described below but nearly all transfers of data
    and mathematical operations are performed using (an) object(s) of this type.

    @subsubsection loading Loading
    The Loader object, receiving a pointer to memory and a filename, creates
    an ObjParser object (the fifth major component).  The ObjParser pulls the
    relevant data from the file and the Loader puts it into memory.  After some input
    by the user is accepted (assuming the simulator is in debug mode), the Wi11 is
    ready to begin executing instructions.

    @subsubsection execution Executing
    The Wi11 component executes instructions in a way very similar to how an
    actual Wi-11 machine would execute them.  It first has the Memory object
    return the instruction referenced by the current value of the PC.  After
    incrementing the PC, the raw instruction is given to the Decoder.  The Decoder
    returns an Instruction object that allows the Wi11 to call one of its many
    private functions that correspond (one-to-one) to each kind of instruction.
    This process is then repeated until either the HALT trap code is found or the
    user-specified instruction limit is reached.
    
    @image latex software_interaction.png "This diagram shows the awareness of each component with those operating below it." width=\textwidth

    @subsection instructions Wi-11 Instruction Set
    @par
    This section describes the format of each operation on the Wi-11.
    First there are necessary definitions and then the list of instructions.
    The name of each instruction is followed by the opcode; this includes any
    base conversions that may be necessary.  Then there is a list of
    the arguments to the command.  The opcode is the first four bits
    of the instruction; the list following the opcode delegates
    purpose to the following 12 bits.

    @subsubsection offset Offsets    
    Offsets to the PC are used by concatenating them with the PC.  Specifically, the first
    7 bits of the PC and the 9 bit offset form the new PC value.  This
    essentially separates memory into pages (the first seven bits of 
    the PC corresponding a "page number").

    @subsubsection indexes Indexes
    Indexes are used to specify a distance from a base value.
    Generally, there is a register holding an address.  The
    index is added to the base address as a positive quantity
    (zero-extended) in order to form a new address.  Because
    the index is zero-extended, the new address is always
    greater than the base address.

    @subsubsection inst Intructions
    @arg ADD (two registers), OPCODE: 0001 (1)
    <ul>
    <li>3 bits: The destination register</li>
    <li>3 bits: First source register</li>
    <li>1 bit:  A zero</li>
    <li>2 bits: Junk - not used.</li>
    <li>3 bits: Second source register</li>
    </ul>
    @arg ADD (register and immediate), OPCODE: 0001 (1)
    <ul>
    <li>3 bits: The destination register</li>
    <li>3 bits: The source register</li>
    <li>1 bit:  A one</li>
    <li>5 bits: An immediate value (2's complement)</li>
    </ul>
    @arg AND (two registers), OPCODE: 0101 (5)
    <ul>
    <li>3 bits: The destination register</li>
    <li>3 bits: First source register</li>
    <li>1 bit:  A zero</li>
    <li>2 bits: Junk - not used</li>
    <li>3 bits: Second source register</li>
    </ul>
    @arg AND (register and immediate), OPCODE: 0101 (5)
    <ul>
    <li>3 bits: The destination register</li>
    <li>3 bits: The source register</li>
    <li>1 bit:  A one</li>
    <li>5 bits: An immediate value (2's complement)</li>
    </ul>
    @arg BRx, OPCODE: 0000 (0)
    <ul>
    <li>1 bit:  Corresponds to the CCR's negative bit</li>
    <li>1 bit:  Corresponds to the CCR's zero bit</li>
    <li>1 bit:  Corresponds to the CCR's positive bit</li>
    <li>9-bits: An \ref offset "offset" to the PC</li>
    </ul>
    @arg DBUG, OPCODE: 1000 (8)
    <ul>
    <li>12 bits: Junk - not used</li>
    </ul>
    @arg JSR, OPCODE: 0100 (4)
    <ul>
    <li>1 bit:  The link bit (The PC is stored in R7 if this is set)</li>
    <li>2 bits: Junk - not used</li>
    <li>9 bits: An \ref offset "offset" to the PC</li>
    </ul>
    @arg JSRR, OPCODE: 1100 (12 - C)
    <ul>
    <li>1 bit:  The link bit (The PC is stored in R7 if this is set)</li>
    <li>2 bits: Junk - not used</li>
    <li>3 bits: A base register</li>
    <li>6 bits: An \ref indexes "index" to the base register</li>
    </ul>
    @arg LD, OPCODE: 0010 (2)
    <ul>
    <li>3 bits: The destination register</li>
    <li>9 bits: An \ref offset "offset" to the PC</li>
    </ul>
    @arg LDI, OPCODE: 1010 (10 - A)
    <ul>
    <li>3 bits: The destination register</li>
    <li>9 bits: An \ref offset "offset" to the PC</li>
    </ul>
    @arg LDR, OPCODE: 0110 (6)
    <ul>
    <li>3 bits: The destination register</li>
    <li>3 bits: A base register</li>
    <li>6 bits: An \ref indexes "index" to the base register</li>
    </ul>
    @arg LEA, OPCODE: 1110 (14 - E)
    <ul>
    <li>3 bits: The destination register</li>
    <li>9 bits: An \ref offset "offset" to the PC</li>
    </ul>
    @arg NOT, OPCODE: 1001 (9)
    <ul>
    <li>3 bits: The destination register</li>
    <li>3 bits: The source register</li>
    <li>6 bits: Junk - not used</li>
    </ul>
    @arg RET, OPCODE: 1101 (13 - D)
    <ul>
    <li>12 bits: Junk - not used</li>
    </ul>
    @arg ST, OPCODE: 0011 (3)
    <ul>
    <li>3 bits: The source register</li>
    <li>9 bits: An \ref offset "offset" to the PC</li>
    </ul>
    @arg STI, OPCODE: 1011 (11 - B)
    <ul>
    <li>3 bits: The source register</li>
    <li>9 bits: An \ref offset "offset" to the PC</li>
    </ul>
    @arg STR, OPCODE: 0111 (7)
    <ul>
    <li>3 bits: The source register</li>
    <li>3 bits: A base register</li>
    <li>6 bits: An \ref indexes "index" to the base register</li>
    </ul>
    @arg TRAP, OPCODE: 1111 (15 - F)
    <ul>
    <li>4 bits: Junk - not used</li>
    <li>8 bits: A trap vector</li>
    </ul>

    @subsubsection trap Traps
    Traps execute a system call.
    The details of these so-called "trap vectors" are below.
    @arg 0x21 - OUT
    <ul><li>Print the ASCII character in the last 8 bits of R0.</li></ul>
    @arg 0x22 - PUTS
    <ul><li>Print the string starting at the address in R0 and ending at a null character.</li></ul>
    @arg 0x23 - IN
    <ul><li>Prompt for and read an ASCII character. Put the result in R0.</li></ul>
    @arg 0x25 - HALT
    <ul><li>End execution.</li></ul>
    @arg 0x31 - OUTN
    <ul><li>Print the value in R0 as a decimal integer.</li></ul>
    @arg 0x33 - INN
    <ul><li>Prompt for and read a decimal number.  Put the result in R0.</li></ul>
    @arg 0x43 - RND
    <ul><li>Generate a random integer and store it in R0.</li></ul>

 
*/


