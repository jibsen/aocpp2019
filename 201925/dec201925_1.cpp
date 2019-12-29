//
// Advent of Code 2019, day 25, part one
//

// If you use HumanIO, you get to play the awesome text adventure. Doing
// this, it is fairly easy to draw up a map of the maze and items and
// create a route that picks up all items and finds the checkpoint room.
//
// The DroidIO starts by going this route, and then tries all combinations
// of items. We could have used the information returned about the current
// weight, but there are only 8! combinations.

#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <tuple>
#include <unordered_set>
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

struct DroidIO {
	// Instructions for picking up every item on the way to the checkpoint
	std::string input =
		"south\012"
		"take whirled peas\012"

		"south\012"
		"south\012"
		"south\012"
		"take festive hat\012"

		"north\012"
		"north\012"
		"north\012"
		"north\012"
		"west\012"
		"take pointer\012"

		"east\012"
		"north\012"
		"take coin\012"

		"north\012"
		"take astronaut ice cream\012"

		"north\012"
		"west\012"
		"take dark matter\012"

		"south\012"
		"take klein bottle\012"

		"west\012"
		"take mutex\012"

		"west\012"
		"south\012"

		"inv\012";

	std::vector<std::string> items = {
		"astronaut ice cream",
		"coin",
		"dark matter",
		"festive hat",
		"klein bottle",
		"mutex",
		"pointer",
		"whirled peas",
	};
	int item_idx = items.size();

	bool operator>>(long long &rhs)
	{
		if (input.empty()) {
			if (item_idx == items.size()) {
				for (const auto &i : items) {
					input += "take " + i + "\012";
				}
				std::reverse(input.begin(), input.end());

				// Find next permutation which drops the pointer before the mutex
				do {
					std::next_permutation(items.begin(), items.end());
				} while (std::find(items.begin(), items.end(), "mutex") < std::find(items.begin(), items.end(), "pointer"));

				item_idx = 0;
			}
			else {
				input = "drop " + items[item_idx] + "\012east\012";
				std::reverse(input.begin(), input.end());
				item_idx++;
			}
		}

		rhs = input.back();
		input.pop_back();

		return true;
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

	DroidIO() { std::reverse(input.begin(), input.end()); }
};

struct HumanIO {
	std::string input;

	bool operator>>(long long &rhs)
	{
		if (input.empty()) {
			std::getline(std::cin, input);
			input.push_back('\012');
			std::reverse(input.begin(), input.end());
		}

		rhs = input.back();
		input.pop_back();

		return true;
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

	Computer<DroidIO> c(program);

	c.run();

	return 0;
}
