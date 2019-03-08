#include <sstream>
#include <iostream>
#include "BinaryExpressionBuilder.hpp"
#include "NumericElementNode.hpp"
#include "BinaryOperationNode.hpp"
#include "ExpressionElementNode.hpp"

std::shared_ptr<ExpressionElementNode> BinaryExpressionBuilder::parse(
  const std::string &expression) {

  std::stringstream exprStream(expression);
  std::string token;

  while (exprStream >> token) {
    std::cout << token;

    if (token == "+" ||
        token == "-" ||
        token == "*" ||
        token == "/") {

      processOperator(token);
    } else if (token == ")") {
      processRightParenthesis();
    } else if (token == "(") {
      m_operatorStack.push(token);
    } else {
      double number = std::stol(token);
      exprStream >> number;
      std::shared_ptr<NumericElementNode> pNode =
        std::make_shared<NumericElementNode>(number);

      m_operandStack.push(pNode);
    }
  }


  while (!m_operatorStack.empty()) {
    doBinary(m_operatorStack.top());
    m_operatorStack.pop();
  }

  if (m_operandStack.size() != 1) {
    throw "not well formed expression\n";
  }

  std::shared_ptr<ExpressionElementNode> pNode = m_operandStack.top();
  return std::dynamic_pointer_cast<BinaryOperationNode>(pNode);
}

void BinaryExpressionBuilder::processOperator(std::string op) {
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
  while (!m_operatorStack.empty() && m_operatorStack.top() != "(") {
    doBinary(m_operatorStack.top());
    m_operatorStack.pop();
  }

  m_operatorStack.pop();
}

void BinaryExpressionBuilder::doBinary(std::string op) {
  std::shared_ptr<ExpressionElementNode> pRight = m_operandStack.top();
  m_operandStack.pop();

  std::shared_ptr<ExpressionElementNode> pLeft = m_operandStack.top();
  m_operandStack.pop();

  std::shared_ptr<BinaryOperationNode> pOperator =
    std::make_shared<BinaryOperationNode>(op, pLeft, pRight);

  m_operandStack.push(pOperator);
}

BinaryExpressionBuilder::Precedence
BinaryExpressionBuilder::precedence(std::string op) {
  if (op == "+" ||
      op == "-") {
    return Precedence::mid;
  } else if (op == "*" ||
             op == "/") {
    return Precedence::high;
  } else {
    return Precedence::low;
  }
}
