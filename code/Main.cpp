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
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " object_file\n";
    return 1;
  }
  
  ifstream file;
  file.exceptions(ifstream::failbit | ifstream::badbit | ifstream::eofbit);
  // throw exception __ if __:
  // 1) ifstream::failbit, logical error
  // 2) ifstream::badbit, i/o error
  // 3) ifstream::eofbit, unexpected eof

  try {
    // command-line argument 1 should be the name
    // of the object file to take as input
    file.open(argv[1]);
  } catch (ifstream::failure e) {
    cerr << "Error: File " << argv[1] << " not found.\n";
    return 1;
  }

  // parse file

  file.close();

  // start simulator

  // return
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
    the environment.  In particular, an understanding the object file syntax and
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
      <b>At a glance:</b> There is a T, the location to store the instruction, and the instruction itself. 

      @subsection end The End Record
      @par
      The End Record is, as the name would suggest, the last line of the line.
      Its purpose is to denote the end of instructions to be written and to give an initial value for the PC.\n\n
      @par
      <b>Components</b>
      @arg The End Record begins with a capital 'E'.\n
      @arg Next, and last, a 4-digit hexadecimal value to be put into the PC.
      @par
      <b>At a glance:</b> There is an E, and the location in memory from which the first instruction should be fetched.

    @section Component Interactions
    @image html software_interaction.png 
*/
