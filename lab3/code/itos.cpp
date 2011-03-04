
#include "itos.h"
#include <string>
using namespace std;

string itos(long int number) {
  if (number == 0) return "0";
  string reversed, temp;

  if (number < 0) {
    temp = "-";
    number *= -1;
  }

  while (number > 0) {
    reversed += number%10+48;
    number/=10;
  }
  int length = reversed.length();
  for (int i=0;i<length;i++) temp += reversed[length-i-1];
  return temp;
}

//=================================================================//

string itoshex(long int number) {
  if (number == 0) {
    return "x0";
  }

  string reverse;

  while (number != 0) {
    short digit = number & 0xF;
    number /= 0x10;

    char temp;
    if (digit < 10) {
      temp = '0' + digit;
    } else {
      // 'A' = 10, so offset by 10
      temp = 'A' + (digit - 10);
    }

    reverse.push_back(temp);
  }

  string hex;
  // get rid of leading zeroes
  int end = reverse.length()-1;
  while (reverse[end] == 0) {
    end--;
  }
  // reverse string to get hex
  while (end >= 0) {
    hex.push_back(reverse[end--]);
  }

  return "x" + hex;
}
