//
// Advent of Code 2019, day 15, part one
//

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <queue>
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

using Map = std::unordered_map<std::pair<int, int>, int, PairHash>;

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

int run_droid(Computer &c, int input)
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

				c.set_arg(dst1, pmode1, input);

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

// Recursively discover map of the maze
void discover_map(Computer &c, Map &map, int x, int y)
{
	if (map[{x, y + 1}] == 0) {
		int north_reply = run_droid(c, 1);

		map[{x, y + 1}] = north_reply + 1;

		if (north_reply) {
			discover_map(c, map, x, y + 1);

			run_droid(c, 2);
		}
	}

	if (map[{x, y - 1}] == 0) {
		int south_reply = run_droid(c, 2);

		map[{x, y - 1}] = south_reply + 1;

		if (south_reply) {
			discover_map(c, map, x, y - 1);

			run_droid(c, 1);
		}
	}

	if (map[{x - 1, y}] == 0) {
		int west_reply = run_droid(c, 3);

		map[{x - 1, y}] = west_reply + 1;

		if (west_reply) {
			discover_map(c, map, x - 1, y);

			run_droid(c, 4);
		}
	}

	if (map[{x + 1, y}] == 0) {
		int east_reply = run_droid(c, 4);

		map[{x + 1, y}] = east_reply + 1;

		if (east_reply) {
			discover_map(c, map, x + 1, y);

			run_droid(c, 3);
		}
	}
}

// Find length of shortest path from start_x, start_y to oxygen system
int find_shortest_path(const std::array<std::array<int, 50>, 50> &maze, int start_x, int start_y)
{
	std::unordered_map<std::pair<int, int>, int, PairHash> distance;
	std::queue<std::pair<int, int>> queue;

	queue.push({start_x, start_y});
	distance[{start_x, start_y}] = 0;

	// BFS computing min distance from start_x, start_y
	while (!queue.empty()) {
		auto [x, y] = queue.front();
		queue.pop();

		int cur_dist = distance[{x, y}];

		if (maze[y][x] == 3) {
			return cur_dist;
		}

		if (maze[y + 1][x] > 1 && distance[{x, y + 1}] == 0) {
			distance[{x, y + 1}] = cur_dist + 1;
			queue.push({x, y + 1});
		}

		if (maze[y - 1][x] > 1 && distance[{x, y - 1}] == 0) {
			distance[{x, y - 1}] = cur_dist + 1;
			queue.push({x, y - 1});
		}

		if (maze[y][x - 1] > 1 && distance[{x - 1, y}] == 0) {
			distance[{x - 1, y}] = cur_dist + 1;
			queue.push({x - 1, y});
		}

		if (maze[y][x + 1] > 1 && distance[{x + 1, y}] == 0) {
			distance[{x + 1, y}] = cur_dist + 1;
			queue.push({x + 1, y});
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

	Map map;

	// Maze coordinates are within [-25, 25] from the start, so we
	// start at 25, 25 to get coordinates in [0, 50]
	map[{25, 25}] = 2;

	discover_map(c, map, 25, 25);

	std::array<std::array<int, 50>, 50> maze;

	for (auto &row : maze) {
		row.fill(0);
	}

	for (const auto &[k, v] : map) {
		maze[k.second][k.first] = v;
	}

	static const char tiles[] = " #.O";

	for (const auto &row : maze) {
		for (auto tile_id : row) {
			std::cout << tiles[tile_id];
		}
		std::cout << '\n';
	}

	std::cout << find_shortest_path(maze, 25, 25) << '\n';

	return 0;
}
