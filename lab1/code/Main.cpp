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
    Primary author of this document: Andrew Groot
    @section intro Introduction
    The "Wi-11 Machine" is a simple, 16-bit computer architecture.
    It has 8 general purpose registers, 3 condition code registers (CCRs),
    and a program counter (PC).  The Wi-11 Simulator is meant to emulate
    its execution, as well as present the user with information regarding the
    state of the machine after each instruction is executed.  However,
    before one can delve into the behind-the-scenes details, one must understand
    the environment.  In particular, an understanding of the object file syntax and
    the interactions between the components used in this project is necessary.

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
