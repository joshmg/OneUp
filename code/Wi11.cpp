// File: Wi11.cpp
// Written by Joshua Green

#include "Wi11.h"
#include "Register.h"
#include "Memory.h"
#include "iDecoder.h"
#include "Decoder.h"
#include "Loader.h"
#include "ResultCodes.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace Decoder_Directory;
using namespace Codes;
using namespace std;

REGISTER_ID Wi11::_Word2RegisterID(const Word& word) const {
  switch (word.ToInt()) {
    case 0: { return R0; } break;
    case 1: { return R1; } break;
    case 2: { return R2; } break;
    case 3: { return R3; } break;
    case 4: { return R4; } break;
    case 5: { return R5; } break;
    case 6: { return R6; } break;
    case 7: { return R7; } break;
    default: { return PC; } break;
  }
}

iRegister& Wi11::_GetRegister(const REGISTER_ID& reg_id) {
  switch (reg_id) {
    case R0: { return _R0; } break;
    case R1: { return _R1; } break;
    case R2: { return _R2; } break;
    case R3: { return _R3; } break;
    case R4: { return _R4; } break;
    case R5: { return _R5; } break;
    case R6: { return _R6; } break;
    case R7: { return _R7; } break;
    case PC: { return _PC; } break;
    default: { 
      cout << "Errorus Register Selection!" << endl;
      return _PC;
    } break;
  }
}

void Wi11::_UpdateCCR(int value) {
  if (value < 0) { _CCR.n = 1, _CCR.z = 0, _CCR.p = 0; }
  else if (value > 0) { _CCR.n = 0, _CCR.z = 0, _CCR.p = 1; }
  else { _CCR.n = 0, _CCR.z = 1, _CCR.p = 0; }
}

RESULT Wi11::_Add(const REGISTER_ID& DR, const REGISTER_ID& SR1, const REGISTER_ID& SR2) {
  _GetRegister(DR) = _GetRegister(SR1) + _GetRegister(SR2);
  _UpdateCCR(_GetRegister(DR).GetValue().ToInt());
  return SUCCESS;
}

RESULT Wi11::_Add(const REGISTER_ID& DR, const REGISTER_ID& SR1, const iWord& immediate) {
  _GetRegister(DR) = _GetRegister(SR1).GetValue() + immediate;
  _UpdateCCR(_GetRegister(DR).GetValue().ToInt());
  return SUCCESS;
}

RESULT Wi11::_And(const REGISTER_ID& DR, const REGISTER_ID& SR1, const REGISTER_ID& SR2) {
  _GetRegister(DR) = _GetRegister(SR1).And(_GetRegister(SR2));
  _UpdateCCR(_GetRegister(DR).GetValue().ToInt());
  return SUCCESS;
}

RESULT Wi11::_And(const REGISTER_ID& DR, const REGISTER_ID& SR1, const iWord& immediate) {
  Register temp_reg(immediate);
  _GetRegister(DR) = _GetRegister(SR1).And(temp_reg);
  _UpdateCCR(_GetRegister(DR).GetValue().ToInt());
  return SUCCESS;
}

RESULT Wi11::_Branch(const iWord& address) {
  Word new_address(_PC.GetValue());
  for (int i=0;i<9;i++) new_address.SetBit(i, address[i]);
  _PC = new_address;

  return SUCCESS;
}

RESULT Wi11::_Debug() {
  DisplayRegisters();
  DisplayMemory();

  return SUCCESS;
}

RESULT Wi11::_JSR(const iWord& address, bool link) {
  Word new_address(_PC.GetValue());
  for (int i=0;i<9;i++) new_address.SetBit(i, address[i]);

  if (link) _R7 = _PC;
  _PC = new_address;

  return SUCCESS;
}

RESULT Wi11::_JSRR(const REGISTER_ID& baseR, const iWord& address, bool link) {
  if (link) _R7 = _PC;
  Register temp_reg(address);

  _PC = _GetRegister(baseR) + temp_reg;

  return SUCCESS;
}

RESULT Wi11::_Load(const REGISTER_ID& DR, const iWord& address) {
  Word new_address(_PC.GetValue());
  for (int i=0;i<9;i++) new_address.SetBit(i, address[i]);

  _GetRegister(DR) = _memory.Load(new_address);
  _UpdateCCR(_GetRegister(DR).GetValue().ToInt());

  return SUCCESS;
}

RESULT Wi11::_LoadI(const REGISTER_ID& DR, const iWord& address) {
  Word new_address(_PC.GetValue());
  for (int i=0;i<9;i++) new_address.SetBit(i, address[i]);

  _GetRegister(DR) = _memory.Load(_memory.Load(new_address));
  _UpdateCCR(_GetRegister(DR).GetValue().ToInt());

  return SUCCESS;
}

RESULT Wi11::_LoadR(const REGISTER_ID& DR, const REGISTER_ID& baseR, const iWord& address) {
  Register temp_rep(address);
  _GetRegister(DR) = _memory.Load((_GetRegister(baseR) + temp_rep).GetValue());
  _UpdateCCR(_GetRegister(DR).GetValue().ToInt());

  return SUCCESS;
}

RESULT Wi11::_LoadEA(const REGISTER_ID& DR, const iWord& address) {
  Word new_address(_PC.GetValue());
  for (int i=0;i<9;i++) new_address.SetBit(i, address[i]);

  _GetRegister(DR) = _memory.Load(new_address);
  _UpdateCCR(_GetRegister(DR).GetValue().ToInt());

  return SUCCESS;
}

RESULT Wi11::_Not(const REGISTER_ID& DR, const REGISTER_ID& SR) {
  _GetRegister(DR) = _GetRegister(SR);
  _GetRegister(DR).Not();
  _UpdateCCR(_GetRegister(DR).GetValue().ToInt());
  return SUCCESS;
}

RESULT Wi11::_Ret() {
  _PC = _R7;
  return SUCCESS;
}

RESULT Wi11::_Store(const REGISTER_ID& SR, const iWord& address) {
  Word new_address(_PC.GetValue());
  for (int i=0;i<9;i++) new_address.SetBit(i, address[i]);

  return _memory.Store(new_address, _GetRegister(SR).GetValue());
}

RESULT Wi11::_STI(const REGISTER_ID& SR, const iWord& address) {
  Word new_address(_PC.GetValue());
  for (int i=0;i<9;i++) new_address.SetBit(i, address[i]);

  return _memory.Store(_memory.Load(new_address), _GetRegister(SR).GetValue());
}

RESULT Wi11::_STR(const REGISTER_ID& SR, const REGISTER_ID& baseR, const iWord& address) {
  return _memory.Store(_GetRegister(baseR).GetValue() + address, _GetRegister(SR).GetValue());
}

RESULT Wi11::_Trap(const iWord& code) {
  switch (code.ToInt()) {
    case 0x21: {
      Word character = _R0.GetValue();
      for (int i=8;i<16;i++) character.SetBit(i, 0);
      cout << (char)character.ToInt();
    } break;
    case 0x22: {
      Word temp_R0 = _GetRegister(R0).GetValue();
      Word data = _memory.Load(temp_R0);
      while (data.ToInt() != 0) cout << (char)_memory.Load(++temp_R0).ToInt();
    } break;
    case 0x23: {
      char c;
      cout << "Enter Character: ";
      cin >> c;

      Word new_R0_value;
      new_R0_value.FromInt((int)c);
      _R0 = new_R0_value;

      _UpdateCCR(_R0.GetValue().ToInt());
    } break;
    case 0x25: {
      return HALT;
    } break;
    case 0x31: {
      cout << _R0.GetValue().ToInt();
    } break;
    case 0x33: {
      int c;
      cout << "Enter Integer: ";
      cin >> c;

      Word new_R0_value;
      new_R0_value.FromInt(c);
      _R0 = new_R0_value;

      _UpdateCCR(_R0.GetValue().ToInt());
    } break;
    case 0x43: {
      srand(time(NULL));
      Word new_R0_value;
      new_R0_value.FromInt(rand()%65536); // 2^16 = 65,536
      _R0 = new_R0_value;

      _UpdateCCR(_R0.GetValue().ToInt());
    } break;
    default: {
      return INVALID_TRAP_CODE;
    } break;
  }
  return SUCCESS;
}

Wi11::Wi11() : _loader(&_memory) { }

bool Wi11::LoadObj(const char* filename) {
  Word initial_pc_value;
  if (_loader.Load(filename, initial_pc_value) == SUCCESS) {
    _PC = initial_pc_value;
    return true;
  }
  else return false;
}

void Wi11::DisplayMemory() const {
  vector< vector<Word> > memory_addresses = _memory.GetUsedMemory();

  cout << "Wi11 Machine Memory" << endl;
  for (int i=0;i<memory_addresses.size();i++) {
    int begin_address = memory_addresses[i][0].ToInt();
    int end_address = memory_addresses[i][1].ToInt();
    while (begin_address <= end_address) {
      Word temp_address;
      temp_address.FromInt(begin_address);
      cout << "[" << begin_address << "] = " << _memory.Load(temp_address).ToStr() << endl;
    }
  }
  cout << endl;
}

void Wi11::DisplayRegisters() const {
  cout << "Wi11 Register Values" << endl << endl;
  cout << "PC = " << _PC.GetValue().ToStr() << endl;
  cout << "R0 = " << _R0.GetValue().ToStr() << '\t' << "R1 = " << _R1.GetValue().ToStr() << endl;
  cout << "R2 = " << _R2.GetValue().ToStr() << '\t' << "R1 = " << _R3.GetValue().ToStr() << endl;
  cout << "R4 = " << _R4.GetValue().ToStr() << '\t' << "R1 = " << _R5.GetValue().ToStr() << endl;
  cout << "R6 = " << _R6.GetValue().ToStr() << '\t' << "R1 = " << _R7.GetValue().ToStr() << endl;
  cout << endl;
}

bool Wi11::ExecuteNext(bool verbose) {
  Instruction instruction = _decoder.DecodeInstruction(_PC.GetValue());
  _PC++;

  switch (instruction.type) {
    case ADD: {
      if (instruction.data.size() == 5) {
        if (verbose) cout << "Add(5) operation: ";
        RESULT result = _Add(_Word2RegisterID(instruction.data[0]), _Word2RegisterID(instruction.data[1]), _Word2RegisterID(instruction.data[4]));
        if (result == SUCCESS) {
          if (verbose) cout << _result_decoder.Find(result) << endl;
          return true;
        }
        else {
          if (verbose) cout << _result_decoder.Find(result) << endl;
          return false;
        }
      }
      else {
        if (verbose) cout << "Add(4) operation: ";
        RESULT result = _Add(_Word2RegisterID(instruction.data[0]), _Word2RegisterID(instruction.data[1]), instruction.data[4]);
        if (verbose) cout << _result_decoder.Find(result) << endl;

        if (result == SUCCESS) return true;
        else return false;
      }
    } break;

    case AND: {
      if (instruction.data.size() == 5) {
        if (verbose) cout << "And(5) operation: ";
        RESULT result = _And(_Word2RegisterID(instruction.data[0]), _Word2RegisterID(instruction.data[1]), _Word2RegisterID(instruction.data[4]));
        if (verbose) cout << _result_decoder.Find(result) << endl;

        if (result == SUCCESS) return true;
        else return false;
      }
      else {
        if (verbose) cout << "And(4) operation: ";
        RESULT result = _Add(_Word2RegisterID(instruction.data[0]), _Word2RegisterID(instruction.data[1]), instruction.data[4]);
        if (verbose) cout << _result_decoder.Find(result) << endl;
        
        if (result == SUCCESS) return true;
        else return false;
      }
    } break;

    case BRx: {
      if (verbose) cout << "Branch operation: ";
      if ((instruction.data[0].ToInt() > 0 && _CCR.n) || 
          (instruction.data[1].ToInt() > 0 && _CCR.z) || 
          (instruction.data[2].ToInt() > 0 && _CCR.p)) {
        RESULT result = _Branch(instruction.data[3]);
        if (verbose) cout << _result_decoder.Find(result) << endl;

        if (result == SUCCESS) return true;
        else return false;
      }
      else {
        if (verbose) cout << "Branch ignored." << endl;
        return true;
      }
    } break;

    case DBUG: {
      if (verbose) cout << "Debug operation: " << endl;

      RESULT result = _Debug();

      if (result == SUCCESS) return true;
      else return false;
    } break;

    case JSR: {
      if (verbose) cout << "JSR operation: ";
      RESULT result = _JSR(instruction.data[2], instruction.data[0].ToInt()>0);
      if (verbose) cout << _result_decoder.Find(result) << endl;

      if (result == SUCCESS) return true;
      else return false;
    } break;

    case JSRR: {
      if (verbose) cout << "JSRR operation: ";
      RESULT result = _JSRR(_Word2RegisterID(instruction.data[2]), instruction.data[3], instruction.data[0].ToInt()>0);
      if (verbose) cout << _result_decoder.Find(result) << endl;

      if (result == SUCCESS) return true;
      else return false;
    } break;

    case LD: {
      if (verbose) cout << "LD operation: ";
      RESULT result = _Load(_Word2RegisterID(instruction.data[0]), instruction.data[1]);
      if (verbose) cout << _result_decoder.Find(result) << endl;

      if (result == SUCCESS) return true;
      else return false;
    } break;

    case LDI: {
      if (verbose) cout << "LDI operation: ";
      RESULT result = _LoadI(_Word2RegisterID(instruction.data[0]), instruction.data[1]);
      if (verbose) cout << _result_decoder.Find(result) << endl;

      if (result == SUCCESS) return true;
      else return false;
    } break;

    case LDR: {
      if (verbose) cout << "LDR operation: ";
      RESULT result = _LoadR(_Word2RegisterID(instruction.data[0]), _Word2RegisterID(instruction.data[1]), instruction.data[2]);
      if (verbose) cout << _result_decoder.Find(result) << endl;

      if (result == SUCCESS) return true;
      else return false;
    } break;

    case LEA: {
      if (verbose) cout << "LEA operation: ";
      RESULT result = _LoadEA(_Word2RegisterID(instruction.data[0]), instruction.data[1]);
      if (verbose) cout << _result_decoder.Find(result) << endl;

      if (result == SUCCESS) return true;
      else return false;
    } break;

    case NOT: {
      if (verbose) cout << "NOT operation: ";
      RESULT result = _Not(_Word2RegisterID(instruction.data[0]), _Word2RegisterID(instruction.data[1]));
      if (verbose) cout << _result_decoder.Find(result) << endl;

      if (result == SUCCESS) return true;
      else return false;
    } break;

    case RET: {
      if (verbose) cout << "RET operation: ";
      RESULT result = _Ret();
      if (verbose) cout << _result_decoder.Find(result) << endl;

      if (result == SUCCESS) return true;
      else return false;
    } break;

    case ST: {
      if (verbose) cout << "ST operation: ";
      RESULT result = _Store(_Word2RegisterID(instruction.data[0]), instruction.data[1]);
      if (verbose) cout << _result_decoder.Find(result) << endl;

      if (result == SUCCESS) return true;
      else return false;
    } break;

    case STI: {
      if (verbose) cout << "STI operation: ";
      RESULT result = _STI(_Word2RegisterID(instruction.data[0]), instruction.data[1]);
      if (verbose) cout << _result_decoder.Find(result) << endl;

      if (result == SUCCESS) return true;
      else return false;
    } break;

    case STR: {
      if (verbose) cout << "STR operation: ";
      RESULT result = _STR(_Word2RegisterID(instruction.data[0]), _Word2RegisterID(instruction.data[1]), instruction.data[2]);
      if (verbose) cout << _result_decoder.Find(result) << endl;

      if (result == SUCCESS) return true;
      else return false;
    } break;

    case TRAP: {
      if (verbose) cout << "TRAP operation: ";
      RESULT result = _Trap(instruction.data[1]);
      if (verbose) cout << _result_decoder.Find(result) << endl;

      if (result == SUCCESS) return true;
      else return false;
    } break;

    default: {
      if (verbose) cout << "Invalid Operation." << endl;
      return false;
    } break;

  }
}
