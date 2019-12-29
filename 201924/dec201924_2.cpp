//
// Advent of Code 2019, day 24, part two
//

#include <algorithm>
#include <array>
#include <deque>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
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

int get_adj(std::deque<Grid>::const_iterator it, int x, int y)
{
	const auto &prev = *std::prev(it);
	const auto &next = *std::next(it);

	int adj = (*it)[y - 1][x] + (*it)[y][x - 1] + (*it)[y][x + 1] + (*it)[y + 1][x];

	// If we are on the outer edge, add adjacent tiles from previous grid
	if (x == 1) {
		adj += prev[3][2];
	}
	if (y == 1) {
		adj += prev[2][3];
	}
	if (x == 5) {
		adj += prev[3][4];
	}
	if (y == 5) {
		adj += prev[4][3];
	}

	// If we are next to middle, add adjacent tiles from next grid
	if (x == 3 && y == 2) {
		adj += next[1][1] + next[1][2] + next[1][3] + next[1][4] + next[1][5];
	}
	if (x == 2 && y == 3) {
		adj += next[1][1] + next[2][1] + next[3][1] + next[4][1] + next[5][1];
	}
	if (x == 3 && y == 4) {
		adj += next[5][1] + next[5][2] + next[5][3] + next[5][4] + next[5][5];
	}
	if (x == 4 && y == 3) {
		adj += next[1][5] + next[2][5] + next[3][5] + next[4][5] + next[5][5];
	}

	return adj;
}

void update_grids(std::deque<Grid> &grids)
{
	Grid empty;

	for (auto &row : empty) {
		row.fill(0);
	}

	// Add two empty grids at front and back so we do not have to check
	grids.push_front(empty);
	grids.push_front(empty);
	grids.push_back(empty);
	grids.push_back(empty);

	std::deque<Grid> next;

	for (auto it = std::next(grids.begin()); it < std::prev(grids.end()); ++it) {
		Grid g;

		for (auto &row : g) {
			row.fill(0);
		}

		for (int y = 1; y < 6; ++y) {
			for (int x = 1; x < 6; ++x) {
				if (x == 3 && y == 3) {
					continue;
				}
				int adj = get_adj(it, x, y);
				g[y][x] = (*it)[y][x] ? adj == 1 : adj == 1 || adj == 2;
			}
		}

		next.push_back(g);
	}

	grids.swap(next);
}

std::size_t count_bugs(const std::deque<Grid> &grids)
{
	std::size_t count = 0;

	for (const auto &grid : grids) {
		for (const auto &row : grid) {
			count += std::accumulate(row.begin(), row.end(), 0);
		}
	}

	return count;
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

	std::deque<Grid> grids;
	grids.push_back(read_grid(argv[1]));

	for (int i = 0; i < 200; ++i) {
		update_grids(grids);
	}

	std::cout << count_bugs(grids) << '\n';

	return 0;
}
