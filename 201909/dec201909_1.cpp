//
// Advent of Code 2019, day 9, part one
//

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

std::vector<long long> read_program(const char *filename)
{
	std::ifstream infile(filename);

	std::vector<long long> program;
	long long code;

	for (char sep = ','; sep == ',' && infile >> code; infile >> sep) {
		program.push_back(code);
	}

	return program;
}

struct Computer {
	std::vector<long long> memory;
	long long pc = 0;
	long long base = 0;

	explicit Computer(std::vector<long long> program) : memory(std::move(program)) {}

	long long get_arg(long long address, int mode) {
		if (mode == 1) {
			return address;
		}

		if (mode == 2) {
			address = base + address;
		}

		auto size = std::max(memory.size(), static_cast<size_t>(address) + 1);
		if (size > memory.size()) {
			memory.resize(size);
		}

		return memory[address];
	}

	void set_arg(long long address, int mode, long long value) {
		if (mode == 2) {
			address = base + address;
		}

		auto size = std::max(memory.size(), static_cast<size_t>(address) + 1);
		if (size > memory.size()) {
			memory.resize(size);
		}

		memory[address] = value;
	}
};

void run_program(std::vector<long long> &program)
{
	Computer c(program);
	bool done = false;

	while (!done) {
		int opcode = c.memory[c.pc];

		int pmode1 = (opcode / 100) % 10;
		int pmode2 = (opcode / 1000) % 10;
		int pmode3 = (opcode / 10000) % 10;

		opcode = opcode % 100;

		switch (opcode) {
		case 1:
		case 2:
			{
				long long op1 = c.get_arg(c.memory[c.pc + 1], pmode1);
				long long op2 = c.get_arg(c.memory[c.pc + 2], pmode2);
				long long dst3 = c.memory[c.pc + 3];

				if (opcode == 1) {
					c.set_arg(dst3, pmode3, op1 + op2);
				}
				else {
					c.set_arg(dst3, pmode3, op1 * op2);
				}

				c.pc += 4;
			}
			break;
		case 3:
			{
				long long dst1 = c.memory[c.pc + 1];
				long long value = 0;

				std::cin >> value;

				c.set_arg(dst1, pmode1, value);

				c.pc += 2;
			}
			break;
		case 4:
			{
				long long op1 = c.get_arg(c.memory[c.pc + 1], pmode1);

				std::cout << op1 << '\n';

				c.pc += 2;
			}
			break;
		case 5:
			{
				long long op1 = c.get_arg(c.memory[c.pc + 1], pmode1);
				long long op2 = c.get_arg(c.memory[c.pc + 2], pmode2);

				if (op1) {
					c.pc = op2;
				}
				else {
					c.pc += 3;
				}
			}
			break;
		case 6:
			{
				long long op1 = c.get_arg(c.memory[c.pc + 1], pmode1);
				long long op2 = c.get_arg(c.memory[c.pc + 2], pmode2);

				if (!op1) {
					c.pc = op2;
				}
				else {
					c.pc += 3;
				}
			}
			break;
		case 7:
			{
				long long op1 = c.get_arg(c.memory[c.pc + 1], pmode1);
				long long op2 = c.get_arg(c.memory[c.pc + 2], pmode2);
				long long dst3 = c.memory[c.pc + 3];

				c.set_arg(dst3, pmode3, op1 < op2);

				c.pc += 4;
			}
			break;
		case 8:
			{
				long long op1 = c.get_arg(c.memory[c.pc + 1], pmode1);
				long long op2 = c.get_arg(c.memory[c.pc + 2], pmode2);
				long long dst3 = c.memory[c.pc + 3];

				c.set_arg(dst3, pmode3, op1 == op2);

				c.pc += 4;
			}
			break;
		case 9:
			{
				long long op1 = c.get_arg(c.memory[c.pc + 1], pmode1);

				c.base += op1;

				c.pc += 2;
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
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cerr << "no program file\n";
		exit(1);
	}

	std::vector<long long> memory = read_program(argv[1]);

	run_program(memory);

	return 0;
}
