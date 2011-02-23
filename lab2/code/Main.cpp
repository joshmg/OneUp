// File:        Main.cpp
// Written by:  Andrew Groot

#include "Extractor.h"
#include "SymbolTable.h"
#include "Printer.h"
#include "ResultCodes.h"
#include <cstdlib>
#include <string>
#include <iostream>
using namespace std;
using namespace Codes;

int main (int argc, char* argv[]) {
  bool TRAP_LABELS;
  string infile, outfile;
  int symbol_length = SYMBOL_TABLE_MAX_SIZE;
  TRAP_LABELS = false;

  if (argc > 2) {
    // get the file names
    infile = argv[1];
    outfile = argv[2];
  } else {
    // Wrong number of arguments.
    cout << "Error: Usage: " << argv[0] << " <input_file> <output_file> [-t] [-s<max_size>]\n";
    return 1; // usage error
  }

  for (int i=3; i<argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] == 's') {
      string size_flag(argv[i]);
      string ascii_size = size_flag.substr(2);
      symbol_length = atoi(ascii_size.c_str());
    }
    else if (argv[i][0] == '-' && argv[i][1] == 't') {
      TRAP_LABELS = true;
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

///////////////////////////////////////
//*** Project-wide documentation ***///
///////////////////////////////////////
/*! @mainpage Introduction
    Primary author: Logan Coulson\n
    Edited by: Andrew Canale\n
    Doxygen: Andrew Groot

    @section intro Introduction
    The "Wi-11 Machine" is a theoretical 16-bit computer architecture. It has 8 general purpose registers, 3 condition code registers (CCRs), and a program counter (PC). The Wi-11 Simulator emulates the execution of this Wi-11 processor.
However, programming for the Wi-11 Simulator is difficult as it only accepts encoded object files as input.  To aid
the user, this assembler translates assembly language into usable object files for the Wi-11 Simulator. Before
alterations are made to the programming of this assembler, a basic understanding of the environment is required. In
particular, one needs to understand the input and output formats, how the components interact, and the programming
conventions used.

    @section input Input
    @subsection format Format
    The input file provided to the assembler must have limitations on the maximum size of the symbol table, the .o
file, and the literal table. When the user runs the program, they may add a -s# to the end of their command. If the
user does, that number is the maximum number of symbols allowed. The maximum number of literals allowed is one half
the maximum number of symbols specified by the user. Additionally, the number of source records/lines in the object
file produced by the assembler is limited to twice the number of symbols specified by the user. The default is two
thousand source records, one thousand symbols, and five hundred literals. Each line of the input file must conform to
the following structural organization:

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
    This assembler must support many different functions. A basic understanding of the different functions is necessary.
DR(Destination Register) is the location where the final value is stored. SR(Source Register) is the source of the
numbers that are being manipulated by the operation. imm5(Immediate) is 5 bits, and it is sign extended to 16 bits
when used. pgoffset9(Page Offset Nine) is used to form the last 7 bits for a memory access. The first 7 bits come from
the PC(Program Counter). index6(Index Six) is used as a six bit number that is added to a register(BaseR) to determine
an offset. In addition to the operations provided by the simulator, there are several pseudo ops that the assembler
provides. A pseudo op is recognizable by the period ‘.’ at the beginning of the command.

    @subsubsection orig .ORIG
    .ORIG x0-xFFFF\n
.ORIG must be the first non-comment record in the source program. The operand indicates the absolute address the
program is to be stored in. If the operand is absent, the program may be stored anywhere. If there is no operand, the
entire program must fit in one page of memory(512  16-bit words). It is also required to have a 6 character label,
which is used to generate the header record in the .o file.

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
and P's are applicable here.  If the character is present, the accompanying bit is set.

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
    The main function uses the extractor class to generate the symbol tables. This is the first pass over the program.
This assembler uses a 2-pass method. The second pass is made immediately after the first pass, and it is also done in
the extractor class, using the line class. After that, it calls the result function to generate the users listing. It
then uses the printer class to print the resulting .o file.

    @subsection Extractor
    The extractor uses the SymbolTable class to create a symbol table for the labels and literals as it parses through
the input program. It also uses the line class to generate the rest of the .o file as it goes through.

    @subsection Line
    The line class is responsible for converting each line into the corresponding text record in the .o file. It uses
the symbol table and word to do this.

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
 
*/
