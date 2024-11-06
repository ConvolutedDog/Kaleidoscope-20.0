
#include <string>
#include <memory> // unique_ptr
#include <vector>
#include <map>

#include "llvm/IR/Value.h" // llvm::Value
#include "llvm/IR/LLVMContext.h" // llvm::LLVMContext
#include "llvm/IR/IRBuilder.h" // llvm::IRBuilder
#include "llvm/IR/Module.h" // llvm::Module

//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (aka Parse Tree)
//===----------------------------------------------------------------------===//

namespace AST {

static std::unique_ptr<llvm::LLVMContext> TheContext;
static std::unique_ptr<llvm::IRBuilder<>> Builder;
static std::unique_ptr<llvm::Module> TheModule;
static std::map<std::string, llvm::Value*> NamedValues;

/// ExprAST - Base class for all expression nodes.
class ExprAST {
public:
  virtual ~ExprAST() = default;
  // virtual llvm::Value *codegen() = 0;
};

/// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST {
public:
  NumberExprAST(const double &Value) : Value(Value) {}
  virtual ~NumberExprAST() = default;
  // llvm::Value *codegen() override;
  llvm::Value *codegen();
private:
  double Value;
};

/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST {
public:
  VariableExprAST(const std::string &Name) : Name(Name) {}
private:
  std::string Name;
};

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST {
public:
  BinaryExprAST(const char &Op, 
                std::unique_ptr<ExprAST> LHS, 
                std::unique_ptr<ExprAST> RHS)
    : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
private:
  char Op;
  std::unique_ptr<ExprAST> LHS, RHS;
};

/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {
public:
  CallExprAST(const std::string &Callee,
              std::vector<std::unique_ptr<ExprAST>> Args)
    : Callee(Callee), Args(std::move(Args)) {}
private:
  std::string Callee;
  std::vector<std::unique_ptr<ExprAST>> Args;
};

/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
class PrototypeAST : public ExprAST {
public:
  PrototypeAST(const std::string &Name, std::vector<std::string> Args)
    : Name(Name), Args(Args) {}
  const std::string &getName() const { return this->Name; }
private:
  std::string Name;
  std::vector<std::string> Args;
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST : public ExprAST {
public:
  FunctionAST(std::unique_ptr<PrototypeAST> Proto,
              std::unique_ptr<ExprAST> Body)
    : Proto(std::move(Proto)), Body(std::move(Body)) {}
private:
  std::unique_ptr<PrototypeAST> Proto;
  std::unique_ptr<ExprAST> Body;
};

} // namespace AST
