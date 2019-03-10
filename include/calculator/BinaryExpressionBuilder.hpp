#pragma once
#include <memory>
#include <string>
#include <stack>

class BinaryOperationNode;
class ExpressionElementNode;
class NumericElementNode;

struct BinaryExpressionBuilder {
  enum class Precedence {
    LOW = 0,
    MID,
    HIGH
  };

  std::shared_ptr<ExpressionElementNode> parse(const std::string &);
  std::stack<std::shared_ptr<ExpressionElementNode>> getStack() const {
    return m_operandStack;
  }

  std::string m_asm;
 private:
  std::stack<std::string> m_operatorStack;
  std::stack<std::shared_ptr<ExpressionElementNode>> m_operandStack;

  void processOperator(std::string op);
  void processRightParenthesis();
  void doBinary(std::string op);
  Precedence precedence(std::string op);
};
