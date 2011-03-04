/*! @file Decoder.h
    @author Andrew Canale
    @author Andrew Groot
    @brief Definition of the private data for the "Decoder" class. (none)
*/

#ifndef DECODER_H
#define DECODER_H

#include "iDecoder.h"
#include "iWord.h"
#include <vector>

class Word;

//! Implements iDecoder.
class Decoder {
  private:

  public:
    Instruction DecodeInstruction(const iWord&) const;


};



#endif
