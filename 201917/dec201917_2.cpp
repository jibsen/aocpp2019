//
// Advent of Code 2019, day 17, part two
//

#include <algorithm>
#include <fstream>
#include <iostream>
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

template<typename IODevice>
class Computer {
	std::vector<long long> memory;
	long long pc = 0;
	long long base = 0;
	bool halt = false;
	IODevice io;

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

public:
	explicit Computer(std::vector<long long> program) : memory(std::move(program)) {}

	void step();
	void run() { while (!halt) { step(); } }
};

template<typename IODevice>
void Computer<IODevice>::step()
{
	if (halt) {
		return;
	}

	int opcode = static_cast<int>(memory[pc]);

	int pmode1 = (opcode / 100) % 10;
	int pmode2 = (opcode / 1000) % 10;
	int pmode3 = (opcode / 10000) % 10;

	opcode = opcode % 100;

	switch (opcode) {
	case 1:
	case 2:
		{
			long long op1 = get_arg(memory[pc + 1], pmode1);
			long long op2 = get_arg(memory[pc + 2], pmode2);
			long long dst3 = memory[pc + 3];

			if (opcode == 1) {
				set_arg(dst3, pmode3, op1 + op2);
			}
			else {
				set_arg(dst3, pmode3, op1 * op2);
			}

			pc += 4;
		}
		break;
	case 3:
		{
			long long dst1 = memory[pc + 1];
			long long value = 0;

			io >> value;

			set_arg(dst1, pmode1, value);

			pc += 2;
		}
		break;
	case 4:
		{
			long long op1 = get_arg(memory[pc + 1], pmode1);

			io << op1;

			pc += 2;
		}
		break;
	case 5:
		{
			long long op1 = get_arg(memory[pc + 1], pmode1);
			long long op2 = get_arg(memory[pc + 2], pmode2);

			if (op1) {
				pc = op2;
			}
			else {
				pc += 3;
			}
		}
		break;
	case 6:
		{
			long long op1 = get_arg(memory[pc + 1], pmode1);
			long long op2 = get_arg(memory[pc + 2], pmode2);

			if (!op1) {
				pc = op2;
			}
			else {
				pc += 3;
			}
		}
		break;
	case 7:
		{
			long long op1 = get_arg(memory[pc + 1], pmode1);
			long long op2 = get_arg(memory[pc + 2], pmode2);
			long long dst3 = memory[pc + 3];

			set_arg(dst3, pmode3, op1 < op2);

			pc += 4;
		}
		break;
	case 8:
		{
			long long op1 = get_arg(memory[pc + 1], pmode1);
			long long op2 = get_arg(memory[pc + 2], pmode2);
			long long dst3 = memory[pc + 3];

			set_arg(dst3, pmode3, op1 == op2);

			pc += 4;
		}
		break;
	case 9:
		{
			long long op1 = get_arg(memory[pc + 1], pmode1);

			base += op1;

			pc += 2;
		}
		break;
	case 99:
		halt = true;
		break;
	default:
		std::cerr << "opcode error: " << opcode << std::endl;
		exit(1);
		break;
	}
}

struct RobotIO {
	// Movement instructions from playing around with the path generated
	// in part 1
	const std::string rules = "C,B,C,A,C,B,A,C,B,A\012"
	                          "R,12,R,4,L,12,L,12\012"
	                          "R,4,R,8,R,10,R,12\012"
	                          "R,8,R,10,R,10\012"
	                          "n\012";
	int rule_i = 0;

	bool operator>>(long long &rhs)
	{
		if (rule_i < rules.size()) {
			rhs = rules[rule_i++];
			return true;
		}
		return false;
	}

	bool operator<<(long long rhs)
	{
		if (rhs == 10 || (rhs < 256 && std::isprint(static_cast<unsigned char>(rhs)))) {
			std::cout << static_cast<char>(rhs);
		}
		else {
			std::cout << rhs << '\n';
		}
		return true;
	}
};

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cerr << "no program file\n";
		exit(1);
	}

	std::vector<long long> program = read_program(argv[1]);

	program[0] = 2;

	Computer<RobotIO> c(program);

	c.run();

	return 0;
}
