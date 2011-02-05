/*! @file Line.h
    @author Andrew Groot
    @brief Definition of the private data for the "Line" class.
*/

#ifndef LINE_H
#define LINE_H

#include "iLine.h"
#include "iWord.h"
#include <string>
#include <vector>

class Line : public iLine {
  private:
    std::string _label;
    std::string _inst;
    // OP_CODE _inst;
    vector<std::string> _args;
    std::string _code;

    std::string _GetNext(std::string& str);

  public:
    Line(std::string line);

    ReadLine(std::string line);
    std::string Label() const;
    std::string Instruction() const;
    operator[] (int index) const;
    int Size() const;
    std::string ToString() const;
    iWord& ToWord() const;
};

#endif
