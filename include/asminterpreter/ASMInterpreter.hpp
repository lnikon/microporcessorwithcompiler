#pragma once

#include <string>
#include <stack>
#include "ASMTranslator.hpp"

struct ASMInterpreter {
	void run(const std::string& filename);

private:
	InstructionData m_instructionData{};

	std::stack<int> m_operands;
	std::stack<std::string> m_operations;
};
