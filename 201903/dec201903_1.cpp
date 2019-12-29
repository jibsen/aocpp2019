//
// Advent of Code 2019, day 3, part one
//

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using Point = std::pair<int, int>;

std::vector<Point> read_wire_path()
{
	std::vector<Point> path;
	std::string line;
	int distance = 0;
	char direction = 0;
	int x = 0;
	int y = 0;

	std::getline(std::cin, line);
	std::istringstream in(line);

	for (char sep = ','; sep == ',' && in >> direction >> distance; in >> sep) {
		int dx = (direction == 'R') - (direction == 'L');
		int dy = (direction == 'U') - (direction == 'D');

		for (int i = 0; i < distance; ++i) {
			x += dx;
			y += dy;
			path.push_back({x, y});
		}
	}

	return path;
}

int main()
{
	auto wire1 = read_wire_path();
	auto wire2 = read_wire_path();

	std::sort(wire1.begin(), wire1.end());
	std::sort(wire2.begin(), wire2.end());

	std::vector<Point> crossings;

	std::set_intersection(wire1.begin(), wire1.end(),
	                      wire2.begin(), wire2.end(),
			      std::back_inserter(crossings));

	int min_distance = std::numeric_limits<int>::max();

	for (const auto &[x, y] : crossings) {
		min_distance = std::min(min_distance, std::abs(x) + std::abs(y));
	}

	std::cout << min_distance << '\n';

	return 0;
}
