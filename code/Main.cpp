#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " object_file\n";
    return 1;
  }
  
  ifstream file;
  file.exceptions(ifstream::failbit | ifstream::badbit | ifstream::eofbit);
  // throw exception __ if __:
  // 1) ifstream::failbit, logical error
  // 2) ifstream::badbit, i/o error
  // 3) ifstream::eofbit, unexpected eof

  try {
    // command-line argument 1 should be the name
    // of the object file to take as input
    file.open(argv[1]);
  } catch (ifstream::failure e) {
    cerr << "Error: File " << argv[1] << " not found.\n";
    return 1;
  }

  // parse file

  file.close();

  // start simulator

  // return
  return 0;
}
