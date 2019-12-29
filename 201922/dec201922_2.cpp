//
// Advent of Code 2019, day 22, part two
//

// First we note that we do not need to perform the operations on an entire
// deck to be able to track the position of one card through the shuffle.
//
// In the modular arithmetic of the deck, the stack shuffle is equivalent to
// negating the position, the cut shuffle is adding or subtracting a number,
// and the increment shuffle is multiplying by a number.
//
// So applying the shuffle process amounts to adding an offset and multiplying
// by a number.

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

enum class Shuffle {
	cut, increment, stack
};

constexpr uint64_t num_cards = 119315717514047ULL;
constexpr uint64_t num_rounds = 101741582076661ULL;

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

// From https://en.wikipedia.org/wiki/Modular_arithmetic
constexpr uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t m)
{
	uint64_t d = 0, mp2 = m >> 1;
	if (a >= m) { a %= m; }
	if (b >= m) { b %= m; }
	for (int i = 0; i < 64; ++i) {
		d = (d > mp2) ? (d << 1) - m : d << 1;
		if (a & 0x8000000000000000ULL) { d += b; }
		if (d >= m) { d -= m; }
		a <<= 1;
	}
	return d;
}

// From https://en.wikipedia.org/wiki/Modular_arithmetic
constexpr uint64_t pow_mod(uint64_t a, uint64_t b, uint64_t m)
{
	uint64_t r = m == 1 ? 0 : 1;
	while (b > 0) {
		if (b & 1) { r = mul_mod(r, a, m); }
		b = b >> 1;
		a = mul_mod(a, a, m);
	}
	return r;
}

uint64_t cut(uint64_t pos, int n) {
	if (n > 0) {
		if (pos >= n) {
			return pos - n;
		}

		return pos + (num_cards - n);
	}

	if (n < 0) {
		n = std::abs(n);

		if (pos >= num_cards - n) {
			return pos - (num_cards - n);
		}

		return pos + n;
	}

	return pos;
}

uint64_t uncut(uint64_t pos, int n) {
	return cut(pos, -n);
}

uint64_t deal_with_increment(uint64_t pos, int n) {
	if (pos == 0) {
		return 0;
	}

	if (std::numeric_limits<uint64_t>::max() / pos > n) {
		return (pos * n) % num_cards;
	}

	return mul_mod(pos, n, num_cards);
}

uint64_t undeal_with_increment(uint64_t pos, int n) {
	if (pos == 0) {
		return 0;
	}

	// Since num_cards is prime, the modular inverse of n is n**(num_cards - 2)
	uint64_t inv = pow_mod(static_cast<uint64_t>(n), num_cards - 2, num_cards);

	if (std::numeric_limits<uint64_t>::max() / pos > inv) {
		return (pos * inv) % num_cards;
	}

	return mul_mod(pos, inv, num_cards);
}

uint64_t deal_into_new_stack(uint64_t pos) {
	return num_cards - 1 - pos;
}

uint64_t undeal_into_new_stack(uint64_t pos) {
	return num_cards - 1 - pos;
}

uint64_t apply_shuffle(const std::vector<std::pair<Shuffle, int>> &instructions, uint64_t pos)
{
	for (auto [type, arg] : instructions) {
		switch (type) {
		case Shuffle::cut:
			pos = cut(pos, arg);
			break;
		case Shuffle::increment:
			pos = deal_with_increment(pos, arg);
			break;
		case Shuffle::stack:
			pos = deal_into_new_stack(pos);
			break;
		default:
			std::cerr << "unknown shuffle method\n";
			exit(1);
		}
	}

	return pos;
}

uint64_t apply_reverse_shuffle(const std::vector<std::pair<Shuffle, int>> &instructions, uint64_t pos)
{
	for (auto [type, arg] : instructions) {
		switch (type) {
		case Shuffle::cut:
			pos = uncut(pos, arg);
			break;
		case Shuffle::increment:
			pos = undeal_with_increment(pos, arg);
			break;
		case Shuffle::stack:
			pos = undeal_into_new_stack(pos);
			break;
		default:
			std::cerr << "unknown shuffle method\n";
			exit(1);
		}
	}

	return pos;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cerr << "no program file\n";
		exit(1);
	}

	auto instructions = read_instructions(argv[1]);

	std::cout << "shuffle size " << instructions.size() << "\n\n";

	uint64_t offset = apply_shuffle(instructions, 0);
	uint64_t scale = apply_shuffle(instructions, 1) - offset;

	std::cout << "offset     " << offset << '\n';
	std::cout << "scale      " << scale << "\n\n";

	std::reverse(instructions.begin(), instructions.end());

	uint64_t rev_offset = apply_reverse_shuffle(instructions, 0);
	uint64_t rev_scale = apply_reverse_shuffle(instructions, 1) - rev_offset;

	std::cout << "rev_offset " << rev_offset << '\n';
	std::cout << "rev_scale  " << rev_scale << "\n\n";

	// We can compute the reverse values as well
	std::cout << "inv offset " << num_cards - mul_mod(offset, rev_scale, num_cards) << '\n';
	std::cout << "inv scale  " << pow_mod(scale, num_cards - 2, num_cards) << "\n\n";

	// The reverse shuffle on 0 gives 41029399044649, and the difference
	// between consecutive values in the forward shuffle is 27693777298340.
	//
	// The forward shuffle on 0 gives 77186865627336, and the difference
	// between consecutive values in the reverse shuffle is 11189855674252.
	//
	// So we get the following equations for the shuffles:
	//
	//   forward: (x - 41029399044649) * 27693777298340
	//   reverse: (x - 77186865627336) * 11189855674252
	//
	// Applying the formula (x - a) * b iteratively to find the source of
	// position x yields
	//
	//   x(k) = b**k * x - b**k * a - b**(k - 1) * a - ... - ba
	//        = b**k * x - b * a * (b**(k - 1) + b**(k - 2) + .. + b + 1)
	//        = b**k * x - b * a * ((b**k - 1) / (b - 1))
	//
	const uint64_t a = offset;
	const uint64_t b = rev_scale;

	// b**k
	const uint64_t bpow = pow_mod(b, num_rounds, num_cards);

	// 1 / (b - 1)
	const uint64_t bm1inv = pow_mod(b - 1, num_cards - 2, num_cards);

	// b * a * ((b**k - 1) / (b - 1))
	uint64_t t2 = mul_mod(bpow - 1, bm1inv, num_cards);
	t2 = mul_mod(t2, a, num_cards);
	t2 = mul_mod(t2, b, num_cards);

	// b**k * x
	const uint64_t t1 = mul_mod(bpow, 2020, num_cards);

	if (t2 > t1) {
		std::cout << "source of position 2020 = " << t1 + (num_cards - t2) << '\n';
	}
	else {
		std::cout << "source of position 2020 = " << t1 - t2 << '\n';
	}

	return 0;
}
