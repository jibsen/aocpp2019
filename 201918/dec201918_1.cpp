//
// Advent of Code 2019, day 18, part one
//

// We can do a recursive search for the shortest path by finding the shortest
// path to each reachable key, and then try each of those possibilities.
//
// Since doing keys 'abc' and 'bac' both result in being at key c with those
// three keys available, we can memorize the shortest path for the rest of
// the search given a position and set of keys.

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
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

bool have_key(std::uint32_t keys, char k)
{
	return std::isupper(k) && (keys & (UINT32_C(1) << (std::tolower(k) - 'a')));
}

void add_key(std::uint32_t &keys, char k)
{
	keys |= UINT32_C(1) << (std::tolower(k) - 'a');
}

void remove_key(std::uint32_t &keys, char k)
{
	keys &= ~(UINT32_C(1) << (std::tolower(k) - 'a'));
}

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

std::vector<std::tuple<int, int, int>> find_reachable_keys(const std::vector<std::string> &map, std::uint32_t &keys, int start_x, int start_y)
{
	std::unordered_map<std::pair<int, int>, int, PairHash> distance;
	std::queue<std::pair<int, int>> queue;

	queue.push({start_x, start_y});
	distance[{start_x, start_y}] = 0;

	std::vector<std::tuple<int, int, int>> options;

	// BFS computing min distance from start_x, start_y
	while (!queue.empty()) {
		auto [x, y] = queue.front();
		queue.pop();

		int cur_dist = distance[{x, y}];

		if (std::islower(map[y][x])) {
			options.push_back({x, y, cur_dist});
			continue;
		}

		if ((map[y + 1][x] == '.' || std::islower(map[y + 1][x]) || have_key(keys, map[y + 1][x]))
		 && distance[{x, y + 1}] == 0) {
			distance[{x, y + 1}] = cur_dist + 1;
			queue.push({x, y + 1});
		}

		if ((map[y - 1][x] == '.' || std::islower(map[y - 1][x]) || have_key(keys, map[y - 1][x]))
		 && distance[{x, y - 1}] == 0) {
			distance[{x, y - 1}] = cur_dist + 1;
			queue.push({x, y - 1});
		}

		if ((map[y][x - 1] == '.' || std::islower(map[y][x - 1]) || have_key(keys, map[y][x - 1]))
		 && distance[{x - 1, y}] == 0) {
			distance[{x - 1, y}] = cur_dist + 1;
			queue.push({x - 1, y});
		}

		if ((map[y][x + 1] == '.' || std::islower(map[y][x + 1]) || have_key(keys, map[y][x + 1]))
		 && distance[{x + 1, y}] == 0) {
			distance[{x + 1, y}] = cur_dist + 1;
			queue.push({x + 1, y});
		}
	}

	return options;
}

using Memory = std::unordered_map<std::tuple<int, int, std::uint32_t>, int, TupleHash>;

int find_shortest_key_path(Memory &mem, std::vector<std::string> &map, std::uint32_t &keys, int start_x, int start_y, int length)
{
	if (auto it = mem.find({start_x, start_y, keys}); it != mem.end()) {
		return length + (*it).second;
	}

	auto options = find_reachable_keys(map, keys, start_x, start_y);

	if (options.empty()) {
		return length;
	}

	int min_len = std::numeric_limits<int>::max();

	for (auto [x, y, dist] : options) {
		char k = map[y][x];

		add_key(keys, k);
		map[y][x] = '.';

		int new_len = find_shortest_key_path(mem, map, keys, x, y, length + dist);
		min_len = std::min(min_len, new_len);

		map[y][x] = k;
		remove_key(keys, k);
	}

	mem[{start_x, start_y, keys}] = min_len - length;

	return min_len;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cerr << "no program file\n";
		exit(1);
	}

	auto map = read_map(argv[1]);

	int start_x = 0;
	int start_y = 0;

	for (int y = 0; y < map.size(); ++y) {
		for (int x = 0; x < map[y].size(); ++x) {
			if (map[y][x] == '@') {
				start_x = x;
				start_y = y;
			}
		}
	}

	std::cout << start_x << ',' << start_y << '\n';

	map[start_y][start_x] = '.';

	std::uint32_t keys = 0;
	Memory mem;

	std::cout << find_shortest_key_path(mem, map, keys, start_x, start_y, 0) << '\n';

	return 0;
}
