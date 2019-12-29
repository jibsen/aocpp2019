//
// Advent of Code 2019, day 3, part two
//

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>

struct PairHash {
	template<typename T1, typename T2>
	std::size_t operator()(const std::pair<T1, T2> &p) const noexcept
	{
		std::size_t h1 = std::hash<T1>()(p.first);
		std::size_t h2 = std::hash<T2>()(p.second);

		return (17 * 37 + h1) * 37 + h2;
	}
};

using Path = std::unordered_map<std::pair<int, int>, int, PairHash>;

Path read_wire_path()
{
	Path path;
	std::string line;
	int distance = 0;
	char direction = 0;
	int x = 0;
	int y = 0;
	int steps = 0;

	std::getline(std::cin, line);
	std::istringstream in(line);

	for (char sep = ','; sep == ',' && in >> direction >> distance; in >> sep) {
		int dx = (direction == 'R') - (direction == 'L');
		int dy = (direction == 'U') - (direction == 'D');

		for (int i = 0; i < distance; ++i) {
			x += dx;
			y += dy;
			// Since insert fails if the key is already present,
			// each point will map to the lowest number of steps
			path.insert({{x, y}, ++steps});
		}
	}

	return path;
}

int main()
{
	auto wire1 = read_wire_path();
	auto wire2 = read_wire_path();

	int min_steps = std::numeric_limits<int>::max();

	for (const auto &[point, steps] : wire1) {
		if (auto it = wire2.find(point); it != wire2.end()) {
			min_steps = std::min(min_steps, steps + (*it).second);
		}
	}

	std::cout << min_steps << '\n';

	return 0;
}
