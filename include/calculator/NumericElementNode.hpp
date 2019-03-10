#pragma once
#include "ExpressionElementNode.hpp"

struct NumericElementNode final : ExpressionElementNode {
  NumericElementNode(const NumericElementNode &) = delete;
  NumericElementNode() = delete;
  NumericElementNode &operator=(const NumericElementNode &) = delete;

  NumericElementNode(int value);
  virtual int value() override;
  virtual NodeType getNodeType() override;

 private:
  int m_value;
};
