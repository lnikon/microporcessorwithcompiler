#include <iostream>
#include <string>

#include "BinaryExpressionBuilder.hpp"
#include "BinaryOperationNode.hpp"
#include "NumericElementNode.hpp"
#include "ASMTranslator.hpp"
#include "ASMInterpreter.hpp"

int main() {
  auto expression = std::string{"((5.3 - 3) +5*(4-3) + 2) / 2"};
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

  // Yeah, my binary is in ASCII :)
  translator.generateMachineCode("output.myasm", "machineCode.txt");
  std::cout << "\n";

  ASMInterpreter interpreter;
  interpreter.run("machineCode.txt");

  std::cout << "RESULT: " << pNode->value() << "\n";
  return 0;
}
