
#include "Parser.h"

//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//

namespace Parser {

std::map<char, int>& getBinopPrecedence() {
  // static variable, ensure that it is initialized once.
  static std::map<char, int> BinopPrecedence;
  return BinopPrecedence;
}

void initBinopPrecedence() {
  auto& BinopPrecedence = getBinopPrecedence();
  // Install standard binary operators.
  // 1 is lowest precedence.
  BinopPrecedence['<'] = 10;
  BinopPrecedence['+'] = 20;
  BinopPrecedence['-'] = 20;
  BinopPrecedence['*'] = 40; // highest.
}

std::unique_ptr<AST::ExprAST> LogError(const char *Str) {
  fprintf(stderr, "Error: %s\n", Str);
  exit(1);
  return nullptr;
}

std::unique_ptr<AST::PrototypeAST> LogErrorP(const char *Str) {
  LogError(Str);
  return nullptr;
}

}  // namespace Parser
