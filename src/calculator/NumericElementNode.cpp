#include "NumericElementNode.hpp"

NumericElementNode::NumericElementNode(int value)
  : m_value(value) {

}

int NumericElementNode::value() {
  return m_value;
}

ExpressionElementNode::NodeType NumericElementNode::getNodeType() {
  return NodeType::Operand;
}
