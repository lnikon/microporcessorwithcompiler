#include <sstream>

#include "BinaryExpressionBuilder.hpp"
#include "NumericElementNode.hpp"
#include "BinaryOperationNode.hpp"

BinaryOperationNode *BinaryExpressionBuilder::parse(
  const std::string &expression) {

  std::stringstream exprStream(expression);
  char token;

  while (exprStream >> token) {
    switch (token) {
    case '+':
    case '-':
    case '*':
    case '/':
      processOperator(token);
      break;

    case ')':
      processRightParenthesis();
      break;

    case '(':
      m_operatorStack.push(token);
      break;

    default:
      exprStream.putback(token);
      double number = .0;

      exprStream >> number;
      NumericElementNode *pNode = new NumericElementNode(number);
      m_operandStack.push(pNode);
      continue;
    }
  }

  while (!m_operatorStack.empty()) {
    doBinary(m_operatorStack.top());
    m_operatorStack.pop();
  }

  if (m_operandStack.size() != 1) {
    throw "not well formed expression\n";
  }

  ExpressionElementNode *pNode = m_operandStack.top();
  return dynamic_cast<BinaryOperationNode *>(pNode);
}

void BinaryExpressionBuilder::processOperator(char op) {
  Precedence opPrecedence = precedence(op);

  while ((!m_operatorStack.empty()) &&
         (static_cast<int>(opPrecedence) <=
          static_cast<int>(precedence(m_operatorStack.top())))) {
    doBinary(m_operatorStack.top());
    m_operatorStack.pop();
  }

  m_operatorStack.push(op);
}

void BinaryExpressionBuilder::processRightParenthesis() {
  while (!m_operatorStack.empty() && m_operatorStack.top() != '(') {
    doBinary(m_operatorStack.top());
    m_operatorStack.pop();
  }

  m_operatorStack.pop();
}

void BinaryExpressionBuilder::doBinary(char op) {
  ExpressionElementNode *pRight = m_operandStack.top();
  m_operandStack.pop();
  ExpressionElementNode *pLeft = m_operandStack.top();
  m_operandStack.pop();
  BinaryOperationNode *pOperator = new BinaryOperationNode(op, pLeft, pRight);
  m_operandStack.push(pOperator);

//  m_asm.append(&op);
//  m_asm.append(" ");
//  m_asm.append(std::to_string(pLeft->value()));
//  m_asm.append("\n");

//  m_asm.append(&op);
//  m_asm.append(" ");
//  m_asm.append(std::to_string(pRight->value()));
//  m_asm.append("\n");
}

BinaryExpressionBuilder::Precedence
BinaryExpressionBuilder::precedence(char op) {
  switch (op) {
  case '+':
  case '-':
    return Precedence::mid;

  case '*':
  case '/':
    return Precedence::high;

  default:
    return Precedence::low;
  }
}
