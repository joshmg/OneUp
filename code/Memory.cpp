// File: Memory.cpp
// Written by Joshua Green

#include "iMemory.h"
#include "Memory.h"
#include "Word.h"
#include "ResultCodes.h"
#include <map>

using namespace std;
using namespace Codes;

//bool WordCompare::operator() (const Word& a, const Word& b) const { return (a.toInt() < b.toInt()); }

Memory::~Memory() {
  for (int i=0;i<_bounded_memory.size();i++) delete[] _bounded_memory[i];
}

RESULT Memory::Reserve(const iWord& initial_address, const iWord& length) {
  _segment_offsets.push_back(initial_address.toInt());
  _segment_lengths.push_back(length.toInt());

  _bounded_memory.push_back(new Word[_segment_lengths.back()]);

  return SUCCESS;
}

Word Memory::Load(const iWord& address) const {
  for (int i=0;i<_bounded_memory.size();i++) {
    if (_segment_offsets[i] <= address.toInt() && address.toInt() < _segment_offsets[i]+_segment_lengths[i]) { // the address is located within a section of bounded memory
      return _bounded_memory[i][address.toInt()-_segment_offsets[i]]; // location found
    }
  }

  // at this point, the address is not found within any of the bounded memory segments, load the data from unbounded segment map
  if (_unbounded_memory.count(address.toInt()) == 0) return Word(); // the address does not exist anywhere in current memory :. returning new word
  else return (_unbounded_memory.find(address.toInt()))->second; // the address exists in unbound memory
}

RESULT Memory::Store(const iWord& address, const Word& value) {
  for (int i=0;i<_bounded_memory.size();i++) {
    if (_segment_offsets[i] <= address.toInt() && address.toInt() < _segment_offsets[i]+_segment_lengths[i]) { // the address is located within a section of bounded memory
      _bounded_memory[i][address.toInt()-_segment_offsets[i]] = value; // store the value in memory
      return SUCCESS; // location found, data stored
    }
  }

  // at this point, the address is not found within any of the bounded memory segments
  _unbounded_memory[address.toInt()] = value; // store the value in memory
  return SUCCESS;
}

bool vect_cmp(vector<Word[2]> i, vector<Word[2]> j) {
  return (i[0][0] < j[0][0]);
}

vector<Word[2]> GetUsedMemory() const {
  vector<Word[2]> value;
  for (int i=0;i<_bounded_memory;i++) {
    Word begin_to_end[2];
    begin_to_end[0] = _segment_offsets[i];
    begin_to_end[1] = _segment_offsets[i]+_segment_lengths[i]-1;

    value.push_back(begin_to_end);
  }

  map<int, Word>::const_iterator it = _unbounded_memory.begin();
  while (it != _unbounded_memory.end()) {
    Word begin_to_end[2];
    begin_to_end[0].FromInt(it->first);
    begin_to_end[0].FromInt(it->first);

    value.push_back(begin_to_end);
    it++;
  }

  // sort the addresses:
  sort(value.begin(), value.end(), &vect_cmp);

  return value;
}
