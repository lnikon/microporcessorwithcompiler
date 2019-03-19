#include <iostream>
#include <string>

#include "Interpreter.hpp"

int main() {
	RegisterMachineInterpreter rgvm;

	const std::string& filename = "two_operand_asm.myasm";
	rgvm.run(filename);

    return 0;
}
