#pragma once
#include <string>
#include <stack>

class BinaryOperationNode;
class ExpressionElementNode;

struct BinaryExpressionBuilder {
  enum class Precedence {
    low = 0,
    mid,
    high
  };

  BinaryOperationNode *parse(const std::string &);
  std::stack<ExpressionElementNode *> getStack() const {
    return m_operandStack;
  }

  std::string m_asm;
 private:
  std::stack<char> m_operatorStack;
  std::stack<ExpressionElementNode *> m_operandStack;

  void processOperator(char op);
  void processRightParenthesis();
  void doBinary(char op);
  Precedence precedence(char op);
};
