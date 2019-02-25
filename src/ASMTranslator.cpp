#include <fstream>
#include <iostream>

#include "ExpressionElementNode.hpp"
#include "BinaryOperationNode.hpp"
#include "BinaryExpressionBuilder.hpp"
#include "ASMTranslator.hpp"

void ASMTranslator::generate(BinaryOperationNode *pASTRoot,
                             const std::string &outputFilename) {
  m_outputASMFile.open(outputFilename);

  if (!m_outputASMFile.is_open()) {
    std::cerr << "unable to open assembly file " << outputFilename <<
              " to write assembly\n";
    return;
  }

  BinaryOperationNode *pNode = pASTRoot;
  postorder(pNode);

  m_outputASMFile.close();
}

void ASMTranslator::postorder(ExpressionElementNode *pNode) {
  using ntype = ExpressionElementNode::NodeType;
  ntype type = pNode->getNodeType();

  if (type == ntype::Operation) {
    BinaryOperationNode *opNode = dynamic_cast<BinaryOperationNode *>(pNode);

    if (opNode == nullptr) {
      return;
    }

    std::cout << opNode->m_binaryOp << " ";
    postorder(opNode->mp_left);
    postorder(opNode->mp_right);
    std::cout << "\n";
  }

  std::cout << pNode->value() << " ";
}