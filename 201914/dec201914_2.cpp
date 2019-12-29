//
// Advent of Code 2019, day 14, part two
//

#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct IngList {
	std::vector<std::pair<std::string, long long>> ingredients;
	long long num_produced = 0;
};

using ReactionList = std::unordered_map<std::string, IngList>;

ReactionList read_reactions()
{
	ReactionList map;

	std::string line;

	while (std::getline(std::cin, line)) {
		IngList inglist;

		auto p = line.find("=>");
		std::istringstream ingredients(line.substr(0, p));
		std::istringstream result(line.substr(p + 2, std::string::npos));

		int res_num = 0;
		std::string res_name;

		result >> res_num >> res_name;

		inglist.num_produced = res_num;

		std::string ing;

		while (std::getline(ingredients, ing, ',')) {
			std::istringstream ingredient(ing);

			int num = 0;
			std::string name;

			ingredient >> num >> name;

			inglist.ingredients.push_back({name, num});
		}

		map[res_name] = std::move(inglist);
	}

	return map;
}

long long get_ore_used(const ReactionList &map, long long fuel)
{
	std::unordered_map<std::string, long long> have;

	std::queue<std::pair<std::string, long long>> need;

	need.push({"FUEL", fuel});

	long long ore_used = 0;

	while (!need.empty()) {
		auto [name, num] = need.front();
		need.pop();

		if (name == "ORE") {
			ore_used += num;
			continue;
		}

		if (have[name]) {
			if (have[name] >= num) {
				have[name] -= num;
				continue;
			}

			num -= have[name];
			have[name] = 0;
		}

		auto &recipe = map.at(name);

		long long num_needed = (num + (recipe.num_produced - 1)) / recipe.num_produced;

		for (const auto &ing : recipe.ingredients) {
			need.push({ing.first, num_needed * ing.second});
		}

		have[name] += num_needed * recipe.num_produced - num;
	}

	return ore_used;
}

int main()
{
	ReactionList map = read_reactions();

	// Use binary search to estimate fuel produced
	long long l = 1000000000000LL / 720484LL;
	long long r = l * 2;

	while (l < r) {
		long long m = l + (r - l) / 2;
		long long m_ore = get_ore_used(map, m);

		if (m_ore < 1000000000000LL) {
			l = m + 1;
		}
		else if (m_ore > 1000000000000LL) {
			r = m;
		}
		else {
			l = m;
			break;
		}
	}

	// Show a few values around the result
	for (long long i = l - 3; i < l + 3; ++i) {
		std::cout << i << " fuel => " << get_ore_used(map, i) << " ore\n";
	}

	return 0;
}
