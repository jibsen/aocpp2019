//
// Advent of Code 2019, day 24, part one
//

#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>

// 5x5 grid with border of zeroes to make indexing easier
using Grid = std::array<std::array<int, 7>, 7>;

Grid read_grid(const char *filename)
{
	std::ifstream infile(filename);

	Grid grid;
	std::string line;
	int y = 1;

	for (auto &row : grid) {
		row.fill(0);
	}

	while (std::getline(infile, line)) {
		int x = 1;
		for (auto ch : line) {
			grid[y][x] = ch == '#';
			++x;
		}
		++y;
	}

	return grid;
}

uint64_t get_biodiversity(const Grid &grid)
{
	uint64_t rating = 0;
	uint64_t mask = 1;

	for (int y = 1; y < 6; ++y) {
		for (int x = 1; x < 6; ++x) {
			if (grid[y][x]) {
				rating |= mask;
			}

			mask <<= 1;
		}
	}

	return rating;
}

void update_grid(Grid &grid)
{
	Grid next;

	for (auto &row : next) {
		row.fill(0);
	}

	for (int y = 1; y < 6; ++y) {
		for (int x = 1; x < 6; ++x) {
			int adj = grid[y - 1][x] + grid[y][x - 1] + grid[y][x + 1] + grid[y + 1][x];
			next[y][x] = grid[y][x] ? adj == 1 : adj == 1 || adj == 2;
		}
	}

	grid.swap(next);
}

void print_grid(const Grid &grid)
{
	for (int y = 1; y < 6; ++y) {
		for (int x = 1; x < 6; ++x) {
			std::cout << (grid[y][x] ? '#' : '.');
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cerr << "no program file\n";
		exit(1);
	}

	Grid grid = read_grid(argv[1]);

	std::unordered_set<uint64_t> rating_seen;

	rating_seen.insert(get_biodiversity(grid));

	for (;;) {
		update_grid(grid);

		uint64_t rating = get_biodiversity(grid);

		if (auto [it, success] = rating_seen.insert(rating); !success) {
			print_grid(grid);

			std::cout << rating << '\n';
			exit(0);
		}
	}

	return 0;
}
