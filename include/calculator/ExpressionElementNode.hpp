#pragma once
struct ExpressionElementNode {
  enum NodeType {
    Operation = 0,
    Operand
  };

  virtual int value() = 0;
  virtual ~ExpressionElementNode() = default;
  virtual NodeType getNodeType() = 0;
};
