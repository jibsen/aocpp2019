//
// Advent of Code 2019, day 16, part two
//

// We note that for the last half of the digits, the FFT for the kth place
// is just the sum of the last n - (k - 1) digits, because the first k - 1
// entries in the pattern will be 0, and the next k will be 1. So the last
// digit is just the last digit, the second-to-last digit is the sum of the
// two last digits, and so on.

#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

void update_tail(std::vector<int> &s)
{
	int sum = std::accumulate(s.begin(), s.end(), 0);

	for (auto &digit : s) {
		int next_sum = sum - digit;
		digit = sum % 10;
		sum = next_sum;
	}
}

int main()
{
	std::vector<int> s;

	std::transform(std::istream_iterator<char>(std::cin),
	               std::istream_iterator<char>(),
		       std::back_inserter(s),
		       [](char ch) { return ch - '0'; });

	std::cout << s.size() << '\n';

	// Get offset in first 7 digits
	int offset = 0;

	for (int i = 0; i < 7; ++i) {
		offset = offset * 10 + s[i];
	}

	if (offset < s.size() / 2 || offset >= 10000 * s.size() - 8) {
		std::cerr << "message not within second half\n";
		return 1;
	}

	// Generate tail of s duplicated 10000 times, starting at offset
	std::vector<int> stail;

	stail.reserve(10000 * s.size() - offset);

	if (offset % s.size()) {
		stail.insert(stail.end(), s.begin() + offset % s.size(), s.end());
	}

	for (int i = 0; i < (10000 * s.size() - offset) / s.size(); ++i) {
		stail.insert(stail.end(), s.begin(), s.end());
	}

	for (int phase = 0; phase < 100; ++phase) {
		update_tail(stail);
	}

	for (int i = 0; i < 8; ++i) {
		std::cout << stail[i];
	}
	std::cout << '\n';

	return 0;
}
