//
// Advent of Code 2019, day 16, part one
//

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <iterator>
#include <vector>

int compute_digit_n(const std::vector<int> &s, int n)
{
	static const std::array<int, 4> pattern = { 0, 1, 0, -1 };

	int res = 0;

	// The first k - 1 terms in the sum for the kth digit are zero
	for (int i = n; i < s.size(); ++i) {
		res += s[i] * pattern[((i + 1) / (n + 1)) % pattern.size()];
	}

	return std::abs(res) % 10;
}

int main()
{
	std::vector<int> s;

	std::transform(std::istream_iterator<char>(std::cin),
	               std::istream_iterator<char>(),
		       std::back_inserter(s),
		       [](char ch) { return ch - '0'; });

	std::vector<int> next;

	for (int phase = 0; phase < 100; ++phase) {
		for (int i = 0; i < s.size(); ++i) {
			next.push_back(compute_digit_n(s, i));
		}

		s.swap(next);
		next.clear();
	}

	for (auto i : s) {
		std::cout << i;
	}
	std::cout << '\n';

	return 0;
}
