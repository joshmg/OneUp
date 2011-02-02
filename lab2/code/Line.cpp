#include "Line.h"
#include <string>
using namespace std;

string Line::_GetNext(string& str) {
  while (str[0] == ' ' || str[0] == '\t' || str[0] == '\n') {
    str = str.substr(1);
  }

  string next = "";
  while (str[0] != ' ' && str[0] != '\t' && str[0] != '\n') {
    next += str[0];
    str = str.substr(1);
  }
  
  return next;
}
