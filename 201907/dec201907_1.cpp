//
// Advent of Code 2019, day 7, part one
//

#include <algorithm>
#include <array>
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

int run_program(std::vector<int> memory, int phase, int input)
{
	int pc = 0;
	bool done = false;
	int output = -1;
	bool phase_done = false;

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

				if (phase_done) {
					memory[op1] = input;
				}
				else {
					memory[op1] = phase;
					phase_done = true;
				}

				pc += 2;
			}
			break;
		case 4:
			{
				int op1 = memory[pc + 1];

				output = pmode1 ? op1 : memory[op1];

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

	return output;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cerr << "no program file\n";
		exit(1);
	}

	std::vector<int> memory = read_program(argv[1]);

	int max_thrust = std::numeric_limits<int>::min();

	std::array<int, 5> phase = { 0, 1, 2, 3, 4 };
	std::array<int, 5> max_phase = { 0, 1, 2, 3, 4 };

	do {
		int thrust = 0;

		for (auto p : phase) {
			thrust = run_program(memory, p, thrust);
		}

		if (thrust > max_thrust) {
			max_thrust = thrust;
			max_phase = phase;
		}
	} while (std::next_permutation(phase.begin(), phase.end()));

	std::cout << "max thruster signal " << max_thrust << " at "
		<< max_phase[0] << ','
		<< max_phase[1] << ','
		<< max_phase[2] << ','
		<< max_phase[3] << ','
		<< max_phase[4] << '\n';

	return 0;
}
