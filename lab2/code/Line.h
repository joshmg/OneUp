/*! @file Line.h
    @author Andrew Groot
    @brief Definition of the private data for the "Line" class.
*/

#ifndef LINE_H
#define LINE_H

#include "iLine.h"
#include "iWord.h"
#include "ResultCodes.h"
#include <string>
#include <vector>

class Line : public iLine {
  private:
    std::string _label;
    std::string _inst;
    vector<std::string> _args;
    std::string _code;

    bool _IsWS(char ch) const;
    std::string _GetNext(std::string& str) const;
    void _SetBits(iWord& w, int value, int index);
    Codes::RESULT _IsReg(string reg);
    int _RegNum(string reg);
    Codes::RESULT _IsConstant(string constant, size);
    int _ReadConstant(string constant);

  public:
    Codes::RESULT ReadLine(std::string line);
    std::string Label() const;
    std::string Instruction() const;
    std::string operator[] (int index) const;
    int Size() const;
    std::string ToString() const;
    iWord& ToWord() const;
    bool IsComment() const;
};

#endif
