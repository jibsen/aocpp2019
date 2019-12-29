//
// Advent of Code 2019, day 12, part one
//

#include <array>
#include <cmath>
#include <iostream>
#include <numeric>

struct Moon {
	int x = 0;
	int y = 0;
	int z = 0;
	int vx = 0;
	int vy = 0;
	int vz = 0;
	Moon(int x_start, int y_start, int z_start)
		: x(x_start), y(y_start), z(z_start) {}
	Moon() = default;
};

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

void update_velocities(std::array<Moon, 4> &obj)
{
	for (int i = 0; i < obj.size() - 1; ++i) {
		for (int j = i + 1; j < obj.size(); ++j) {
			obj[i].vx += gravity(obj[i].x, obj[j].x);
			obj[i].vy += gravity(obj[i].y, obj[j].y);
			obj[i].vz += gravity(obj[i].z, obj[j].z);

			obj[j].vx += gravity(obj[j].x, obj[i].x);
			obj[j].vy += gravity(obj[j].y, obj[i].y);
			obj[j].vz += gravity(obj[j].z, obj[i].z);
		}
	}
}

void apply_velocities(std::array<Moon, 4> &obj)
{
	for (auto &m : obj) {
		m.x += m.vx;
		m.y += m.vy;
		m.z += m.vz;
	}
}

int total_energy(const std::array<Moon, 4> &obj)
{
	return std::accumulate(obj.begin(), obj.end(), 0, [](int energy, const auto &m) {
		int e_pot = std::abs(m.x) + std::abs(m.y) + std::abs(m.z);
		int e_kin = std::abs(m.vx) + std::abs(m.vy) + std::abs(m.vz);
		return energy + e_pot * e_kin;
	});
}

void print_system(const std::array<Moon, 4> &obj, int step)
{
	std::cerr << "After " << step << " steps:\n";

	for (const auto &m : obj) {
		std::cerr << "pos=<" << m.x << ',' << m.y << ',' << m.z
			<< "> vel=<" << m.vx << ',' << m.vy << ',' << m.vz << ">\n";
	}
	std::cerr << "Total energy: " << total_energy(obj) << "\n\n";
}

int main()
{
	std::array<Moon, 4> obj = {
		Moon(17, -9, 4),
		Moon(2, 2, -13),
		Moon(-1, 5, -1),
		Moon(4, 7, -7)
	};

	for (int time = 0; time <= 1000; ++time) {
		if (time % 100 == 0) { print_system(obj, time); }
		update_velocities(obj);
		apply_velocities(obj);
	}

	return 0;
}
