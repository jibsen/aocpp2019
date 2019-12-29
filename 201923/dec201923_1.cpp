//
// Advent of Code 2019, day 23, part one
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <tuple>
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
	bool booted = false;
	int id = 0;

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
	IODevice io;

	Computer(std::vector<long long> program, int addr)
	 : memory(std::move(program)), id(addr) {}

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

			if (!booted) {
				set_arg(dst1, pmode1, id);

				booted = true;

				pc += 2;

				break;
			}

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

struct NetworkIO {
	std::queue<std::pair<long long, long long>> packets_in;
	std::queue<std::tuple<long long, long long, long long>> packets_out;
	std::queue<long long> input;
	std::vector<long long> output;

	bool operator>>(long long &rhs)
	{
		if (input.empty()) {
			if (packets_in.empty()) {
				rhs = -1;
				return true;
			}

			auto [x, y] = packets_in.front();
			packets_in.pop();

			input.push(x);
			input.push(y);
		}

		rhs = input.front();
		input.pop();

		return true;
	}

	bool operator<<(long long rhs)
	{
		output.push_back(rhs);

		if (output.size() == 3) {
			packets_out.push({output[0], output[1], output[2]});
			output.clear();
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

	std::vector<Computer<NetworkIO>> computers;

	computers.reserve(50);

	for (int i = 0; i < 50; ++i) {
		computers.emplace_back(program, i);
	}

	for (;;) {
		for (int i = 0; i < 50; ++i) {
			computers[i].step();

			if (!computers[i].io.packets_out.empty()) {
				auto [addr, x, y] = computers[i].io.packets_out.front();
				computers[i].io.packets_out.pop();

				if (addr == 255) {
					std::cout << y << '\n';
					exit(0);
				}

				computers[addr].io.packets_in.push({x, y});
			}
		}
	}

	return 0;
}
