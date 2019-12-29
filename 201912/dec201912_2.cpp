//
// Advent of Code 2019, day 12, part two
//

// We note that the update function is invertible, which means reaching the
// initial state again will be the first repeat. The x coordinates of the
// four moons form an independent system, as do the y, and z coordinates.
//
// We can thus compute the minimal period of each coordinate and find the
// least common multiple of these to get the first step at which all three
// systems will be in the initial state again at the same time.

#include <array>
#include <iostream>

namespace {

int gravity(int c, int c_other)
{
	if (c < c_other) {
		return 1;
	}

	if (c > c_other) {
		return -1;
	}

	return 0;
}

void update_velocities(const std::array<int, 4> &pos, std::array<int, 4> &vel)
{
	for (int i = 0; i < pos.size() - 1; ++i) {
		for (int j = i + 1; j < pos.size(); ++j) {
			int grav = gravity(pos[i], pos[j]);

			vel[i] += grav;
			vel[j] -= grav;
		}
	}
}

void apply_velocities(std::array<int, 4> &pos, const std::array<int, 4> &vel)
{
	for (int i = 0; i < pos.size(); ++i) {
		pos[i] += vel[i];
	}
}

unsigned long long find_period(const std::array<int, 4> &initial_pos, const std::array<int, 4> &initial_vel)
{
	std::array<int, 4> pos = initial_pos;
	std::array<int, 4> vel = initial_vel;

	unsigned long long step = 0;

	do {
		update_velocities(pos, vel);
		apply_velocities(pos, vel);
		++step;
	} while (pos != initial_pos || vel != initial_vel);

	return step;
}

} // namespace

int main()
{
	auto x_period = find_period({17, 2, -1, 4}, {0,0,0,0});
	auto y_period = find_period({-9, 2, 5, 7}, {0,0,0,0});
	auto z_period = find_period({4, -13, -1, -7}, {0,0,0,0});

	// Compute LCM of the periods of the x, y, and z coordinates
	unsigned long long xm = x_period;
	unsigned long long ym = y_period;
	unsigned long long zm = z_period;

	while (xm != ym || ym != zm) {
		if (xm <= ym && xm <= zm) {
			xm += x_period;
		}
		else if (ym <= xm && ym <= zm) {
			ym += y_period;
		}
		else if (zm <= xm && zm <= ym) {
			zm += z_period;
		}
	}

	std::cout << xm << '\n';

	return 0;
}
