// File: Memory.cpp
// Written by Joshua Green

#include "iMemory.h"
#include "Memory.h"
#include "Word.h"
#include "ResultCodes.h"
#include <map>
#include <algorithm>

using namespace std;
using namespace Codes;

//bool WordCompare::operator() (const Word& a, const Word& b) const { return (a.ToInt() < b.ToInt()); }

Memory::~Memory() {
  for (int i=0;i<_bounded_memory.size();i++) delete[] _bounded_memory[i];
}

RESULT Memory::Reserve(const iWord& initial_address, const iWord& length) {
  _segment_offsets.push_back(initial_address.ToInt());
  _segment_lengths.push_back(length.ToInt());

  _bounded_memory.push_back(new Word[_segment_lengths.back()]);

  return SUCCESS;
}

Word Memory::Load(const iWord& address) const {
  for (int i=0;i<_bounded_memory.size();i++) {
    if (_segment_offsets[i] <= address.ToInt() && address.ToInt() < _segment_offsets[i]+_segment_lengths[i]) { // the address is located within a section of bounded memory
      return _bounded_memory[i][address.ToInt()-_segment_offsets[i]]; // location found
    }
  }

  // at this point, the address is not found within any of the bounded memory segments, load the data from unbounded segment map
  if (_unbounded_memory.count(address.ToInt()) == 0) return Word(); // the address does not exist anywhere in current memory :. returning new word
  else return (_unbounded_memory.find(address.ToInt()))->second; // the address exists in unbound memory
}

RESULT Memory::Store(const iWord& address, const Word& value) {
  for (int i=0;i<_bounded_memory.size();i++) {
    if (_segment_offsets[i] <= address.ToInt() && address.ToInt() < _segment_offsets[i]+_segment_lengths[i]) { // the address is located within a section of bounded memory
      _bounded_memory[i][address.ToInt()-_segment_offsets[i]] = value; // store the value in memory
      return SUCCESS; // location found, data stored
    }
  }

  // at this point, the address is not found within any of the bounded memory segments
  _unbounded_memory[address.ToInt()] = value; // store the value in memory
  return SUCCESS;
}

bool vect_cmp(vector<Word> i, vector<Word> j) {
  return (i[0].ToInt() < j[0].ToInt());
}

vector<vector<Word>> Memory::GetUsedMemory() const {
  vector<vector<Word>> value;
  for (int i=0;i<_bounded_memory.size();i++) {
    vector<Word> begin_to_end;
    begin_to_end.push_back(Word());
    begin_to_end.push_back(Word());

    begin_to_end[0].FromInt(_segment_offsets[i]);
    begin_to_end[1].FromInt(_segment_offsets[i]+_segment_lengths[i]-1);

    value.push_back(begin_to_end);
  }

  map<int, Word>::const_iterator it = _unbounded_memory.begin();
  while (it != _unbounded_memory.end()) {
    vector<Word> begin_to_end;
    begin_to_end.push_back(Word());
    begin_to_end.push_back(Word());

    begin_to_end[0].FromInt(it->first);
    begin_to_end[0].FromInt(it->first);

    value.push_back(begin_to_end);
    it++;
  }

  // sort the addresses:
  sort(value.begin(), value.end(), &vect_cmp);
  return value;
}
