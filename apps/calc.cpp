#include <iostream>
#include <string>

#include "BinaryExpressionBuilder.hpp"
#include "BinaryOperationNode.hpp"
#include "NumericElementNode.hpp"
#include "ASMTranslator.hpp"

int main() {
  std::string expression;
  std::cout << "expression: ";
  getline(std::cin, expression);

  BinaryExpressionBuilder builder;
  std::shared_ptr<ExpressionElementNode> pNode = builder.parse(expression);

  ASMTranslator translator;
  translator.generate(std::dynamic_pointer_cast<BinaryOperationNode>(pNode),
                      "output.myasm");
  std::cout << "\n";

  std::cout << "RESULT: " << pNode->value() << "\n";
  return 0;
}
