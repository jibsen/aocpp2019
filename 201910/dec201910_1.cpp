//
// Advent of Code 2019, day 10, part one
//

// If an asteroid is (x, y) away, and (dx, dy) is the smallest step of
// which (x, y) is a multiple, then asteroids on positions (x + k * dx,
// y + k * dy) are blocked.
//
// We can count asteroids in expanding squares around our current source,
// while removing those blocked by the ones we find.
//
// If the space were substantially larger than the number of asteroids, we
// could have sorted the asteroids by distance from current instead.

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::vector<int>> read_map(const char *filename)
{
	std::ifstream infile(filename);

	std::vector<std::vector<int>> map;
	std::string line;

	while (std::getline(infile, line)) {
		std::vector<int> row;

		for (auto ch : line) {
			row.push_back(ch == '#');
		}

		map.push_back(row);
	}

	return map;
}

// Find smallest (dx, dy) step of which (x, y) is a multiple
constexpr std::pair<int, int> simplify(int x, int y)
{
	for (;;) {
		int d = std::gcd(x, y);
		if (d == 1) {
			break;
		}
		x /= d;
		y /= d;
	}
	return {x, y};
}

// Remove (x, y) and all asterids on the straight line from (ax, ay) through
// (x, y) blocked by (x, y)
void remove_blocked(std::vector<std::vector<int>> &map, int ax, int ay, int x, int y)
{
	auto [dx, dy] = simplify(x - ax, y - ay);

	while (y >= 0 && y < map.size() && x >= 0 && x < map[0].size()) {
		map[y][x] = 0;
		y += dy;
		x += dx;
	}
}

int count_visible(std::vector<std::vector<int>> map, int ax, int ay)
{
	int visible = 0;

	int max_d = std::max(std::max(ax + 1, static_cast<int>(map[0].size()) - ax),
	                     std::max(ay + 1, static_cast<int>(map.size() - ay)));

	// For each square around (ax, ay) with increasing size, count asteroids
	// and remove any blocked by them
	for (int d = 1; d < max_d; ++d) {
		if (ay - d >= 0) {
			int max_x = std::min(ax + d + 1, static_cast<int>(map[0].size()));

			for (int x = std::max(ax - d, 0); x < max_x; ++x)
			{
				if (map[ay - d][x]) {
					++visible;
					remove_blocked(map, ax, ay, x, ay - d);
				}
			}
		}

		if (ax - d >= 0) {
			int max_y = std::min(ay + d + 1, static_cast<int>(map.size()));

			for (int y = std::max(ay - d, 0); y < max_y; ++y)
			{
				if (map[y][ax - d]) {
					++visible;
					remove_blocked(map, ax, ay, ax - d, y);
				}
			}
		}

		if (ay + d < map.size()) {
			int max_x = std::min(ax + d + 1, static_cast<int>(map[0].size()));

			for (int x = std::max(ax - d, 0); x < max_x; ++x)
			{
				if (map[ay + d][x]) {
					++visible;
					remove_blocked(map, ax, ay, x, ay + d);
				}
			}
		}

		if (ax + d < map[0].size()) {
			int max_y = std::min(ay + d + 1, static_cast<int>(map.size()));

			for (int y = std::max(ay - d, 0); y < max_y; ++y)
			{
				if (map[y][ax + d]) {
					++visible;
					remove_blocked(map, ax, ay, ax + d, y);
				}
			}
		}
	}

	return visible;
}

int main(int argc, char *argv[])
{

	if (argc != 2) {
		std::cerr << "no program file\n";
		exit(1);
	}

	auto map = read_map(argv[1]);

	int max_visible = std::numeric_limits<int>::min();
	int max_x = -1;
	int max_y = -1;

	for (int y = 0; y < map.size(); ++y) {
		for (int x = 0; x < map[y].size(); ++x) {
			if (map[y][x]) {
				int visible = count_visible(map, x, y);
				if (visible > max_visible) {
					max_visible = visible;
					max_x = x;
					max_y = y;
				}
			}
		}
	}

	std::cout << max_x << ',' << max_y << " with " << max_visible << '\n';

	return 0;
}
