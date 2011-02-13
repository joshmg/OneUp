// File:        ResultCodes.cpp
// Written by:  Andrew Canale
// Edited by:   Andrew Groot

#include "ResultCodes.h"
#include <string>

using namespace std;
using namespace Codes;

string ResultDecoder::Find(const RESULT& result) const {

	// retrieve iterator from map of codes
	map<RESULT, string>::const_iterator it = _codes.find(result);

	// check if retrieved iterator is the same as the map::end
	if( it == _codes.end () ) {
		return "Error: Undefined Result Code Enumeration";
	} else {
		string tmp = "Error: " + result.info;

    if (result.info.length() > 0) {
      tmp += ": ";
    }

    tmp += (it -> second);

    return tmp
	}

}

