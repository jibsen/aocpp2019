//
// Advent of Code 2019, day 5, part two
//

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<int> read_program(const char *filename)
{
	std::ifstream infile(filename);

	std::vector<int> program;
	int code;

	for (char sep = ','; sep == ',' && infile >> code; infile >> sep) {
		program.push_back(code);
	}

	return program;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cerr << "no program file\n";
		exit(1);
	}

	std::vector<int> memory = read_program(argv[1]);

	int pc = 0;
	bool done = false;

	while (!done) {
		int opcode = memory[pc];

		int pmode1 = (opcode / 100) % 10;
		int pmode2 = (opcode / 1000) % 10;
		int pmode3 = (opcode / 10000) % 10;

		opcode = opcode % 100;

		switch (opcode) {
		case 1:
		case 2:
			{
				int op1 = memory[pc + 1];
				int op2 = memory[pc + 2];
				int op3 = memory[pc + 3];

				int size = memory.size();
				if (!pmode1) { size = std::max(size, op1 + 1); }
				if (!pmode2) { size = std::max(size, op2 + 1); }
				if (!pmode3) { size = std::max(size, op3 + 1); }

				if (size > memory.size()) {
					memory.resize(size);
				}

				if (opcode == 1) {
					memory[op3] = (pmode1 ? op1 : memory[op1]) + (pmode2 ? op2 : memory[op2]);
				}
				else {
					memory[op3] = (pmode1 ? op1 : memory[op1]) * (pmode2 ? op2 : memory[op2]);
				}

				pc += 4;
			}
			break;
		case 3:
			{
				int op1 = memory[pc + 1];
				int value = 0;

				std::cin >> value;

				memory[op1] = value;

				pc += 2;
			}
			break;
		case 4:
			{
				int op1 = memory[pc + 1];

				std::cout << (pmode1 ? op1 : memory[op1]) << '\n';

				pc += 2;
			}
			break;
		case 5:
			{
				int op1 = memory[pc + 1];
				int op2 = memory[pc + 2];

				if (pmode1 ? op1 : memory[op1]) {
					pc = pmode2 ? op2 : memory[op2];
				}
				else {
					pc += 3;
				}
			}
			break;
		case 6:
			{
				int op1 = memory[pc + 1];
				int op2 = memory[pc + 2];

				if (!(pmode1 ? op1 : memory[op1])) {
					pc = pmode2 ? op2 : memory[op2];
				}
				else {
					pc += 3;
				}
			}
			break;
		case 7:
			{
				int op1 = memory[pc + 1];
				int op2 = memory[pc + 2];
				int op3 = memory[pc + 3];

				memory[op3] = (pmode1 ? op1 : memory[op1]) < (pmode2 ? op2 : memory[op2]);

				pc += 4;
			}
			break;
		case 8:
			{
				int op1 = memory[pc + 1];
				int op2 = memory[pc + 2];
				int op3 = memory[pc + 3];

				memory[op3] = (pmode1 ? op1 : memory[op1]) == (pmode2 ? op2 : memory[op2]);

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

	return 0;
}
