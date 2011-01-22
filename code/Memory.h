/*! @file Memory.h
    @author Joshua Green
    @author Andrew Groot
    @brief Definition of private data for the "Memory" class.
*/

#ifndef MEMORY_H
#define MEMORY_H

#include "iMemory.h"
#include "iWord.h"
#include "ResultCodes.h"
#include <map>
#include <vector>

class Word;

class Memory : iMemory {
  private:
    //! Provide constant time access to reserved memory.
    std::vector<Word*> _bounded_memory;
    //! Keep track of the initial addresses.
    std::vector<int> _segment_offsets;
    //! Keep track of the size of reserved memory.
    std::vector<int> _segment_lengths;

    //! Map out-of-bounds values to new Words.
    std::map<int, Word> _unbounded_memory;

  public:
    virtual ~Memory();

    virtual Codes::RESULT Reserve(const iWord& initial_address, const iWord& length);

    virtual Word Load(const iWord&) const;

    virtual Codes::RESULT Store(const iWord& address, const Word& value);

    std::vector<Word[2]> GetUsedMemory() const;
};

#endif
