//
// Advent of Code 2019, day 1, part two
//

#include <iostream>

int main()
{
	long long mass = 0;
	long long total_fuel = 0;

	while (std::cin >> mass) {
		long long fuel = (mass / 3) - 2;

		if (fuel > 0) {
			total_fuel += fuel;

			long long extra_fuel = (fuel / 3) - 2;

			while (extra_fuel > 0) {
				total_fuel += extra_fuel;
				extra_fuel = (extra_fuel / 3) - 2;
			}
		}
	}

	std::cout << total_fuel;

	return 0;
}
