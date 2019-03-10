#include <iostream>

#include "BinaryOperationNode.hpp"

BinaryOperationNode::BinaryOperationNode(
  BinaryOperationNode::OperationType binaryOp,
  std::shared_ptr<ExpressionElementNode> pLeft,
  std::shared_ptr<ExpressionElementNode> pRight)
  : mp_left(pLeft),
	mp_right(pRight),
	m_operationResult(.0),
	m_binaryOp(binaryOp) {

}

int BinaryOperationNode::value() {
  int leftValue = mp_left->value();
  int rightValue = mp_right->value();

  int m_operationResult = 0;

  if (m_binaryOp == "+") {
    m_operationResult = leftValue + rightValue;
  } else if (m_binaryOp == "-") {
    m_operationResult = leftValue - rightValue;
  } else if (m_binaryOp == "*") {
    m_operationResult = leftValue * rightValue;
  } else if (m_binaryOp == "/") {
    m_operationResult = leftValue / rightValue;
  }

  return m_operationResult;
}

ExpressionElementNode::NodeType BinaryOperationNode::getNodeType() {
  return NodeType::Operation;
}
