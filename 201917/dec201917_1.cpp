//
// Advent of Code 2019, day 17, part one
//

#include <algorithm>
#include <array>
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

int run_robot(Computer &c)
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

char get_char_at(const std::vector<std::string> &map, int x, int y)
{
	if (y < 0 || y >= map.size()) {
		return 0;
	}
	if (x < 0 || x >= map[y].size()) {
		return 0;
	}
	return map[y][x];
}

bool is_path_or_robot(char c)
{
	return c == '#' || c == '^' || c == 'v' || c == '<' || c == '>';
}

int num_neighbors(const std::vector<std::string> &map, int x, int y)
{
	return is_path_or_robot(get_char_at(map, x, y - 1)) +
	       is_path_or_robot(get_char_at(map, x, y + 1)) +
	       is_path_or_robot(get_char_at(map, x - 1, y)) +
	       is_path_or_robot(get_char_at(map, x + 1, y));
}

// Note: there are a number of special cases the rules for part one do not
// cover, like start and end point on a line. This covers these, but not the
// special case of start and end point being the same:
//
// ###
// #.#
// ##^
//
int sum_alignment_parameters(const std::vector<std::string> &map)
{
	int sum = 0;

	for (int y = 0; y < map.size(); ++y) {
		for (int x = 0; x < map[y].size(); ++x) {
			if (is_path_or_robot(map[y][x])) {
				if (num_neighbors(map, x, y) >= 3) {
					sum += x * y;
				}
			}
		}
	}

	return sum;
}

enum class Direction {
	up, down, left, right
};

void print_robot_path(const std::vector<std::string> &map)
{
	int x = 0;
	int y = 0;
	Direction direction = Direction::up;

	// Find robot coordinates
	for (int iy = 0; iy < map.size(); ++iy) {
		for (int ix = 0; ix < map[iy].size(); ++ix) {
			if (map[iy][ix] == '^') {
				x = ix;
				y = iy;
			}
		}
	}

	for (;;) {
		// Move as far as we can in the current direction
		switch (direction) {
		case Direction::up:
			{
				int steps = 0;

				while (get_char_at(map, x, y - 1) == '#') {
					++steps;
					--y;
				}

				if (steps) {
					std::cout << steps << ',';
				}

				break;
			}
		case Direction::down:
			{
				int steps = 0;

				while (get_char_at(map, x, y + 1) == '#') {
					++steps;
					++y;
				}

				if (steps) {
					std::cout << steps << ',';
				}

				break;
			}
		case Direction::left:
			{
				int steps = 0;

				while (get_char_at(map, x - 1, y) == '#') {
					++steps;
					--x;
				}

				if (steps) {
					std::cout << steps << ',';
				}

				break;
			}
		case Direction::right:
			{
				int steps = 0;

				while (get_char_at(map, x + 1, y) == '#') {
					++steps;
					++x;
				}

				if (steps) {
					std::cout << steps << ',';
				}

				break;
			}
		}

		// Attempt to turn
		if (direction == Direction::up) {
			if (get_char_at(map, x - 1, y) == '#') {
				std::cout << "L,";
				direction = Direction::left;
			}
			else if (get_char_at(map, x + 1, y) == '#') {
				std::cout << "R,";
				direction = Direction::right;
			}
			else {
				break;
			}
		}
		else if (direction == Direction::down) {
			if (get_char_at(map, x - 1, y) == '#') {
				std::cout << "R,";
				direction = Direction::left;
			}
			else if (get_char_at(map, x + 1, y) == '#') {
				std::cout << "L,";
				direction = Direction::right;
			}
			else {
				break;
			}
		}
		else if (direction == Direction::left) {
			if (get_char_at(map, x, y + 1) == '#') {
				std::cout << "L,";
				direction = Direction::down;
			}
			else if (get_char_at(map, x, y - 1) == '#') {
				std::cout << "R,";
				direction = Direction::up;
			}
			else {
				break;
			}
		}
		else if (direction == Direction::right) {
			if (get_char_at(map, x, y + 1) == '#') {
				std::cout << "R,";
				direction = Direction::down;
			}
			else if (get_char_at(map, x, y - 1) == '#') {
				std::cout << "L,";
				direction = Direction::up;
			}
			else {
				break;
			}
		}
	}

	std::cout << '\n';
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cerr << "no program file\n";
		exit(1);
	}

	std::vector<long long> program = read_program(argv[1]);

	Computer c(program);

	std::vector<std::string> map;

	for (;;) {
		int ch = run_robot(c);

		if (ch == -1) {
			break;
		}

		std::string line;

		do {
			line.push_back(ch);
			ch = run_robot(c);
		} while (ch != 10 && ch != -1);

		std::cout << line << '\n';

		map.push_back(line);

		if (ch == -1) {
			break;
		}
	}

	std::cout << sum_alignment_parameters(map) << '\n';

	print_robot_path(map);

	return 0;
}
