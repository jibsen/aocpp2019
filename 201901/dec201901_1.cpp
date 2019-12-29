//
// Advent of Code 2019, day 1, part one
//

#include <iostream>
#include <iterator>
#include <numeric>

int main()
{
	std::istream_iterator<long long> it(std::cin);
	std::istream_iterator<long long> end;

	std::cout << std::accumulate(it, end, 0LL,
		[](long long acc, const auto &mass) {
			return acc + (mass / 3) - 2;
		});

	return 0;
}
