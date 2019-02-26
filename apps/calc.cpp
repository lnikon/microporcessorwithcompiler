#include <iostream>
#include <string>

#include "BinaryExpressionBuilder.hpp"
#include "BinaryOperationNode.hpp"
#include "NumericElementNode.hpp"

#include "ASMTranslator.hpp"

using ntype = ExpressionElementNode::NodeType;
void postoreder(ExpressionElementNode *pNode) {
  ntype type = pNode->getNodeType();

  if (type == ntype::Operation) {
    BinaryOperationNode *opNode = dynamic_cast<BinaryOperationNode *>(pNode);

    if (opNode == nullptr) {
      return;
    }

    std::cout << opNode->m_binaryOp << " ";
    postoreder(opNode->mp_left);
    postoreder(opNode->mp_right);
    std::cout << "\n";
  } else {
    std::cout << pNode->value() << " ";
  }
}

int main() {
  std::string expression;
  std::cout << "expression: ";
  getline(std::cin, expression);

  BinaryExpressionBuilder builder;
  BinaryOperationNode *pNode = builder.parse(expression);

//  postoreder(dynamic_cast<ExpressionElementNode *>(pNode));

  ASMTranslator translator;
  translator.generate(pNode, "output.myasm");
  std::cout << "\n";

  std::cout << "RESULT: " << pNode->value() << "\n";
  return 0;
}
