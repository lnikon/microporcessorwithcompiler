#pragma once
#include "ExpressionElementNode.hpp"

struct NumericElementNode final : ExpressionElementNode {
  NumericElementNode(const NumericElementNode &) = delete;
  NumericElementNode() = delete;
  NumericElementNode &operator=(const NumericElementNode &) = delete;

  NumericElementNode(double value);
  virtual double value() override;
  virtual NodeType getNodeType() override;

 private:
  double m_value;
};
