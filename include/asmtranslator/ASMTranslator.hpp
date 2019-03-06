#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include <memory>
#include <optional>

#include "BinaryOperationNode.hpp"

struct ASMTranslatorData {
  std::unordered_map<std::string, bool> m_supportedOperations;
  std::unordered_map<std::string, std::string> m_operationToMnemonic;

  ASMTranslatorData() {
    // TODO: Figure out antoher way to handle operations
    m_supportedOperations["+"] = true;
    m_supportedOperations["-"] = true;
    m_supportedOperations["*"] = true;
    m_supportedOperations["/"] = true;

    m_operationToMnemonic["+"] = "ADD";
    m_operationToMnemonic["-"] = "SUB";
    m_operationToMnemonic["*"] = "MUL";
    m_operationToMnemonic["/"] = "DIV";
  }

  bool isSupportedOperation(std::string operation) {
    return
      m_supportedOperations.find(operation) != m_supportedOperations.end();
  }

  std::optional<std::string> getMnemonicForOperation(std::string operation) {
    if (isSupportedOperation(operation)) {
      return m_operationToMnemonic[operation];
    }

    return std::nullopt;
  }
};

struct InstructionData {
  std::unordered_map<std::string, std::string> m_mnemonicToChain;

  InstructionData() {
    m_mnemonicToChain["NOP"]  = "000";
    m_mnemonicToChain["PUSH"] = "001";
    m_mnemonicToChain["ADD"]  = "010";
    m_mnemonicToChain["SUB"]  = "011";
    m_mnemonicToChain["MUL"]  = "100";
    m_mnemonicToChain["DIV"]  = "101";
  }

  std::optional<std::string> getInstructionChain(std::string &instruction) {
    auto it = m_mnemonicToChain.find(instruction);

    if (it != m_mnemonicToChain.end()) {
      return it->second;
    }

    return std::nullopt;
  }
};

struct BinaryExpressionBuilder;
struct ASMTranslator final {
  void generateASM(std::shared_ptr<BinaryOperationNode> pASTRoot,
                   const std::string &outputFilename);

  void generateMachineCode(const std::string &inputAssembly,
                           const std::string &outputFilename);

 private:
  std::fstream m_outputASMFile;
  ASMTranslatorData m_translatorData;

  void postorder(ExpressionElementNode *pNode);
};
