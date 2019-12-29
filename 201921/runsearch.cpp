//
// Find solution for AoC day 21 part two using brute-force search
//

// Avoiding the overhead of the INTCODE interpreter, this brute-force search
// found a 6 instruction solution in roughly 40 minutes.

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

struct Opcode {
	int op = 0;
	int src = 0;
	int dst = 0;
	bool operator==(const Opcode &rhs) { return op == rhs.op && src == rhs.src && dst == rhs.dst; }
};

struct State {
	// First 10 elements serve as a circular buffer of the hull, elements
	// at index 10 and 11 are T and J
	std::array<unsigned int, 12> registers;
	int reg_offs = 0;

	void step(unsigned int next)
	{
		registers[reg_offs++] = next;

		if (reg_offs == 10) {
			reg_offs = 0;
		}
	}

	unsigned int reg(int i) const
	{
		return i < 10 ? registers[(reg_offs + i) % 10] : registers[i];
	}
};

bool run(const std::vector<Opcode> &program, State &state, const std::vector<unsigned int> &hull)
{
	auto it = hull.begin();

	for (int i = 0; i < 10; ++i) {
		state.step(*it++);
	}

	for (;;) {
		if (it == hull.end()) {
			return true;
		}

		state.registers[10] = 0;
		state.registers[11] = 0;

		for (const auto &op : program) {
			switch (op.op) {
			case 0:
				state.registers[op.dst] = !state.reg(op.src);
				break;
			case 1:
				state.registers[op.dst] &= state.reg(op.src);
				break;
			case 2:
				state.registers[op.dst] |= state.reg(op.src);
				break;
			}
		}

		if (state.registers[11]) {
			if (!state.reg(4)) {
				return false;
			}

			if (std::distance(it, hull.end()) < 4) {
				return true;
			}

			state.step(*it++);
			state.step(*it++);
			state.step(*it++);
			state.step(*it++);
		}
		else {
			if (!state.reg(1)) {
				return false;
			}

			state.step(*it++);
		}
	}
}

bool increment(std::vector<int> &v, int max)
{
	for (int i = 0; i < v.size(); ++i) {
		if (v[i] < max) {
			++v[i];
			if (i == v.size() - 1) {
				std::cout << (v[i] * 100) / max << "%\n";
			}
			return true;
		}
		v[i] = 0;
	}
	return false;
}

bool contains_double(const std::vector<int> &v)
{
	for (int i = 1; i < v.size(); ++i) {
		if (v[i - 1] == v[i]) {
			return true;
		}
	}

	return false;
}

int main(int argc, char *argv[])
{
	std::array<std::string, 3> op_names = { "NOT ", "AND ", "OR " };
	std::array<std::string, 12> src_names = { "! ", "A ", "B ", "C ", "D ", "E ", "F ", "G ", "H ", "I ", "T ", "J " };
	std::array<std::string, 12> dst_names = { "!", "!", "!", "!", "!", "!", "!", "!", "!", "!", "T", "J" };
	std::vector<unsigned int> hull = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1,
		1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1,
		1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	};

	std::vector<Opcode> possible;

	// Generate all possible instructions
	for (int op = 0; op < 3; ++op) {
		for (int src = 1; src < 12; ++src) {
			for (int dst = 10; dst < 12; ++dst) {
				possible.push_back({op, src, dst});
			}
		}
	}

	// Remove AND T T, AND J J, OR T T, and OR J J
	possible.erase(std::find(possible.begin(), possible.end(), Opcode{1, 10, 10}));
	possible.erase(std::find(possible.begin(), possible.end(), Opcode{1, 11, 11}));
	possible.erase(std::find(possible.begin(), possible.end(), Opcode{2, 10, 10}));
	possible.erase(std::find(possible.begin(), possible.end(), Opcode{2, 11, 11}));

	// Skip all NOT instructions up to NOT T J as last instruction (all
	// the ones before will overwrite J with NOT A-I)
	int first_last = std::distance(possible.begin(), std::find(possible.begin(), possible.end(), Opcode{0, 10, 11}));

	std::cout << possible.size() << " possible instructions\n";

	std::vector<int> picks = { 0, 0, 0, 0, first_last };

	for (int len = 5; len < 15; ++len) {
		std::cout << len << '\n';
		do {
			// If last instruction does not write to J, or there
			// are two of the same instructions in a row, skip
			if (possible[picks.back()].dst != 11 || contains_double(picks)) {
				continue;
			}

			std::vector<Opcode> program;

			for (int i : picks) {
				program.push_back(possible[i]);
			}

			State state;

			if (run(program, state, hull)) {
				for (int i : picks) {
					std::cout << i << ',';
				}
				std::cout << '\n';

				for (const auto &op : program) {
					std::cout << op_names[op.op] << src_names[op.src] << dst_names[op.dst] << '\n';
				}

				for (const auto &op : program) {
					std::cout << op.op << ',' << op.src << ',' << op.dst << '\n';
				}

				exit(0);
			}
		} while (increment(picks, possible.size() - 1));

		picks.push_back(first_last);
	}

	return 0;
}
