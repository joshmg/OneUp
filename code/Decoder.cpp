// File: Decoder.cpp
// coded by Andrew Canale

#include "Decoder.h"
#include "iWord.h"
#include <vector>
#include "ResultCodes.h"


using namespace std;
using namespace Decoder;
using namespace Codes;

Instruction DecodeInstruction(const iWord& word) const {

	//retrieve all binary from iWord
	string str = ToStr(word);

	//new string to store opcode only
	string newstr;

	//loop to add filler zeroes to newstr
	int counter = 0;
	while ( counter < 12 ) {
		newstr += '0';
		counter++;
	}

	//copy opcode from str into newstr to isolate it
	newstr[12] = str[3];
	newstr[13] = str[2];
	newstr[14] = str[1];
	newstr[15] = str[0];

	//create an iWord with newstr, and then convert back into an int, for the switch below
	iWord opcode;
	opcode.FromStr(newstr);
	int opint = opcode.ToInt();

	//clear OP from newstr for use to hold the parameters in binary below
	newstr[12] = '0';
	newstr[13] = '0';
	newstr[14] = '0';
	newstr[15] = '0';

	//Determine opcode
	switch (opint)
	{
	case 1:
		// OP = ADD
		Instruction.type = ADD;

		//first parameter: DR. Isolate DR into newstr
		newstr[13] = str[4];
		newstr[14] = str[5];
		newstr[15] = str[6];

		//newstr to Word
		iWord param;
		param.FromStr(newstr);
		Instruction.data.push_back(param);

		//second parameter: SR1. Isolate SR1 into newstr
		newstr[13] = str[7];
		newstr[14] = str[8];
		newstr[15] = str[9];

		//newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

		//third parameter: extended OP (binary digit which differentiates between SR2 or imm5
		newstr[13] = '0';
		newstr[14] = '0';
		newstr[15] = str[10];

		//newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

		//distinguish between SR2 or imm5 as fourth parameter
		if( newstr[15] == '0') {
			// fourth parameter: xx
			newstr[14] = str[11];
			newstr[15] = str[12];

			// newstr to Word, and push onto Instruction.data
			param.FromStr(newstr);
			Instruction.data.push_back(param);

			// fifth parameter: SR2
			newstr[13] = str[13];
			newstr[14] = str[14];
			newstr[15] = str[15];

			// newstr to Word, and push onto Instruction.data
			param.FromStr(newstr);
			Instruction.data.push_back(param);

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
			Instruction.data.push_back(param);

		}
	case 5:
		// OP = AND
		Instruction.type = AND;

		// first parameter: DR. Isolate DR into newstr
		newstr[13] = str[4];
		newstr[14] = str[5];
		newstr[15] = str[6];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

		// second parameter: SR1
		newstr[13] = str[7];
		newstr[14] = str[8];
		newstr[15] = str[9];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

		// third parameter: extended OP which differentiates between the two ANDs.
		newstr[13] = '0';
		newstr[14] = '0';
		newstr[15] = str[15];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

		// differentiate between SR2 and imm5
		if( newstr[15] == '0') {
			// fourth parameter: xx
			newstr[14] = str[11];
			newstr[15] = str[12];

			// newstr to Word, and push onto Instruction.data
			param.FromStr(newstr);
			Instruction.data.push_back(param);

			// fifth parameter: SR2
			newstr[13] = str[13];
			newstr[14] = str[14];
			newstr[15] = str[15];

			// newstr to Word, and push onto Instruction.data
			param.FromStr(newstr);
			Instruction.data.push_back(param);
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
			Instruction.data.push_back(param);
		}

	case 0:
		// OP = BRx
		Instruction.type = BRx;

		// first parameter: n
		newstr[15] = str[4];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

		// second parameter: z
		newstr[15] = str[5];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

		// third parameter: p
		newstr[15] = str[6];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

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
		Instruction.data.push_back(param);

	case 8:
		// OP = DBUG
		Instruction.type = DBUG;

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
		param.FromStr(newstr);
		Instruction.data.push_back(param);


	case 4:
		// OP = JSR
		Instruction.type = JSR;

		// first parameter: L
		newstr[15] = str[4];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

		// second parameter: xx
		newstr[14] = str[5];
		newstr[15] = str[6];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

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
		Instruction.data.push_back(param);

	case 12:
		// OP = JSRR
		Instruction.type = JSRR;

		// first parameter: L
		newstr[15] = str[4];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

		// second parameter: xx
		newstr[14] = str[5];
		newstr[15] = str[6];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

		// third parameter: BaseR
		newstr[13] = str[7];
		newstr[14] = str[8];
		newstr[15] = str[9];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

		// fourth parameter: index6
		newstr[10] = str[10];
		newstr[11] = str[11];
		newstr[12] = str[12];
		newstr[13] = str[13];
		newstr[14] = str[14];
		newstr[15] = str[15];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

	case 2:
		// OP = LD
		Instruction.type = LD;

		// first parameter: DR
		newstr[13] = str[4];
		newstr[14] = str[5];
		newstr[15] = str[6];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

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
		Instruction.data.push_back(param);

	case 10:
		// OP = LDI
		Instruction.type = LDI;

		// first parameter: DR
		newstr[13] = str[4];
		newstr[14] = str[5];
		newstr[15] = str[6];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

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
		Instruction.data.push_back(param);


	case 6:
		// OP = LDR
		Instruction.type = LDR;

		// first parameter: DR
		newstr[13] = str[4];
		newstr[14] = str[5];
		newstr[15] = str[6];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

		// second parameter: BaseR
		newstr[13] = str[7];
		newstr[14] = str[8];
		newstr[15] = str[9];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

		// third parameter: index6
		newstr[10] = str[10];
		newstr[11] = str[11];
		newstr[12] = str[12];
		newstr[13] = str[13];
		newstr[14] = str[14];
		newstr[15] = str[15];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

	case 14:
		// OP = LEA
		Instruction.type = LEA;

		// first parameter: DR
		newstr[13] = str[4];
		newstr[14] = str[5];
		newstr[15] = str[6];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

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
		Instruction.data.push_back(param);


	case 9:
		// OP = NOT
		Instruction.type = NOT;

		// first parameter: DR
		newstr[13] = str[4];
		newstr[14] = str[5];
		newstr[15] = str[6];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

		// second parameter: SR
		newstr[13] = str[7];
		newstr[14] = str[8];
		newstr[15] = str[9];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

		// third parameter: xxxxxx
		newstr[10] = str[10];
		newstr[11] = str[11];
		newstr[12] = str[12];
		newstr[13] = str[13];
		newstr[14] = str[14];
		newstr[15] = str[15];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

	case 13:
		// OP = RET
		Instruction.type = RET;
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
		param.FromStr(newstr);
		Instruction.data.push_back(param);


	case 3:
		// OP = ST
		Instruction.type = ST;

		// first parameter: SR
		newstr[13] = str[4];
		newstr[14] = str[5];
		newstr[15] = str[6];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

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
		Instruction.data.push_back(param);

	case 11:
		// OP = STI
		Instruction.type = STI;

		// first parameter: SR
		newstr[13] = str[4];
		newstr[14] = str[5];
		newstr[15] = str[6];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

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
		Instruction.data.push_back(param);

	case 7:
		// OP = STR
		Instruction.type = STR;

		// first parameter: DR
		newstr[13] = str[4];
		newstr[14] = str[5];
		newstr[15] = str[6];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

		// second parameter: BaseR
		newstr[13] = str[7];
		newstr[14] = str[8];
		newstr[15] = str[9];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

		// third parameter: index6
		newstr[10] = str[10];
		newstr[11] = str[11];
		newstr[12] = str[12];
		newstr[13] = str[13];
		newstr[14] = str[14];
		newstr[15] = str[15];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);


	case 15:
		// OP = TRAP
		Instruction.type = TRAP;

		// first parameter: xxxx
		newstr[12] = str[4];
		newstr[13] = str[5];
		newstr[14] = str[6];
		newstr[15] = str[7];

		// newstr to Word, and push onto Instruction.data
		param.FromStr(newstr);
		Instruction.data.push_back(param);

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
		Instruction.data.push_back(param);		

	default:
		// not a valid opcode, so give an error message
		ResultDecoder(RESULT::Invalid_op);

	}
	}
