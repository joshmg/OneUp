// File: Memory.h
// Written by Joshua Green

#ifndef MEMORY_H
#define MEMORY_H

#include "iMemory.h"
#include "iWord.h"
#include "ResultCodes.h"
#include <map>
#include <vector>

class Word;

/*class WordCompare {
  bool operator() (const Word& a, const Word& b) const;
};*/

class Memory : iMemory {
  private:
    std::vector<Word*> _bounded_memory;
    std::vector<int> _segment_offsets;
    std::vector<int> _segment_lengths;

    std::map<int, Word> _unbounded_memory;

  public:
    virtual ~Memory();

    virtual Codes::RESULT Reserve(const iWord& initial_address, const iWord& length);

    virtual Word Load(const iWord&) const;

    virtual Codes::RESULT Store(const iWord& address, const Word& value);

    std::vector<Word[2]> GetUsedMemory() const;
};

#endif
