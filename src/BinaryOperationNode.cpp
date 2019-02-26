#include <iostream>

#include "BinaryOperationNode.hpp"

BinaryOperationNode::BinaryOperationNode(
  char binaryOp,
  ExpressionElementNode *pLeft,
  ExpressionElementNode *pRight)
  : mp_left(pLeft), mp_right(pRight), m_binaryOp(binaryOp) {

}

double BinaryOperationNode::value() {
  double leftValue = mp_left->value();
  double rightValue = mp_right->value();

  double m_operationResult = 0;

  switch (m_binaryOp) {
  case '+': {
    m_operationResult = leftValue + rightValue;
    break;
  }

  case '-': {
    m_operationResult = leftValue - rightValue;
    break;
  }

  case '*': {
    m_operationResult = leftValue * rightValue;
    break;
  }

  case '/': {
    m_operationResult = leftValue / rightValue;
    break;
  }

  default: {
    std::cerr << "unkown operation\n";
    break;
  }
  }

  return m_operationResult;
}

ExpressionElementNode::NodeType BinaryOperationNode::getNodeType() {
  return NodeType::Operation;
}
