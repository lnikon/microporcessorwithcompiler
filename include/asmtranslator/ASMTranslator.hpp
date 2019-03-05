#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include <memory>
#include <optional>

#include "BinaryOperationNode.hpp"

struct ASMTranslatorData {
  std::unordered_map<char, bool> m_supportedOperations;
  std::unordered_map<char, std::string> m_operationToMnemonic;

  ASMTranslatorData() {
    // TODO: Figure out antoher way to handle operations
    m_supportedOperations['+'] = true;
    m_supportedOperations['-'] = true;
    m_supportedOperations['*'] = true;
    m_supportedOperations['/'] = true;

    m_operationToMnemonic['+'] = "ADD";
    m_operationToMnemonic['-'] = "SUB";
    m_operationToMnemonic['*'] = "MUL";
    m_operationToMnemonic['/'] = "DIV";
  }

  bool isSupportedOperation(char operation) {
    return
      m_supportedOperations.find(operation) != m_supportedOperations.end();
  }

  std::optional<std::string> getMnemonicForOperation(char operation) {
    if (isSupportedOperation(operation)) {
      return m_operationToMnemonic[operation];
    }

    return std::nullopt;
  }
};

struct BinaryExpressionBuilder;
struct ASMTranslator final {
  void generate(std::shared_ptr<BinaryOperationNode> pASTRoot,
                const std::string &outputFilename);

 private:
  std::fstream m_outputASMFile;
  ASMTranslatorData m_translatorData;

  void postorder(ExpressionElementNode *pNode);
};
