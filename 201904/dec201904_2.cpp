//
// Advent of Code 2019, day 4, part two
//

#include <array>
#include <iostream>

using Number = std::array<int, 6>;

void increment_nondecreasing(Number &number)
{
	int digit = 0;

	for (; digit < 6; ++digit) {
		if (number[digit] < 9) {
			++number[digit];
			break;
		}
	}

	for (; digit > 0; --digit) {
		number[digit - 1] = number[digit];
	}
}

bool contains_single_repeat(const Number &number)
{
	int digit = -1;
	int count = 0;

	for (int i = 0; i < 6; ++i) {
		if (number[i] == digit) {
			++count;
		}
		else {
			if (count == 2) {
				return true;
			}
			digit = number[i];
			count = 1;
		}
	}

	return count == 2;
}

void print_number(const Number &number)
{
	for (auto d = number.rbegin(); d != number.rend(); ++d) {
		std::cout << *d;
	}
	std::cout << '\n';
}

int main()
{
	// 158899 is the lowest valid password above 158126
	Number number = { 9, 9, 8, 8, 5, 1 };
	size_t num_passwords = 0;

	// 588999 is the highest valid password below 624574
	while (number[5] < 6) {
		if (contains_single_repeat(number)) {
			++num_passwords;
		}
		increment_nondecreasing(number);
	}

	std::cout << num_passwords << '\n';

	return 0;
}
