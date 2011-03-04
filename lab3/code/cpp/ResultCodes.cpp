// File:        ResultCodes.cpp
// Written by:  Andrew Canale
// Edited by:   Andrew Groot

#include "../h/ResultCodes.h"
#include <string>
using namespace std;
using namespace Codes;

string ResultDecoder::Find(const RESULT& result) const {

	// retrieve iterator from map of codes
	map<ERROR, string>::const_iterator it = _codes.find(result.msg);

	// check if retrieved iterator is the same as the map::end
	if ( it == _codes.end () ) {
		return "Error: Undefined Result Code Enumeration";
	} else {
    // prepend "Error" to info
		string tmp = "Error: " + result.info;
    if (result.info.length() > 0) {
      // if necessary, add another separator
      tmp += ": ";
    }
    // append error message
    tmp += (it -> second) + '\n';
    // return full message
    return tmp;
	}

}

