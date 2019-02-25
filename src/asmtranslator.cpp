#include <fstream>
#include <iostream>

#include "ExpressionElementNode.hpp"
#include "asmtranslator.hpp"

void ASMTranslator::generate(BinaryExpressionBuilder *pASTBuilder,
                             const std::string &outputFilename) {
  std::fstream outputASMFile(outputFilename);

  if (!outputASMFile.is_open()) {
    std::cerr << "unable to open assembly file " << outputFilename <<
              " to write assembly\n";
    return;
  }

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
  }

  std::cout << pNode->value() << " ";
}
