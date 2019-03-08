#include "NumericElementNode.hpp"

NumericElementNode::NumericElementNode(double value)
  : m_value(value) {

}

double NumericElementNode::value() {
  return m_value;
}

ExpressionElementNode::NodeType NumericElementNode::getNodeType() {
  return NodeType::Operand;
}
