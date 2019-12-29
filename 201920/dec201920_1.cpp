//
// Advent of Code 2019, day 20, part one
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

struct PairHash {
	template<typename T1, typename T2>
	std::size_t operator()(const std::pair<T1, T2> &p) const noexcept
	{
		std::size_t h1 = std::hash<T1>()(p.first);
		std::size_t h2 = std::hash<T2>()(p.second);

		return (17 * 37 + h1) * 37 + h2;
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

using Portals = std::map<std::pair<int, int>, std::pair<int, int>>;

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
						portals[{x - 1, y}] = (*it).second;
						portals[(*it).second] = {x - 1, y};
					}
				}
				else {
					// AB.
					auto [it, success] = doors.insert({{map[y][x], map[y][x + 1]}, {x + 2, y}});
					if (!success) {
						portals[{x + 2, y}] = (*it).second;
						portals[(*it).second] = {x + 2, y};
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
						portals[{x, y - 1}] = (*it).second;
						portals[(*it).second] = {x, y - 1};
					}
				}
				else {
					// A
					// B
					// .
					auto [it, success] = doors.insert({{map[y][x], map[y + 1][x]}, {x, y + 2}});
					if (!success) {
						portals[{x, y + 2}] = (*it).second;
						portals[(*it).second] = {x, y + 2};
					}
				}
				map[y][x] = ' ';
				map[y + 1][x] = ' ';
			}
		}
	}

	return {portals, doors[{'A', 'A'}], doors[{'Z', 'Z'}]};
}

int find_shortest_path(const std::vector<std::string> &map, Portals &portals, int start_x, int start_y, int end_x, int end_y)
{
	std::unordered_map<std::pair<int, int>, int, PairHash> distance;
	std::queue<std::pair<int, int>> queue;

	queue.push({start_x, start_y});
	distance[{start_x, start_y}] = 0;

	// BFS computing min distance from start_x, start_y
	while (!queue.empty()) {
		auto [x, y] = queue.front();
		queue.pop();

		if (x == end_x && y == end_y) {
			break;
		}

		int cur_dist = distance[{x, y}];

		if (auto it = portals.find({x, y}); it != portals.end() && distance[(*it).second] == 0) {
			distance[(*it).second] = cur_dist + 1;
			queue.push((*it).second);
		}

		if (map[y + 1][x] == '.' && distance[{x, y + 1}] == 0) {
			distance[{x, y + 1}] = cur_dist + 1;
			queue.push({x, y + 1});
		}

		if (map[y - 1][x] == '.' && distance[{x, y - 1}] == 0) {
			distance[{x, y - 1}] = cur_dist + 1;
			queue.push({x, y - 1});
		}

		if (map[y][x - 1] == '.' && distance[{x - 1, y}] == 0) {
			distance[{x - 1, y}] = cur_dist + 1;
			queue.push({x - 1, y});
		}

		if (map[y][x + 1] == '.' && distance[{x + 1, y}] == 0) {
			distance[{x + 1, y}] = cur_dist + 1;
			queue.push({x + 1, y});
		}
	}

	return distance[{end_x, end_y}];
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
