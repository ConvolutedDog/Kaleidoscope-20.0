
#include "Parser.h"

//===----------------------------------------------------------------------===//
// Top-Level parsing
//===----------------------------------------------------------------------===//

static void HandleDefinition() {
  if (Parser::ParseDefinition()) {
    fprintf(stderr, "Parsed a function definition.\n");
  } else {
    // Skip token for error recovery.
    Parser::getNextToken();
  }
}

static void HandleExtern() {
  if (Parser::ParseExtern()) {
    fprintf(stderr, "Parsed an extern\n");
  } else {
    // Skip token for error recovery.
    Parser::getNextToken();
  }
}

static void HandleTopLevelExpression() {
  // Evaluate a top-level expression into an anonymous function.
  if (Parser::ParseTopLevelExpr()) {
    fprintf(stderr, "Parsed a top-level expr\n");
  } else {
    // Skip token for error recovery.
    Parser::getNextToken();
  }
}

/// top ::= definition | external | expression | ';'
static void MainLoop() {
  while (true) {
    // fprintf(stderr, "ready> ");
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

//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//

int main(int argv, char **argc) {
  // Install standard binary operators.
  Parser::initBinopPrecedence();

  Lexer::file = fopen(argc[1], "r");

  // Prime the first token.
  // fprintf(stderr, "ready> ");
  Parser::getNextToken();

  // Run the main "interpreter loop" now.
  MainLoop();

  fclose(Lexer::file);

  return 0;
}
