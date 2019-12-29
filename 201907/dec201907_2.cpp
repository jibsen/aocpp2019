//
// Advent of Code 2019, day 7, part two
//

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
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

struct Computer {
	std::vector<int> memory;
	int pc = 0;
	bool done = false;
	bool phase_done = false;
	int output = -1;

	explicit Computer(std::vector<int> program) : memory(std::move(program)) {}
};

int run_program(Computer &c, int phase, int input)
{
	while (!c.done) {
		int opcode = c.memory[c.pc];

		int pmode1 = (opcode / 100) % 10;
		int pmode2 = (opcode / 1000) % 10;
		int pmode3 = (opcode / 10000) % 10;

		opcode = opcode % 100;

		switch (opcode) {
		case 1:
		case 2:
			{
				int op1 = c.memory[c.pc + 1];
				int op2 = c.memory[c.pc + 2];
				int op3 = c.memory[c.pc + 3];

				int size = c.memory.size();
				if (!pmode1) { size = std::max(size, op1 + 1); }
				if (!pmode2) { size = std::max(size, op2 + 1); }
				if (!pmode3) { size = std::max(size, op3 + 1); }

				if (size > c.memory.size()) {
					c.memory.resize(size);
				}

				if (opcode == 1) {
					c.memory[op3] = (pmode1 ? op1 : c.memory[op1]) + (pmode2 ? op2 : c.memory[op2]);
				}
				else {
					c.memory[op3] = (pmode1 ? op1 : c.memory[op1]) * (pmode2 ? op2 : c.memory[op2]);
				}

				c.pc += 4;
			}
			break;
		case 3:
			{
				int op1 = c.memory[c.pc + 1];

				if (c.phase_done) {
					c.memory[op1] = input;
				}
				else {
					c.memory[op1] = phase;
					c.phase_done = true;
				}

				c.pc += 2;
			}
			break;
		case 4:
			{
				int op1 = c.memory[c.pc + 1];

				c.output = pmode1 ? op1 : c.memory[op1];

				c.pc += 2;

				return c.output;
			}
			break;
		case 5:
			{
				int op1 = c.memory[c.pc + 1];
				int op2 = c.memory[c.pc + 2];

				if (pmode1 ? op1 : c.memory[op1]) {
					c.pc = pmode2 ? op2 : c.memory[op2];
				}
				else {
					c.pc += 3;
				}
			}
			break;
		case 6:
			{
				int op1 = c.memory[c.pc + 1];
				int op2 = c.memory[c.pc + 2];

				if (!(pmode1 ? op1 : c.memory[op1])) {
					c.pc = pmode2 ? op2 : c.memory[op2];
				}
				else {
					c.pc += 3;
				}
			}
			break;
		case 7:
			{
				int op1 = c.memory[c.pc + 1];
				int op2 = c.memory[c.pc + 2];
				int op3 = c.memory[c.pc + 3];

				c.memory[op3] = (pmode1 ? op1 : c.memory[op1]) < (pmode2 ? op2 : c.memory[op2]);

				c.pc += 4;
			}
			break;
		case 8:
			{
				int op1 = c.memory[c.pc + 1];
				int op2 = c.memory[c.pc + 2];
				int op3 = c.memory[c.pc + 3];

				c.memory[op3] = (pmode1 ? op1 : c.memory[op1]) == (pmode2 ? op2 : c.memory[op2]);

				c.pc += 4;
			}
			break;
		case 99:
			c.done = true;
			break;
		default:
			std::cerr << "opcode error: " << opcode << std::endl;
			exit(1);
			break;
		}
	}

	return c.output;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cerr << "no program file\n";
		exit(1);
	}

	std::vector<int> memory = read_program(argv[1]);

	int max_thrust = std::numeric_limits<int>::min();

	std::array<int, 5> phase = { 5, 6, 7, 8, 9 };
	std::array<int, 5> max_phase = { 5, 6, 7, 8, 9 };

	do {
		Computer a(memory);
		Computer b(memory);
		Computer c(memory);
		Computer d(memory);
		Computer e(memory);

		int thrust = 0;

		do {
			thrust = run_program(a, phase[0], thrust);
			thrust = run_program(b, phase[1], thrust);
			thrust = run_program(c, phase[2], thrust);
			thrust = run_program(d, phase[3], thrust);
			thrust = run_program(e, phase[4], thrust);
		} while (!e.done);

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
