#pragma once
#include <memory>
#include <string>
#include <stack>

class BinaryOperationNode;
class ExpressionElementNode;
class NumericElementNode;

struct BinaryExpressionBuilder {
  enum class Precedence {
    low = 0,
    mid,
    high
  };

  std::shared_ptr<ExpressionElementNode> parse(const std::string &);
  std::stack<std::shared_ptr<ExpressionElementNode>> getStack() const {
    return m_operandStack;
  }

  std::string m_asm;
 private:
  std::stack<char> m_operatorStack;
  std::stack<std::shared_ptr<ExpressionElementNode>> m_operandStack;

  void processOperator(char op);
  void processRightParenthesis();
  void doBinary(char op);
  Precedence precedence(char op);
};
