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

  std::string operationMnemonic;

  if (type == ntype::Operation) {
    BinaryOperationNode *opNode = dynamic_cast<BinaryOperationNode *>(pNode);

    if (opNode == nullptr) {
      return;
    }

    if (m_translatorData.isSupportedOperation(opNode->m_binaryOp)) {
      operationMnemonic = m_translatorData.getMnemonicForOperation(
                            opNode->m_binaryOp);
      postorder(opNode->mp_left);
      postorder(opNode->mp_right);
      m_outputASMFile << operationMnemonic << "\n";
//      std::cout << operationMnemonic << "\n";
    } else {
      std::cerr << "unsupported operation\n";
      return;
    }
  } else {
    m_outputASMFile << "PUSH" << " " << pNode->value() << "\n";
//    std::cout << "PUSH" << " " << pNode->value() << "\n";
  }
}
