// File: Decoder.cpp
// coded by Andrew Canale

#include "../h/Decoder.h"
#include "../h/Word.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;
using namespace Decoder_Directory;

Instruction Decoder::DecodeInstruction(const iWord& word) const {
  Instruction return_instruction;

  //retrieve all binary from Word
  string str = word.ToStr();

  //new string to store opcode only
  string newstr(16, '0');

  //copy opcode from str into newstr to isolate it
  newstr[12] = str[0];
  newstr[13] = str[1];
  newstr[14] = str[2];
  newstr[15] = str[3];

  //create an Word with newstr, and then convert back into an int, for the switch below
  Word opcode;
  opcode.FromStr(newstr);
  int opint = opcode.ToInt();

  //clear OP from newstr for use to hold the parameters in binary below
  newstr[12] = '0';
  newstr[13] = '0';
  newstr[14] = '0';
  newstr[15] = '0';

  //Determine opcode
  switch (opint) {
    case 1: {
      // OP = ADD
      return_instruction.type = ADD;

      //first parameter: DR. Isolate DR into newstr
      newstr[13] = str[4];
      newstr[14] = str[5];
      newstr[15] = str[6];

      //newstr to Word
      Word param;
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      //second parameter: SR1. Isolate SR1 into newstr
      newstr[13] = str[7];
      newstr[14] = str[8];
      newstr[15] = str[9];

      //newstr to Word, and push onto Instruction.data
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      //third parameter: extended OP (binary digit which differentiates between SR2 or imm5
      newstr[13] = '0';
      newstr[14] = '0';
      newstr[15] = str[10];

      //newstr to Word, and push onto Instruction.data
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      //distinguish between SR2 or imm5 as fourth parameter
      if( newstr[15] == '0') {
        // fourth parameter: xx
        newstr[14] = str[11];
        newstr[15] = str[12];

        // newstr to Word, and push onto Instruction.data
        param.FromStr(newstr);
        return_instruction.data.push_back(param);

        // fifth parameter: SR2
        newstr[13] = str[13];
        newstr[14] = str[14];
        newstr[15] = str[15];

        // newstr to Word, and push onto Instruction.data
        param.FromStr(newstr);
        return_instruction.data.push_back(param);

      }
      else {
        // fourth parameter: imm5
        newstr[11] = str[11];
        newstr[12] = str[12];
        newstr[13] = str[13];
        newstr[14] = str[14];
        newstr[15] = str[15];

        // newstr to Word, and push onto the end of Instruction.data
        param.FromStr(newstr);

        // sign extend the immediate value
        bool extend = param[4];
        for (int i=5;i<16;i++) param.SetBit(i, extend);

        return_instruction.data.push_back(param);
      }
    } break;

    case 5: {
      // OP = AND
      return_instruction.type = AND;

      // first parameter: DR. Isolate DR into newstr
      newstr[13] = str[4];
      newstr[14] = str[5];
      newstr[15] = str[6];

      // newstr to Word, and push onto Instruction.data
      Word param;
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      // second parameter: SR1
      newstr[13] = str[7];
      newstr[14] = str[8];
      newstr[15] = str[9];

      // newstr to Word, and push onto Instruction.data
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      // third parameter: extended OP which differentiates between the two ANDs.
      newstr[13] = '0';
      newstr[14] = '0';
      newstr[15] = str[10];

      // newstr to Word, and push onto Instruction.data
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      // differentiate between SR2 and imm5
      if( newstr[15] == '0') {
        // fourth parameter: xx
        newstr[14] = str[11];
        newstr[15] = str[12];

        // newstr to Word, and push onto Instruction.data
        param.FromStr(newstr);
        return_instruction.data.push_back(param);

        // fifth parameter: SR2
        newstr[13] = str[13];
        newstr[14] = str[14];
        newstr[15] = str[15];

        // newstr to Word, and push onto Instruction.data
        param.FromStr(newstr);
        return_instruction.data.push_back(param);
      }
      else {
        // fourth parameter: imm5
        newstr[11] = str[11];
        newstr[12] = str[12];
        newstr[13] = str[13];
        newstr[14] = str[14];
        newstr[15] = str[15];

        // newstr to Word, and push onto the end of Instruction.data
        param.FromStr(newstr);

        // sign extend the immediate value
        bool extend = param[4];
        for (int i=5;i<16;i++) param.SetBit(i, extend);

        return_instruction.data.push_back(param);
      }
    } break;

    case 0: {
      // OP = BRx
      return_instruction.type = BRx;

      // first parameter: n
      newstr[15] = str[4];

      // newstr to Word, and push onto Instruction.data
      Word param;
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      // second parameter: z
      newstr[15] = str[5];

      // newstr to Word, and push onto Instruction.data
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      // third parameter: p
      newstr[15] = str[6];

      // newstr to Word, and push onto Instruction.data
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      //fourth parameter: pgoffset9
      newstr[7] = str[7];
      newstr[8] = str[8];
      newstr[9] = str[9];
      newstr[10] = str[10];
      newstr[11] = str[11];
      newstr[12] = str[12];
      newstr[13] = str[13];
      newstr[14] = str[14];
      newstr[15] = str[15];

      // newstr to Word, and push onto Instruction.data
      param.FromStr(newstr);
      return_instruction.data.push_back(param);
    } break;

    case 8: {
      // OP = DBUG
      return_instruction.type = DBUG;

      // first (and only) parameter

      newstr[4] = str[4];
      newstr[5] = str[5];
      newstr[6] = str[6];
      newstr[7] = str[7];
      newstr[8] = str[8];
      newstr[9] = str[9];
      newstr[10] = str[10];
      newstr[11] = str[11];
      newstr[12] = str[12];
      newstr[13] = str[13];
      newstr[14] = str[14];
      newstr[15] = str[15];

      // newstr to Word, and push onto Instruction.data
      Word param;
      param.FromStr(newstr);
      return_instruction.data.push_back(param);
    } break;

    case 4: {
      // OP = JSR
      return_instruction.type = JSR;

      // first parameter: L
      newstr[15] = str[4];

      // newstr to Word, and push onto Instruction.data
      Word param;
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      // second parameter: xx
      newstr[14] = str[5];
      newstr[15] = str[6];

      // newstr to Word, and push onto Instruction.data
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      // third parameter: pgoffset9
      newstr[7] = str[7];
      newstr[8] = str[8];
      newstr[9] = str[9];
      newstr[10] = str[10];
      newstr[11] = str[11];
      newstr[12] = str[12];
      newstr[13] = str[13];
      newstr[14] = str[14];
      newstr[15] = str[15];

      // newstr to Word, and push onto Instruction.data
      param.FromStr(newstr);
      return_instruction.data.push_back(param);
    } break;

    case 12: {
      // OP = JSRR
      return_instruction.type = JSRR;

      // first parameter: L
      newstr[15] = str[4];

      // newstr to Word, and push onto Instruction.data
      Word param;
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      // second parameter: xx
      newstr[14] = str[5];
      newstr[15] = str[6];

      // newstr to Word, and push onto Instruction.data
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      // third parameter: BaseR
      newstr[13] = str[7];
      newstr[14] = str[8];
      newstr[15] = str[9];

      // newstr to Word, and push onto Instruction.data
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      // fourth parameter: index6
      newstr[10] = str[10];
      newstr[11] = str[11];
      newstr[12] = str[12];
      newstr[13] = str[13];
      newstr[14] = str[14];
      newstr[15] = str[15];

      // newstr to Word, and push onto Instruction.data
      param.FromStr(newstr);
      return_instruction.data.push_back(param);
    } break;

    case 2: {
      // OP = LD
      return_instruction.type = LD;

      // first parameter: DR
      newstr[13] = str[4];
      newstr[14] = str[5];
      newstr[15] = str[6];

      // newstr to Word, and push onto Instruction.data
      Word param;
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      // second parameter: pgoffset9
      newstr[7] = str[7];
      newstr[8] = str[8];
      newstr[9] = str[9];
      newstr[10] = str[10];
      newstr[11] = str[11];
      newstr[12] = str[12];
      newstr[13] = str[13];
      newstr[14] = str[14];
      newstr[15] = str[15];

      // newstr to Word, and push onto Instruction.data
      param.FromStr(newstr);
      return_instruction.data.push_back(param);
    } break;

    case 10: {
      // OP = LDI
      return_instruction.type = LDI;

      // first parameter: DR
      newstr[13] = str[4];
      newstr[14] = str[5];
      newstr[15] = str[6];

      // newstr to Word, and push onto Instruction.data
      Word param;
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      // second parameter: pgoffset9
      newstr[7] = str[7];
      newstr[8] = str[8];
      newstr[9] = str[9];
      newstr[10] = str[10];
      newstr[11] = str[11];
      newstr[12] = str[12];
      newstr[13] = str[13];
      newstr[14] = str[14];
      newstr[15] = str[15];

      // newstr to Word, and push onto Instruction.data
      param.FromStr(newstr);
      return_instruction.data.push_back(param);
    } break;

    case 6: {
      // OP = LDR
      return_instruction.type = LDR;

      // first parameter: DR
      newstr[13] = str[4];
      newstr[14] = str[5];
      newstr[15] = str[6];

      // newstr to Word, and push onto Instruction.data
      Word param;
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      // second parameter: BaseR
      newstr[13] = str[7];
      newstr[14] = str[8];
      newstr[15] = str[9];

      // newstr to Word, and push onto Instruction.data
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      // third parameter: index6
      newstr[10] = str[10];
      newstr[11] = str[11];
      newstr[12] = str[12];
      newstr[13] = str[13];
      newstr[14] = str[14];
      newstr[15] = str[15];

      // newstr to Word, and push onto Instruction.data
      param.FromStr(newstr);
      return_instruction.data.push_back(param);
    } break;

    case 14: {
      // OP = LEA
      return_instruction.type = LEA;

      // first parameter: DR
      newstr[13] = str[4];
      newstr[14] = str[5];
      newstr[15] = str[6];

      // newstr to Word, and push onto Instruction.data
      Word param;
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      // second parameter: pgoffset9
      newstr[7] = str[7];
      newstr[8] = str[8];
      newstr[9] = str[9];
      newstr[10] = str[10];
      newstr[11] = str[11];
      newstr[12] = str[12];
      newstr[13] = str[13];
      newstr[14] = str[14];
      newstr[15] = str[15];

      // newstr to Word, and push onto Instruction.data
      param.FromStr(newstr);
      return_instruction.data.push_back(param);
    } break;

    case 9: {
      // OP = NOT
      return_instruction.type = NOT;

      // first parameter: DR
      newstr[13] = str[4];
      newstr[14] = str[5];
      newstr[15] = str[6];

      // newstr to Word, and push onto Instruction.data
      Word param;
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      // second parameter: SR
      newstr[13] = str[7];
      newstr[14] = str[8];
      newstr[15] = str[9];

      // newstr to Word, and push onto Instruction.data
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      // third parameter: xxxxxx
      newstr[10] = str[10];
      newstr[11] = str[11];
      newstr[12] = str[12];
      newstr[13] = str[13];
      newstr[14] = str[14];
      newstr[15] = str[15];

      // newstr to Word, and push onto Instruction.data
      param.FromStr(newstr);
      return_instruction.data.push_back(param);
    } break;

    case 13: {
      // OP = RET
      return_instruction.type = RET;
      // first (and only) parameter
      newstr[4] = str[4];
      newstr[5] = str[5];
      newstr[6] = str[6];
      newstr[7] = str[7];
      newstr[8] = str[8];
      newstr[9] = str[9];
      newstr[10] = str[10];
      newstr[11] = str[11];
      newstr[12] = str[12];
      newstr[13] = str[13];
      newstr[14] = str[14];
      newstr[15] = str[15];

      // newstr to Word, and push onto Instruction.data
      Word param;
      param.FromStr(newstr);
      return_instruction.data.push_back(param);
    } break;

    case 3: {
      // OP = ST
      return_instruction.type = ST;

      // first parameter: SR
      newstr[13] = str[4];
      newstr[14] = str[5];
      newstr[15] = str[6];

      // newstr to Word, and push onto Instruction.data
      Word param;
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      // second parameter: pgoffset9
      newstr[7] = str[7];
      newstr[8] = str[8];
      newstr[9] = str[9];
      newstr[10] = str[10];
      newstr[11] = str[11];
      newstr[12] = str[12];
      newstr[13] = str[13];
      newstr[14] = str[14];
      newstr[15] = str[15];

      // newstr to Word, and push onto Instruction.data
      param.FromStr(newstr);
      return_instruction.data.push_back(param);
    } break;

    case 11: {
      // OP = STI
      return_instruction.type = STI;

      // first parameter: SR
      newstr[13] = str[4];
      newstr[14] = str[5];
      newstr[15] = str[6];

      // newstr to Word, and push onto Instruction.data
      Word param;
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      // second parameter: pgoffset9
      newstr[7] = str[7];
      newstr[8] = str[8];
      newstr[9] = str[9];
      newstr[10] = str[10];
      newstr[11] = str[11];
      newstr[12] = str[12];
      newstr[13] = str[13];
      newstr[14] = str[14];
      newstr[15] = str[15];

      // newstr to Word, and push onto Instruction.data
      param.FromStr(newstr);
      return_instruction.data.push_back(param);
    } break;

    case 7: {
      // OP = STR
      return_instruction.type = STR;

      // first parameter: DR
      newstr[13] = str[4];
      newstr[14] = str[5];
      newstr[15] = str[6];

      // newstr to Word, and push onto Instruction.data
      Word param;
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      // second parameter: BaseR
      newstr[13] = str[7];
      newstr[14] = str[8];
      newstr[15] = str[9];

      // newstr to Word, and push onto Instruction.data
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      // third parameter: index6
      newstr[10] = str[10];
      newstr[11] = str[11];
      newstr[12] = str[12];
      newstr[13] = str[13];
      newstr[14] = str[14];
      newstr[15] = str[15];

      // newstr to Word, and push onto Instruction.data
      param.FromStr(newstr);
      return_instruction.data.push_back(param);
    } break;

    case 15: {
      // OP = TRAP
      return_instruction.type = TRAP;

      // first parameter: xxxx
      newstr[12] = str[4];
      newstr[13] = str[5];
      newstr[14] = str[6];
      newstr[15] = str[7];

      // newstr to Word, and push onto Instruction.data
      Word param;
      param.FromStr(newstr);
      return_instruction.data.push_back(param);

      // second parameter: trapvect8
      newstr[8] = str[8];
      newstr[9] = str[9];
      newstr[10] = str[10];
      newstr[11] = str[11];
      newstr[12] = str[12];
      newstr[13] = str[13];
      newstr[14] = str[14];
      newstr[15] = str[15];
      
      // newstr to Word, and push onto Instruction.data
      param.FromStr(newstr);
      return_instruction.data.push_back(param);    
    } break;

    default: {
      return_instruction.type = ERROR;
    }
  }

  return return_instruction;
}
