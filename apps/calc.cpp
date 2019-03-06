#include <iostream>
#include <string>

#include "BinaryExpressionBuilder.hpp"
#include "BinaryOperationNode.hpp"
#include "NumericElementNode.hpp"
#include "ASMTranslator.hpp"

int main() {
  std::string expression = "3 + 4";
//  std::cout << "expression: ";
//  getline(std::cin, expression);

  BinaryExpressionBuilder builder;
  std::shared_ptr<ExpressionElementNode> pNode = builder.parse(expression);

  ASMTranslator translator;
  std::shared_ptr<BinaryOperationNode> sharedNode =
    std::dynamic_pointer_cast<BinaryOperationNode>(pNode);

  if (sharedNode == nullptr) {
    throw "unable to cast ExpressionElementNode to BinaryOperationNode\n";
  }

  translator.generateASM(sharedNode,
                         "output.myasm");
  std::cout << "\n";

  std::cout << "RESULT: " << pNode->value() << "\n";
  return 0;
}
