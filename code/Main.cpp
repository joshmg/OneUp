// File: Main.cpp
// Written by Joshua Green
//            Andrew Groot

#include "Wi11.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

/*! @brief The main function parses the object file and uses the Wi11 class to simulate the Wi11 machine.
    @author Andrew Groot
    
    Parses command-line argument (object file name) and open's the object file.
    After reading the data it, in association with the myriad components designed to help it,
    executes the hardware logic of the Wi-11 machine, prompting the user for his/her desired
    mode of execution before each step.
*/
int main(int argc, char* argv[]) {
  bool DEBUG = false;
  vector<string> obj_files;
  for (int i=1;i<argc;i++) {
    if (argv[i][0] == '-' && (argv[i][1] == 'd' || argv[i][1] == 'D')) DEBUG = true;
    else obj_files.push_back(string(argv[i]));
  }

  if (argc < 2 || (DEBUG && argc < 3)) {
    // prompt users for object files
    string ask_again = "Yes";
    while (ask_again[0] == 'y' || ask_again[0] == 'Y') {
      cout << "Enter Obj Filename: ";
      string filename;
      getline(cin, filename);
      obj_files.push_back(filename);
      cout << "Add another object filename? (y/N) ";
      getline(cin, ask_again);
    }
  }
  
  Wi11 simulator;

  if (DEBUG) cout << "Loading object files... ";
  for (int i=0;i<obj_files.size();i++) {
    if (!simulator.LoadObj(obj_files[i].c_str())) {
      cout << "Aborting." << endl;
      return 1;
    }
  }
  if (DEBUG) cout << "done." << endl;

  if (DEBUG) {
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

      while (simulator.ExecuteNext(true) && iterations != 0) {
        --iterations;
      }
    
      cout << endl;
      simulator.DisplayRegisters();
      simulator.DisplayMemory();
    }
    else if (mode[0] == 'S' || mode[0] == 's') {
      cout << "[Step Mode]" << endl;

      while (simulator.ExecuteNext(true) && iterations != 0) {
        --iterations;
        cout << "Continue? (Y/n) ";
        string temp;
        getline(cin, temp);

        if (!(temp[0] != 'n' && temp[0] != 'N')) {
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

      while (simulator.ExecuteNext(false) && iterations != 0) {
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
/*! @mainpage
    @section intro Introduction
    The "Wi-11 Machine" is a simple, 16-bit computer architecture.
    It has 8 general purpose registers, 3 condition code registers (CCRs),
    and a program counter (PC).  This software package is meant to emulate
    its execution, as well as present the user with information regarding the
    state of the machine after each instruction is executed.  However,
    before one can delve into the behind-the-scenes details, one must understand
    the environment.  In particular, an understanding of the object file syntax and
    the interactions between the components used in this project is necessary.

    @section syntax Object Files
    @par
    The object files (ususally file_name.o) that this simulator accepts
    are ascii text files with the following structure:
    @arg One \ref header "Header Record"
    @arg Several \ref text "Text Records"
    @arg One \ref end "End Record"
    
      @subsection header The Header Record
      @par
      The Header Record is a single line that prepares the system for
      the storing the instructions to come.
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
      and the number of memory locations for instuctions.

      @subsection text Text Records
      @par
      Following the Header Record are serveral Text Records.
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
      The End Record is, as the name would suggest, the last line of the line.
      Its purpose is to denote the end of instructions to be written and to give an initial value for the PC.\n\n
      @par
      <b>Components</b>
      @arg The End Record begins with a capital 'E'.\n
      @arg Next, and last, a 4-digit hexadecimal value to be put into the PC.
      @par
      <b>At a glance:</b> There is an 'E', and the location in memory from which the first instruction should be fetched.

    @section Component Interaction
    @image latex software_interaction.png width=\textwidth
    @subsection components Components

    @subsection instructions Wi11 Instruction Set

*/
