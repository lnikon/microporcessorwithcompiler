#pragma once
#include <vector>
#include <string>

#include "BinaryOperationNode.hpp"

struct ASMTranslatorForRegisterMachine {
	void generateAssembly(BinaryOperationNode *pASTRoot);

	void generateMachineCode(const std::string& inputFilename,
							 const std::string& outputFilename);
private:
	std::vector<std::string> m_instructions;
};
