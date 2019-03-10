#include <iostream>
#include <fstream>
#include <sstream>

#include "ASMInterpreter.hpp"

void ASMInterpreter::run(const std::string& filename) {
	std::ifstream machineCode(filename);

	if(!machineCode.is_open()) {
		throw "can't open machine code file " + filename + " to execute";
	}

	auto finalChain = std::string{};

	std::stringstream ss;
	ss << machineCode.rdbuf();
	finalChain = ss.str();

//	machineCode.seekg(0, std::ios::end);
//	finalChain.reserve(machineCode.tellg());
//	machineCode.seekg(0, std::ios::beg);
//
//	finalChain.assign(std::istreambuf_iterator<char>{machineCode},
//			std::istreambuf_iterator<char>());

	// Our machine code chains format is follow
	// [0...2][3-34]
	// opcode operand
	auto opcode = std::string{};
	auto operand = std::string{};

	int pos = 0;
	int prevPos = 0;

	while(pos != finalChain.size()) {
		auto instrChain = finalChain.substr(pos, InstructionData::INSTRUCTION_WIDTH);
		auto instrOpt = m_instructionData.getInstructionName(instrChain);
		if(!instrOpt.has_value()) {
			throw "unspecified instruction chain: " + instrChain + "\n";
		}

		pos += InstructionData::INSTRUCTION_WIDTH;

		auto instrName = instrOpt.value();
		auto instrOperandBinary = std::string{};

		if(instrName == "NOP") {
			// Just NOP it!
		} else if(instrName == "PUSH") {
			instrOperandBinary = finalChain.substr(pos, InstructionData::OPERAND_WIDTH);
			auto operandDecimal = binaryToDecimal(instrOperandBinary, InstructionData::OPERAND_WIDTH);

			pos += InstructionData::OPERAND_WIDTH;

			m_operations.push(instrName);
			m_operands.push(operandDecimal);

			std::cout << instrName << " " << operandDecimal << "\n";
		} else if(instrName == "ADD") {
			auto rightOperand = m_operands.top();
			m_operands.pop();
			auto leftOperand = m_operands.top();
			m_operands.pop();

			auto result = leftOperand + rightOperand;
			m_operands.push(result);

			std::cout << instrName << " " << leftOperand << " " << rightOperand << "\n";
		} else if(instrName == "SUB") {
			auto rightOperand = m_operands.top();
			m_operands.pop();
			auto leftOperand = m_operands.top();
			m_operands.pop();

			auto result = leftOperand - rightOperand;
			m_operands.push(result);

			std::cout << instrName << " " << leftOperand << " " << rightOperand << "\n";
		} else if(instrName == "MUL") {
			auto rightOperand = m_operands.top();
			m_operands.pop();
			auto leftOperand = m_operands.top();
			m_operands.pop();

			auto result = leftOperand * rightOperand;
			m_operands.push(result);

			std::cout << instrName << " " << leftOperand << " " << rightOperand << "\n";
		} else if(instrName == "DIV") {
			auto rightOperand = m_operands.top();
			m_operands.pop();
			auto leftOperand = m_operands.top();
			m_operands.pop();

			auto result = leftOperand / rightOperand;
			m_operands.push(result);

			std::cout << instrName << " " << leftOperand << " " << rightOperand << "\n";
		}
	}
}
