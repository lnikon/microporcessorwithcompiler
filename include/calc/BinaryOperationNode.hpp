#pragma once
#include <memory>
#include "ExpressionElementNode.hpp"

struct BinaryOperationNode final: ExpressionElementNode {
  BinaryOperationNode(char binaryOp,
                      std::shared_ptr<ExpressionElementNode> pLeft,
                      std::shared_ptr<ExpressionElementNode> pRight);

  virtual double value() override;
  virtual NodeType getNodeType() override;

  std::shared_ptr<ExpressionElementNode> mp_left;
  std::shared_ptr<ExpressionElementNode> mp_right;
  double m_operationResult;
  char m_binaryOp;
 private:

  BinaryOperationNode() = delete;
  BinaryOperationNode(const BinaryOperationNode &) = delete;
  BinaryOperationNode &operator=(const BinaryOperationNode &) = delete;
};
