//
// Advent of Code 2019, day 6, part two
//

#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

using AdjList = std::unordered_map<std::string, std::vector<std::string>>;

int main()
{
	AdjList adj;

	std::string from, to;

	while (std::getline(std::cin, from, ')') && std::getline(std::cin, to)) {
		adj[from].push_back(to);
		adj[to].push_back(from);
	}

	std::unordered_map<std::string, int> distance;
	std::queue<std::string> queue;

	queue.push("YOU");
	distance["YOU"] = 0;

	// BFS computing min distance from YOU
	while (!queue.empty()) {
		std::string n = queue.front();
		queue.pop();

		for (const auto &obj : adj[n]) {
			if (distance[obj] == 0) {
				distance[obj] = distance[n] + 1;
				if (obj == "SAN") {
					// minus 2 because we need the distance from
					// the object we are orbiting to the object
					// santa is orbiting
					std::cout << distance["SAN"] - 2 << '\n';
					exit(0);
				}
				queue.push(obj);
			}
		}
	}

	return 1;
}
