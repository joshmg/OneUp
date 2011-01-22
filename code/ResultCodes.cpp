// File: ResultCodes.cpp
// coded by Andrew Canale

#include "ResultCodes.h"
#include <string>

using namespace std;
using namespace Codes;

string ResultDecoder::Find () const {

	// retrieve iterator from map of codes
	map<Result, string>::iterator it = _codes.find(Result);

	// check if retrieved iterator is the same as the map::end
	if( it == _codes.end () ) {
		return "Error: Undefined Result Code.";
	}
	else {
		return it -> second;
	}

}
