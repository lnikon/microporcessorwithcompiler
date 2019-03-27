#pragma once
#include <memory>
#include "ExpressionElementNode.hpp"

struct BinaryOperationNode final: ExpressionElementNode {
  using OperationType = std::string;

  BinaryOperationNode(OperationType binaryOp,
      std::shared_ptr<ExpressionElementNode> pLeft,
      std::shared_ptr<ExpressionElementNode> pRight);


  virtual int value() override;
  virtual NodeType getNodeType() override;

  std::shared_ptr<ExpressionElementNode> mp_left;
  std::shared_ptr<ExpressionElementNode> mp_right;
  int m_operationResult;
  OperationType m_binaryOp;
  private:

  BinaryOperationNode() = delete;
  BinaryOperationNode(const BinaryOperationNode &) = delete;
  BinaryOperationNode &operator=(const BinaryOperationNode &) = delete;
};
