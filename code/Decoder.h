// File: Decoder.h
// Written by Andrew Canale

#ifndef DECODER_H
#define DECODER_H

#include "iWord.h"
#include <vector>

class Word;

class Decoder {
  private:

  public:
    Instruction DecodeInstruction(const iWord&) const;


};



#endif
