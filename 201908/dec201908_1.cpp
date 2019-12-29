//
// Advent of Code 2019, day 8, part one
//

#include <iostream>

int main()
{
	int min_zero = std::numeric_limits<int>::max();
	int checksum = 0;

	while (std::cin) {
		int num_zero = 0;
		int num_one = 0;
		int num_two = 0;

		for (int i = 0; i < 6 * 25; ++i) {
			char ch;

			std::cin >> ch;

			num_zero += ch == '0';
			num_one += ch == '1';
			num_two += ch == '2';
		}

		if (num_zero < min_zero) {
			min_zero = num_zero;
			checksum = num_one * num_two;
		}
	}

	std::cout << checksum << '\n';

	return 0;
}
