
#include "Parser.h"

#include "llvm/IR/Constants.h" // llvm::ConstantFP
#include "llvm/IR/Type.h" // llvm::Type
#include "llvm/IR/DerivedTypes.h" // llvm::FunctionType
#include "llvm/IR/BasicBlock.h" // llvm::BasicBlock
#include "llvm/IR/Verifier.h" // llvm::verifyFunction

//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (aka Parse Tree)
//===----------------------------------------------------------------------===//

namespace AST {

std::unique_ptr<llvm::LLVMContext>& getTheContext() {
  static std::unique_ptr<llvm::LLVMContext> TheContext;
  return TheContext;
}
std::unique_ptr<llvm::IRBuilder<>>& getBuilder() {
  static std::unique_ptr<llvm::IRBuilder<>> Builder;
  return Builder;
}
std::unique_ptr<llvm::Module>& getTheModule() {
  static std::unique_ptr<llvm::Module> TheModule;
  return TheModule;
}
std::map<std::string, llvm::Value*>& getNamedValues() {
  static std::map<std::string, llvm::Value*> NamedValues;
  return NamedValues;
}

void InitializeModule() {
  auto& TheContext = getTheContext();
  auto& Builder = getBuilder();
  auto& TheModule = getTheModule();
  // Open a new context and module.
  TheContext = std::make_unique<llvm::LLVMContext>();
  TheModule = std::make_unique<llvm::Module>("my cool jit", *TheContext);

  // Create a new builder for the module.
  Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
}

llvm::Value *LogErrorV(const char *Str) {
  Parser::LogError(Str);
  return nullptr;
}

llvm::Value *NumberExprAST::codegen() {
  auto& TheContext = getTheContext();
  return llvm::ConstantFP::get(*TheContext, llvm::APFloat(Value));
}

llvm::Value *VariableExprAST::codegen() {
  auto& NamedValues = getNamedValues();
  llvm::Value *V = NamedValues[Name];
  if (!V)
    return LogErrorV("Unknown variable name");
  return V;
}


llvm::Value *BinaryExprAST::codegen() {
  llvm::Value *L = LHS->codegen();
  llvm::Value *R = RHS->codegen();
  if (!L || !R)
    return nullptr;

  auto& Builder = getBuilder();
  auto& TheContext = getTheContext();

  switch (Op) {
  case '+':
    return Builder->CreateFAdd(L, R, "addtmp");
  case '-':
    return Builder->CreateFSub(L, R, "subtmp");
  case '*':
    return Builder->CreateFMul(L, R, "multmp");
  case '<':
    L = Builder->CreateFCmpULT(L, R, "cmptmp");
    // Convert bool 0/1 to double 0.0 or 1.0
    return Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*TheContext), "booltmp");
  default:
    return LogErrorV("invalid binary operator");
  }
}

llvm::Value *CallExprAST::codegen() {
  auto& TheModule = getTheModule();
  auto& Builder = getBuilder();
  // Look up the name in the global module table.
  llvm::Function *CalleeF = TheModule->getFunction(Callee);
  if (!CalleeF)
    return LogErrorV("Unknown function referenced");

  // If argument mismatch error.
  if (CalleeF->arg_size() != Args.size())
    return LogErrorV("Incorrect # arguments passed");

  std::vector<llvm::Value *> ArgsV;
  for (unsigned i = 0, e = Args.size(); i != e; ++i) {
    ArgsV.push_back(Args[i]->codegen());
    if (!ArgsV.back())
      return nullptr;
  }

  return Builder->CreateCall(CalleeF, ArgsV, "calltmp");
}

llvm::Function *PrototypeAST::codegen() {
  auto& TheContext = getTheContext();
  auto& TheModule = getTheModule();
  // Make the function type:  double(double,double) etc.
  std::vector<llvm::Type *> Doubles(Args.size(), llvm::Type::getDoubleTy(*TheContext));
  llvm::FunctionType *FT =
      llvm::FunctionType::get(llvm::Type::getDoubleTy(*TheContext), Doubles, false);

  llvm::Function *F =
      llvm::Function::Create(FT, llvm::Function::ExternalLinkage, Name, TheModule.get());

  // Set names for all arguments.
  unsigned Idx = 0;
  for (auto &Arg : F->args())
    Arg.setName(Args[Idx++]);

  return F;
}

llvm::Function *FunctionAST::codegen() {
  auto& TheContext = getTheContext();
  auto& Builder = getBuilder();
  auto& TheModule = getTheModule();
  auto& NamedValues = getNamedValues();
  // First, check for an existing function from a previous 'extern' declaration.
  llvm::Function *TheFunction = TheModule->getFunction(Proto->getName());

  if (!TheFunction)
    TheFunction = Proto->codegen();

  if (!TheFunction)
    return nullptr;

  // Create a new basic block to start insertion into.
  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "entry", TheFunction);
  Builder->SetInsertPoint(BB);

  // Record the function arguments in the NamedValues map.
  NamedValues.clear();
  for (auto &Arg : TheFunction->args())
    NamedValues[std::string(Arg.getName())] = &Arg;

  if (llvm::Value *RetVal = Body->codegen()) {
    // Finish off the function.
    Builder->CreateRet(RetVal);

    // Validate the generated code, checking for consistency.
    llvm::verifyFunction(*TheFunction);

    return TheFunction;
  }

  // Error reading body, remove function.
  TheFunction->eraseFromParent();
  return nullptr;
}

} // namespace AST
