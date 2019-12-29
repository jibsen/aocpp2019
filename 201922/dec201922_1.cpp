//
// Advent of Code 2019, day 22, part one
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

enum class Shuffle {
	cut, increment, stack
};

std::vector<std::pair<Shuffle, int>> read_instructions(const char *filename)
{
	std::ifstream infile(filename);
	std::string line;

	std::vector<std::pair<Shuffle, int>> instructions;

	while (std::getline(infile, line)) {
		if (line.size() > 4 && line.substr(0, 3) == "cut") {
			instructions.push_back({Shuffle::cut, std::stoi(line.substr(4))});
		}
		else if (line.size() > 20 && line.substr(5, 4) == "with") {
			instructions.push_back({Shuffle::increment, std::stoi(line.substr(20))});
		}
		else if (line.size() > 9 && line.substr(5, 4) == "into") {
			instructions.push_back({Shuffle::stack, 0});
		}
		else {
			std::cout << "unable to parse " << line << '\n';
		}
	}

	return instructions;
}

void cut(std::vector<int> &cards, int n) {
	if (n > 0) {
		std::rotate(cards.begin(), cards.begin() + n, cards.end());
	}
	else if (n < 0) {
		std::rotate(cards.begin(), cards.end() - std::abs(n), cards.end());
	}
}

void deal_with_increment(std::vector<int> &cards, int n) {
	std::vector<int> table(cards.size());
	int i = 0;
	for (auto c : cards) {
		table[i] = c;
		i = (i + n) % cards.size();
	}
	cards.swap(table);
}

void deal_into_new_stack(std::vector<int> &cards) {
	std::reverse(cards.begin(), cards.end());
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cerr << "no program file\n";
		exit(1);
	}

	auto instructions = read_instructions(argv[1]);

	std::cout << instructions.size() << '\n';

	std::vector<int> cards(10007);

	std::iota(cards.begin(), cards.end(), 0);

	for (auto [type, arg] : instructions) {
		switch (type) {
		case Shuffle::cut:
			cut(cards, arg);
			break;
		case Shuffle::increment:
			deal_with_increment(cards, arg);
			break;
		case Shuffle::stack:
			deal_into_new_stack(cards);
			break;
		default:
			std::cerr << "unknown shuffle method\n";
			exit(1);
		}
	}

	for (int i = 0; i < cards.size(); ++i) {
		if (cards[i] == 2019) {
			std::cout << i << '\n';
			break;
		}
	}

	return 0;
}
