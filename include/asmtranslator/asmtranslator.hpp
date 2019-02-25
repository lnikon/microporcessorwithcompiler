#pragma once
#include <string>

struct BinaryExpressionBuilder;
struct ASMTranslator final {
  void generate(BinaryExpressionBuilder *pASTBuilder,
                const std::string &outputFilename);
};
