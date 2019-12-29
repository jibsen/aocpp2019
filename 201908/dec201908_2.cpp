//
// Advent of Code 2019, day 8, part two
//

#include <array>
#include <iostream>

using Layer = std::array<std::array<int, 25>, 6>;

int main()
{
	Layer img;

	// Set all pixels to transparent
	for (auto &row : img) {
		row.fill(2);
	}

	// Read layers, overwriting transparent pixels
	while (std::cin) {
		for (auto &row : img) {
			for (auto &pixel : row) {
				char ch;

				std::cin >> ch;

				if (pixel == 2) {
					pixel = ch - '0';
				}
			}
		}
	}

	// Print white pixels
	for (const auto &row : img) {
		for (auto pixel : row) {
			std::cout << (pixel ? 'X' : ' ');
		}
		std::cout << '\n';
	}

	return 0;
}
