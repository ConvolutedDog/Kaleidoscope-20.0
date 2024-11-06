
#include "Parser.h"

//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//

namespace Parser {

void initBinopPrecedence() {
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
