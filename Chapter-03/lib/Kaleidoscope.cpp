
#include "Driver.h"

#include "llvm/Support/raw_ostream.h" // llvm::errs

//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//

int main(int argv, char **argc) {
  // Install standard binary operators.
  Parser::initBinopPrecedence();
  // Make the module, which holds all the code.
  AST::InitializeModule();

  Lexer::file = fopen(argc[1], "r");
  if (Lexer::file == NULL) {
    std::cerr << "Open file " << argc[1] << " failed." << std::endl;
    exit(1);
  }

  Parser::getNextToken();

  // Run the main "interpreter loop" now.
  Driver::MainLoop();

  fclose(Lexer::file);

  // Print out all of the generated code.
  // TODO: the printed info is not completed.
  // AST::getTheModule()->print(llvm::errs(), nullptr);

  return 0;
}
