#pragma once
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <memory>
#include <optional>

#include "BinaryOperationNode.hpp"

std::string decimalToBinary(int number, int bitNumber);
int binaryToDecimal(const std::string& binary, int bitNumber);
int charToInt(char c);
char intToChar(int i);

struct ASMTranslatorData {
  std::unordered_map<std::string, bool> m_supportedOperations;
  std::unordered_map<std::string, std::string> m_operationToMnemonic;

  ASMTranslatorData() {
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
	static const int INSTRUCTION_WIDTH = 3;
	static const int OPERAND_WIDTH = 32;
	std::unordered_map<std::string, std::string> m_mnemonicToChain;

  InstructionData() {
    m_mnemonicToChain["NOP"]  = "000";
    m_mnemonicToChain["PUSH"] = "001";
    m_mnemonicToChain["ADD"]  = "010";
    m_mnemonicToChain["SUB"]  = "011";
    m_mnemonicToChain["MUL"]  = "100";
    m_mnemonicToChain["DIV"]  = "101";
  }

  std::optional<std::string> getInstructionChain(const std::string &instruction) {
    auto it = m_mnemonicToChain.find(instruction);

    if (it != m_mnemonicToChain.end()) {
      return it->second;
    }

    return std::nullopt;
  }

  /*! \brief Returns instruction name for specified chain
   *
   *  @chain binary string which corresponds to instruction
   *  @return instruction name, if exists, otherwise empty std::optional<std::string>
   */
  std::optional<std::string> getInstructionName(const std::string& chain) {
	  auto it = std::find_if(std::begin(m_mnemonicToChain), std::end(m_mnemonicToChain), [chain](const auto& nameToChain) {
			  return nameToChain.second == chain;
	  });

	  if(it != std::end(m_mnemonicToChain)) {
		  return std::make_optional(it->first);
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

  // TODO: Move this whole shit into separate Common class
  // and links to it
//  std::string decimalToBinary(int number, int bitNumber);
//  int binaryToDecimal(const std::string& binary, int bitNumber);
//  int charToInt(char c);
//  char intToChar(int i);

 private:
  std::fstream m_outputASMFile;
  ASMTranslatorData m_translatorData;
  InstructionData m_instructionData;

  void postorder(ExpressionElementNode *pNode);
};
