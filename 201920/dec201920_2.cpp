//
// Advent of Code 2019, day 20, part two
//

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

struct TupleHash {
	template<typename... Ts>
	std::size_t operator()(const std::tuple<Ts...> &tuple) const noexcept
	{
		std::size_t hash = 17;

		std::apply([&hash](const auto &... args) {
			(..., (hash = hash * 37 + std::hash<Ts>()(args)));
		}, tuple);

		return hash;
	}
};

std::vector<std::string> read_map(const char *filename)
{
	std::ifstream infile(filename);

	std::vector<std::string> map;
	std::string line;

	while (std::getline(infile, line)) {
		map.push_back(line);
	}

	return map;
}

using Portals = std::map<std::pair<int, int>, std::pair<std::pair<int, int>, int>>;

std::tuple<Portals, std::pair<int, int>, std::pair<int, int>> find_portals(std::vector<std::string> &map)
{
	std::map<std::pair<char, char>, std::pair<int, int>> doors;
	Portals portals;

	// Portal names are read left to right and top to bottom, so scan like
	// that, removing the two letters when a portal is found
	for (int y = 0; y < map.size(); ++y) {
		for (int x = 0; x < map[y].size(); ++x) {
			if (!std::isupper(map[y][x])) {
				continue;
			}

			if (x + 1 < map[y].size() && std::isupper(map[y][x + 1])) {
				if (x > 0 && map[y][x - 1] == '.') {
					// .AB
					auto [it, success] = doors.insert({{map[y][x], map[y][x + 1]}, {x - 1, y}});
					if (!success) {
						int direction = x < map[y].size() / 2 ? 1 : -1;
						portals[{x - 1, y}] = {(*it).second, direction};
						portals[(*it).second] = {{x - 1, y}, -direction};
					}
				}
				else {
					// AB.
					auto [it, success] = doors.insert({{map[y][x], map[y][x + 1]}, {x + 2, y}});
					if (!success) {
						int direction = x < map[y].size() / 2 ? -1 : 1;
						portals[{x + 2, y}] = {(*it).second, direction};
						portals[(*it).second] = {{x + 2, y}, -direction};
					}
				}
				map[y][x] = ' ';
				map[y][x + 1] = ' ';
			}
			else {
				if (y > 0 && map[y - 1][x] == '.') {
					// .
					// A
					// B
					auto [it, success] = doors.insert({{map[y][x], map[y + 1][x]}, {x, y - 1}});
					if (!success) {
						int direction = y < map.size() / 2 ? 1 : -1;
						portals[{x, y - 1}] = {(*it).second, direction};
						portals[(*it).second] = {{x, y - 1}, -direction};
					}
				}
				else {
					// A
					// B
					// .
					auto [it, success] = doors.insert({{map[y][x], map[y + 1][x]}, {x, y + 2}});
					if (!success) {
						int direction = y < map.size() / 2 ? -1 : 1;
						portals[{x, y + 2}] = {(*it).second, direction};
						portals[(*it).second] = {{x, y + 2}, -direction};
					}
				}
				map[y][x] = ' ';
				map[y + 1][x] = ' ';
			}
		}
	}

	return {portals, doors[{'A', 'A'}], doors[{'Z', 'Z'}]};
}

int find_shortest_path(const std::vector<std::string> &map, const Portals &portals, int start_x, int start_y, int end_x, int end_y)
{
	std::unordered_map<std::tuple<int, int, int>, int, TupleHash> distance;
	std::queue<std::tuple<int, int, int>> queue;

	queue.push({start_x, start_y, 0});
	distance[{start_x, start_y, 0}] = 0;

	// BFS computing min distance from start_x, start_y
	while (!queue.empty()) {
		auto [x, y, level] = queue.front();
		queue.pop();

		if (level == 0 && x == end_x && y == end_y) {
			break;
		}

		int cur_dist = distance[{x, y, level}];

		if (auto it = portals.find({x, y}); it != portals.end()) {
			int direction = (*it).second.second;

			if ((direction > 0 || level > 0)
			 && distance[{(*it).second.first.first, (*it).second.first.second, level + direction}] == 0) {
				distance[{(*it).second.first.first, (*it).second.first.second, level + direction}] = cur_dist + 1;
				queue.push({(*it).second.first.first, (*it).second.first.second, level + direction});
			}
		}

		if (map[y + 1][x] == '.' && distance[{x, y + 1, level}] == 0) {
			distance[{x, y + 1, level}] = cur_dist + 1;
			queue.push({x, y + 1, level});
		}

		if (map[y - 1][x] == '.' && distance[{x, y - 1, level}] == 0) {
			distance[{x, y - 1, level}] = cur_dist + 1;
			queue.push({x, y - 1, level});
		}

		if (map[y][x - 1] == '.' && distance[{x - 1, y, level}] == 0) {
			distance[{x - 1, y, level}] = cur_dist + 1;
			queue.push({x - 1, y, level});
		}

		if (map[y][x + 1] == '.' && distance[{x + 1, y, level}] == 0) {
			distance[{x + 1, y, level}] = cur_dist + 1;
			queue.push({x + 1, y, level});
		}
	}

	return distance[{end_x, end_y, 0}];
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cerr << "no program file\n";
		exit(1);
	}

	auto map = read_map(argv[1]);

	auto [portals, start, end] = find_portals(map);

	std::cout << find_shortest_path(map, portals, start.first, start.second, end.first, end.second) << '\n';

	return 0;
}
