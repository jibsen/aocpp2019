//
// Advent of Code 2019, day 4, part one
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

bool contains_repeat(const Number &number)
{
	for (int digit = 1; digit < 6; ++digit) {
		if (number[digit - 1] == number[digit]) {
			return true;
		}
	}

	return false;
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
	// 158888 is the lowest valid password above 158126
	Number number = { 8, 8, 8, 8, 5, 1 };
	size_t num_passwords = 0;

	// 599999 is the highest valid password below 624574
	while (number[5] < 6) {
		if (contains_repeat(number)) {
			++num_passwords;
		}
		increment_nondecreasing(number);
	}

	std::cout << num_passwords << '\n';

	return 0;
}
