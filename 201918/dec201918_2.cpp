//
// Advent of Code 2019, day 18, part two
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

using Memory = std::map<std::tuple<int, int, int, int, int, int, int, int, std::uint32_t>, int>;

int find_shortest_key_path(Memory &mem, std::vector<std::string> &map, std::uint32_t &keys, std::uint32_t all_keys, const std::vector<std::pair<int, int>> &pos, int length)
{
	if (keys == all_keys) {
		return length;
	}

	if (auto it = mem.find({pos[0].first, pos[0].second,
	                        pos[1].first, pos[1].second,
	                        pos[2].first, pos[2].second,
	                        pos[3].first, pos[3].second, keys}); it != mem.end()) {
		return length + (*it).second;
	}

	int min_len = std::numeric_limits<int>::max();

	for (int i = 0; i < pos.size(); ++i) {
		int start_x = pos[i].first;
		int start_y = pos[i].second;

		auto options = find_reachable_keys(map, keys, start_x, start_y);

		if (options.empty()) {
			continue;
		}

		std::vector<std::pair<int, int>> new_pos = pos;

		for (auto [x, y, dist] : options) {
			char k = map[y][x];

			add_key(keys, k);
			map[y][x] = '.';

			new_pos[i] = {x, y};

			int new_len = find_shortest_key_path(mem, map, keys, all_keys, new_pos, length + dist);
			min_len = std::min(min_len, new_len);

			map[y][x] = k;
			remove_key(keys, k);
		}
	}

	if (min_len != std::numeric_limits<int>::max()) {
		mem[{pos[0].first, pos[0].second,
		     pos[1].first, pos[1].second,
		     pos[2].first, pos[2].second,
		     pos[3].first, pos[3].second, keys}] = min_len - length;
	}

	return min_len;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cerr << "no program file\n";
		exit(1);
	}

	auto map = read_map(argv[1]);

	std::vector<std::pair<int, int>> pos;
	std::uint32_t all_keys = 0;

	for (int y = 0; y < map.size(); ++y) {
		for (int x = 0; x < map[y].size(); ++x) {
			if (map[y][x] == '@') {
				pos.push_back({x, y});
				map[y][x] = '.';
			}

			if (std::islower(map[y][x])) {
				add_key(all_keys, map[y][x]);
			}
		}
	}

	std::uint32_t keys = 0;
	Memory mem;

	std::cout << find_shortest_key_path(mem, map, keys, all_keys, pos, 0) << '\n';

	return 0;
}
