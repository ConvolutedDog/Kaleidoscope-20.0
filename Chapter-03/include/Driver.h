
#ifndef DRIVER_H
#define DRIVER_H

#include "Parser.h"

//===----------------------------------------------------------------------===//
// Top-Level parsing and JIT Driver
//===----------------------------------------------------------------------===//

namespace Driver {

static void HandleDefinition() {
  if (auto FnAST = Parser::ParseDefinition()) {
    if (auto *FnIR = FnAST->codegen()) {
      fprintf(stderr, "\033[0;32mRead function definition:\033[0m\n");
      FnIR->print(llvm::errs());
      fprintf(stderr, "\n");
    }
  } else {
    // Skip token for error recovery.
    Parser::getNextToken();
  }
}

static void HandleExtern() {
  if (auto ProtoAST = Parser::ParseExtern()) {
    if (auto *FnIR = ProtoAST->codegen()) {
      fprintf(stderr, "\033[0;32mRead extern:\033[0m\n");
      FnIR->print(llvm::errs());
      fprintf(stderr, "\n");
    }
  } else {
    // Skip token for error recovery.
    Parser::getNextToken();
  }
}

static void HandleTopLevelExpression() {
  // Evaluate a top-level expression into an anonymous function.
  if (auto FnAST = Parser::ParseTopLevelExpr()) {
    if (auto *FnIR = FnAST->codegen()) {
      fprintf(stderr, "\033[0;32mRead top-level expression:\033[0m\n");
      FnIR->print(llvm::errs());
      fprintf(stderr, "\n");

      // Remove the anonymous expression.
      FnIR->eraseFromParent();
    }
  } else {
    // Skip token for error recovery.
    Parser::getNextToken();
  }
}

/// top ::= definition | external | expression | ';'
static void MainLoop() {
  while (true) {
    switch (Parser::CurTok) {
    case Lexer::tok_eof:
      return;
    case ';': // ignore top-level semicolons.
      Parser::getNextToken();
      break;
    case Lexer::tok_def:
      HandleDefinition();
      break;
    case Lexer::tok_extern:
      HandleExtern();
      break;
    default:
      HandleTopLevelExpression();
      break;
    }
  }
}

}

#endif // #define DRIVER_H