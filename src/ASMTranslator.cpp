#include <fstream>
#include <iostream>

#include "ExpressionElementNode.hpp"
#include "BinaryOperationNode.hpp"
#include "BinaryExpressionBuilder.hpp"
#include "ASMTranslator.hpp"

void ASMTranslator::generate(std::shared_ptr<BinaryOperationNode> pASTRoot,
                             const std::string &outputFilename) {
  m_outputASMFile.open(outputFilename, std::fstream::out | std::fstream::trunc);

  if (!m_outputASMFile.is_open()) {
    throw "unable to open assembly file " + outputFilename +
    " to write assembly\n";
  }

  BinaryOperationNode *pNode = pASTRoot.get();
  postorder(pNode);

  m_outputASMFile.close();
}

void ASMTranslator::postorder(ExpressionElementNode *pNode) {
  using ntype = ExpressionElementNode::NodeType;
  ntype type = pNode->getNodeType();

  std::optional<std::string> operationMnemonic;

  if (type == ntype::Operation) {
    BinaryOperationNode *opNode = dynamic_cast<BinaryOperationNode *>(pNode);

    if (opNode == nullptr) {
      throw "unable to cast to BinaryOperationNode at ASMTranslator::postorder()";
    }

    if (m_translatorData.isSupportedOperation(opNode->m_binaryOp)) {
      operationMnemonic = m_translatorData.getMnemonicForOperation(
                            opNode->m_binaryOp);

      if (!operationMnemonic.has_value()) {
        throw "can\t find mnemonic for specified operation\n";
      }

      postorder(opNode->mp_left.get());
      postorder(opNode->mp_right.get());
      m_outputASMFile << operationMnemonic.value() << "\n";
    } else {
      throw "unsupported operation in ASMTranslator::postorder\n";
    }
  } else {
    m_outputASMFile << "PUSH" << " " << pNode->value() << "\n";
  }
}
