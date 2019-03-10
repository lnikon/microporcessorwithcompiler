#include <fstream>
#include <ostream>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <sstream>
#include <cmath>

#include "ExpressionElementNode.hpp"
#include "BinaryOperationNode.hpp"
#include "BinaryExpressionBuilder.hpp"
#include "ASMTranslator.hpp"

std::string decimalToBinary(int number, int bitNumber) {
	std::string result;
	const int base = 2;
	for(int i = 0; i < bitNumber; i++) {
		bool bit = number % base;
		result += std::to_string(bit);
		number /= 2;
	}

	return result;
}

int binaryToDecimal(const std::string& binary, int bitNumber) {
	// TODO: Remake to std::optional<int>
	if(bitNumber < 0) {
		return -1;
	}

	if(static_cast<std::size_t>(bitNumber) > binary.size()) {
		return -1;
	}

	auto result = int{0};
	const auto base = int{2};
	int bit = 0;
	for(auto i{0}; i < bitNumber; i++) {
		bit = charToInt(binary[i]);
		result +=  bit * std::pow(base, i);
	}

	return result;
}

int charToInt(char c) {
	return c - '0';
}

char intToChar(int i) {
	return i + '0';
}

void ASMTranslator::generateASM(std::shared_ptr<BinaryOperationNode> pASTRoot,
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

void ASMTranslator::generateMachineCode(const std::string &inputAssembly,
                                        const std::string &outputFilename) {
  m_outputASMFile.open(outputFilename, std::fstream::out | std::fstream::trunc);

  if (!m_outputASMFile.is_open()) {
    throw "unable to open file " + outputFilename +
    " to write machine code\n";
  }

  std::fstream inputAssemblyFile(inputAssembly, std::fstream::in);

  if (!inputAssemblyFile.is_open()) {
    throw "unable open assembly file " + inputAssembly +
    " to read instructions\n";
  }

  std::vector<std::string> machineCode;
  std::string instructionLine;
  while(std::getline(inputAssemblyFile, instructionLine)) {
	  if(instructionLine.empty()) {
		  continue;
	  }

	  std::istringstream instrStream(instructionLine);
	  std::vector<std::string> instrs (std::istream_iterator<std::string>{instrStream},
			  std::istream_iterator<std::string>());


	  const auto instrName = instrs[0];
	  const auto instrChain = m_instructionData.getInstructionChain(instrName);

	  if(instrChain.has_value()) {
		  auto instrChainValue = instrChain.value();

		  auto instrOperand 	 = std::string{};
		  auto instrOperandChain = std::string{};
		  auto finalChain 		 = std::string{};

		  std::cout << "<instrChainValue: " << instrChainValue << ">\n";

		  if(instrName == "NOP") {
			  finalChain = instrChainValue;
		  } else if(instrName == "PUSH") {
			  instrOperand = instrs[1];
			  instrOperandChain = decimalToBinary(std::stoi(instrOperand), InstructionData::OPERAND_WIDTH);
			  finalChain = instrChainValue + instrOperandChain;
		  } else if(instrName == "ADD" || instrName == "SUB" || instrName == "MUL" || instrName == "DIV") {
			  finalChain = instrChainValue;
		  }

		  machineCode.emplace_back(finalChain);
	  }
  }

  std::ostream_iterator<std::string> osi{m_outputASMFile};
  std::copy(std::begin(machineCode), std::end(machineCode), osi);

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
