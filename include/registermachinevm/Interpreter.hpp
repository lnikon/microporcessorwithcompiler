#pragma once

#include <vector>
#include <string>
#include <tuple>

struct RegisterMachineInterpreter {
	void run(const std::string& inputAssembly);

private:
	std::vector<std::tuple<std::string, std::string, std::string>> m_instructions;

	std::size_t m_programCounter{0};
	std::tuple<std::string, std::string, std::string> m_instructionRegister;

	void fillInstructions(const std::string& inputAssembly);

	std::tuple<std::string, std::string, std::string> fetch();
	void decode(const std::tuple<std::string, std::string, std::string>&);
	void execute();
};
