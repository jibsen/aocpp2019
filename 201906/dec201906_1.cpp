//
// Advent of Code 2019, day 6, part one
//

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using AdjList = std::unordered_map<std::string, std::vector<std::string>>;

int sum_orbits(const AdjList &adj, const std::string &src, int dist)
{
	int sum = 0;

	if (auto it = adj.find(src); it != adj.end()) {
		for (const auto &obj : (*it).second) {
			sum += sum_orbits(adj, obj, dist + 1);
		}
	}

	return sum + dist;
}

int main()
{
	AdjList adj;

	std::string from, to;

	while (std::getline(std::cin, from, ')') && std::getline(std::cin, to)) {
		adj[from].push_back(to);
	}

	std::cout << sum_orbits(adj, "COM", 0) << '\n';

	return 0;
}
