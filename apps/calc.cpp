#include <iostream>
#include <string>
#include <cstdlib>

#include "BinaryExpressionBuilder.hpp"
#include "BinaryOperationNode.hpp"
#include "NumericElementNode.hpp"
#include "ASMTranslatorForStackMachine.hpp"
#include "ASMInterpreter.hpp"

int main() {
  auto expression = std::string{"((5 - 3) +5*(4-3) + 2) / 2"};
  BinaryExpressionBuilder builder;
  std::shared_ptr<ExpressionElementNode> pNode = builder.parse(expression);

  ASMTranslatorForStackMachine translator;
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
