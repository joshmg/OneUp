/*! @file itos.h
    @author Joshua Green
    @brief Utility function: int to std::string
*/

#include <string>

std::string itos(long int number) {
  if (number == 0) return "0";
  std::string reversed, temp;

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
