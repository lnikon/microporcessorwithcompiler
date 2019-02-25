#pragma once
#include <fstream>
#include <string>

#include "BinaryOperationNode.hpp"

struct BinaryExpressionBuilder;
struct ASMTranslator final {
  void generate(BinaryOperationNode *pASTRoot,
                const std::string &outputFilename);

 private:
  std::fstream m_outputASMFile;

  void postorder(ExpressionElementNode *pNode);

};
