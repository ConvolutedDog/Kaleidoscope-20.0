
#include "AST.h"

#include "llvm/IR/Constants.h" // llvm::ConstantFP

//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (aka Parse Tree)
//===----------------------------------------------------------------------===//

namespace AST {

  llvm::Value *NumberExprAST::codegen() {
    return llvm::ConstantFP::get(*TheContext, llvm::APFloat(Value));
  }

} // namespace AST
