#include <fstream>
#include <sstream>
#include <iterator>
#include <string>

#include "Interpreter.hpp"

void RegisterMachineInterpreter::run(
		const std::string& inputAssembly) {
	fillInstructions(inputAssembly);

	const std::size_t instructionsSize = m_instructions.size();

	while(m_programCounter != instructionsSize) {
		std::tuple<std::string, std::string, std::string> instructionWithOperands = fetch();
		decode(instructionWithOperands);
		execute();
	}
}


void RegisterMachineInterpreter::fillInstructions(
		const std::string& inputAssembly) {
	using StringIterator = std::istream_iterator<std::string>;

	std::fstream inputAssemblyStream(inputAssembly);

	if(!inputAssemblyStream.is_open()) {
		throw "can't open input assembly from Register Machine Interpreter\n";
	}

	auto line = std::string{};

	auto instruction = std::string{};
	auto operand1 = std::string{};
	auto operand2 = std::string{};

	while(inputAssemblyStream >> line) {
		auto lineStream = std::istringstream{line};

		std::vector<std::string> lineVector{StringIterator{lineStream},
											StringIterator{}};

		bool areThreeTokens = (lineVector.size() == 3);

		instruction = lineVector[0];
		operand1 = lineVector[1];

		if(areThreeTokens) {
			operand2 = lineVector[2];
		} else {
			operand2 = std::string{};
		}

		m_instructions.push_back(std::make_tuple(instruction, operand1, operand1));
	}
}

std::tuple<std::string, std::string, std::string> RegisterMachineInterpreter::fetch() {
	return m_instructions[m_programCounter++];
}

void RegisterMachineInterpreter::decode(
		const std::tuple<std::string, std::string, std::string>& instructionWithOperands) {

}

void RegisterMachineInterpreter::execute() {

}
