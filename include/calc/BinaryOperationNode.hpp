#pragma once
#include "ExpressionElementNode.hpp"

struct BinaryOperationNode final: ExpressionElementNode {
  BinaryOperationNode(char binaryOp,
                      ExpressionElementNode *pLeft,
                      ExpressionElementNode *pRight);

  virtual double value() override;
  virtual NodeType getNodeType() override;

  ExpressionElementNode *mp_left;
  ExpressionElementNode *mp_right;
  char m_binaryOp;
 private:

  BinaryOperationNode() = delete;
  BinaryOperationNode(const BinaryOperationNode &) = delete;
  BinaryOperationNode &operator=(const BinaryOperationNode &) = delete;
};
