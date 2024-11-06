
#ifndef LEXER_H
#define LEXER_H

#include <string> // c_str
#include <cctype> // isspace, isdigit, isalpha, isascii
#include <cstdlib> // strtod
#include <cstdio> // EOF, fgetc
#include <iostream> // cerr
#include <stdio.h> // FILE

//===----------------------------------------------------------------------===//
// Lexer
//===----------------------------------------------------------------------===//

namespace Lexer {

enum Token : int {
  // end of file
  tok_eof = -1,

  // commands
  tok_def = -2,
  tok_extern = -3,

  // primary
  tok_identifier = -4,
  tok_number = -5,

  // error
  tok_error = -6,
};

static std::string IdentifierStr;
static std::string NumStr;
static double NumVal;

static FILE *file;

int getchar(FILE *file);

static int getToken(FILE *file) {
  static int LastChar = ' ';
// std::cout << "0" << " " << bool(isspace(LastChar)) << std::endl;
  // Skip any whitespace.
  while (isspace(LastChar)) {
    // std::cout << "#" << static_cast<char>(LastChar) << "#" << std::endl;
    LastChar = getchar(file);
    // std::cout << "$" << static_cast<char>(LastChar) << "$" << std::endl;
  }
// std::cout << "1" << std::endl;
  if (isalpha(LastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
    IdentifierStr = LastChar;
    while (isalnum((LastChar = getchar(file))))
      IdentifierStr += LastChar;

    if (IdentifierStr == "def")
      return tok_def;
    if (IdentifierStr == "extern")
      return tok_extern;
    return tok_identifier;
  }
// std::cout << "2" << std::endl;
  if (isdigit(LastChar) || LastChar == '.') { // Number: [0-9.]+
    std::string NumStr;
    do {
      NumStr += LastChar;
      LastChar = getchar(file);
    } while (isdigit(LastChar) || LastChar == '.');

    NumVal = strtod(NumStr.c_str(), nullptr);
    return tok_number;
  }
// std::cout << "3" << std::endl;
  if (LastChar == '#') {
    // Comment until end of line.
    do
      LastChar = getchar(file);
    while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

    if (LastChar != EOF)
      return getToken(file);
  }
// std::cout << "4" << std::endl;
  // Check for end of file.  Don't eat the EOF.
  if (LastChar == EOF)
    return tok_eof;
// std::cout << "5" << std::endl;
  // Otherwise, just return the character as its ascii value.
  int ThisChar = LastChar;
  LastChar = getchar(file);
  return ThisChar;
}
}; // namespace Lexer

#endif // #define LEXER_H
