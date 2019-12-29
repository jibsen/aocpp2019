//
// Advent of Code 2019, day 13, part one
//

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct PairHash {
	template<typename T1, typename T2>
	std::size_t operator()(const std::pair<T1, T2> &p) const noexcept
	{
		std::size_t h1 = std::hash<T1>()(p.first);
		std::size_t h2 = std::hash<T2>()(p.second);

		return (17 * 37 + h1) * 37 + h2;
	}
};

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
	bool done = false;

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

int run_program(Computer &c)
{
	while (!c.done) {
		int opcode = static_cast<int>(c.memory[c.pc]);

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

				c.pc += 2;

				return static_cast<int>(op1);
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
			c.done = true;
			break;
		default:
			std::cerr << "opcode error: " << opcode << std::endl;
			exit(1);
			break;
		}
	}

	return -1;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cerr << "no program file\n";
		exit(1);
	}

	std::vector<long long> program = read_program(argv[1]);

	Computer c(program);

	std::unordered_map<std::pair<int, int>, int, PairHash> tiles;

	while (!c.done) {
		int x = run_program(c);
		int y = run_program(c);
		int id = run_program(c);
		tiles[{x, y}] = id + 1;
	}

	std::cout << std::count_if(tiles.begin(), tiles.end(),
		[](const auto &e) { return e.second == 3; }) << '\n';

	return 0;
}
