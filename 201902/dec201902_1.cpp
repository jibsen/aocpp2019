//
// Advent of Code 2019, day 2, part one
//

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<int> read_program()
{
	std::vector<int> program;
	int code;

	for (char sep = ','; sep == ',' && std::cin >> code; std::cin >> sep) {
		program.push_back(code);
	}

	return program;
}

int main()
{
	std::vector<int> memory = read_program();
	int pc = 0;
	bool done = false;

	memory[1] = 12;
	memory[2] = 2;

	while (!done) {
		int opcode = memory[pc];

		switch (opcode) {
		case 1:
		case 2:
			{
				int op1, op2, dst;

				op1 = memory[pc + 1];
				op2 = memory[pc + 2];
				dst = memory[pc + 3];

				int size = std::max(std::max(op1, op2), dst) + 1;

				if (size > memory.size()) {
					memory.resize(size);
				}

				if (opcode == 1) {
					memory[dst] = memory[op1] + memory[op2];
				}
				else {
					memory[dst] = memory[op1] * memory[op2];
				}

				pc += 4;
			}
			break;
		case 99:
			done = true;
			break;
		default:
			std::cerr << "opcode error: " << opcode << std::endl;
			exit(1);
			break;
		}
	}

	for (const auto &v : memory) {
		std::cout << v << ',';
	}

	return 0;
}
