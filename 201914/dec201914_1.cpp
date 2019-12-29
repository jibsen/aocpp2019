//
// Advent of Code 2019, day 14, part one
//

#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct IngList {
	std::vector<std::pair<std::string, int>> ingredients;
	int num_produced = 0;
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

int main()
{
	ReactionList map = read_reactions();

	std::unordered_map<std::string, int> have;

	std::queue<std::pair<std::string, int>> need;

	need.push({"FUEL", 1});

	int ore_used = 0;

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

		const auto &recipe = map[name];

		int num_needed = (num + (recipe.num_produced - 1)) / recipe.num_produced;

		for (const auto &ing : recipe.ingredients) {
			need.push({ing.first, num_needed * ing.second});
		}

		have[name] += num_needed * recipe.num_produced - num;
	}

	std::cout << ore_used << '\n';

	return 0;
}
