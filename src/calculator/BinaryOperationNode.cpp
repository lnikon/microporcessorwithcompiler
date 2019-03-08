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

double BinaryOperationNode::value() {
  double leftValue = mp_left->value();
  double rightValue = mp_right->value();

  double m_operationResult = 0;

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
