//
// Advent of Code 2019, day 10, part two
//

// We sort the asteroids first by angle from starting position of laser,
// and then by distance. Asteroids at the same angle (blocked) are assigned
// increasing round numbers by distance.

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

constexpr double PI = 3.14159265358979323846;

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

// Assign increasing round numbers to asterids on the straight line from
// (ax, ay) through (x, y) blocked by (x, y)
void update_blocked(std::vector<std::vector<int>> &map, int ax, int ay, int x, int y)
{
	auto [dx, dy] = simplify(x - ax, y - ay);
	int round = 1;

	while (y >= 0 && y < map.size() && x >= 0 && x < map[0].size()) {
		if (map[y][x] == 1) {
			map[y][x] = round++;
		}
		y += dy;
		x += dx;
	}
}

double dist(int x1, int y1, int x2, int y2)
{
	return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

// Find angle between the line from (x1, y1) to (x2, y2) and the Y axis,
// as a value between 0 and 2 * pi
double angle(int x1, int y1, int x2, int y2)
{
	double a = std::acos((y1 - y2) / dist(x1, y1, x2, y2));
	return x2 < x1 ? 2 * PI - a : a;
}

int main(int argc, char *argv[])
{

	if (argc != 2) {
		std::cerr << "no program file\n";
		exit(1);
	}

	auto map = read_map(argv[1]);

	const int ax = 29;
	const int ay = 28;

	bool done = false;

	map[ay][ax] = 0;
	int num_destroyed = 0;

	for (int round = 1; !done; ++round) {
		std::vector<std::tuple<double, double, int, int>> points;

		done = true;

		// Collect all asteroids that belong to this round
		for (int y = 0; y < map.size(); ++y) {
			for (int x = 0; x < map[0].size(); ++x) {
				if (map[y][x] == round) {
					done = false;
					points.emplace_back(angle(ax, ay, x, y), dist(ax, ay, x, y), x, y);
				}
			}
		}

		// Sort first by angle from starting position of laser, and then by distance
		std::sort(points.begin(), points.end(), [](const auto &lhs, const auto &rhs) {
			// We need an epsilon when comparing angles for equality
			// due to floating point errors
			if (std::abs(std::get<0>(lhs) - std::get<0>(rhs)) > 0.0000001) {
				return std::get<0>(lhs) < std::get<0>(rhs);
			}
			return std::get<1>(lhs) < std::get<1>(rhs);
		});

		// Destroy asteroids in order of angle, updating the round of
		// ones that are blocked
		for (auto [a, r, x, y] : points) {
			if (map[y][x] == round) {
				update_blocked(map, ax, ay, x, y);
				++num_destroyed;
				if (num_destroyed == 200) {
					std::cout << x * 100 + y << '\n';
				}
			}
		}
	}

	return 0;
}
