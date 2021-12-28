#include <fstream>
#include <vector>

#define NUM_GRID_ROWS	(10)
#define NUM_GRID_COLS	(10)

static const int NEIGHBOR_INDEXES[8][2] = {
	{-1, -1}, {-1, 0}, {-1, 1},
	{ 0, -1},          { 0, 1},
	{ 1, -1}, { 1, 0}, { 1, 1}
};

struct Position {
	int row;
	int col;
};

struct Octopus {
	int energy_level;
	bool flashed;
	Position pos;
};

class OctopusGrid
{
public:
	void display(void);
	void step(void);
	void add_octopus(Octopus octopus);
	int get_num_flashes(void) { return m_num_flashes; }
	bool did_all_flash_together(void) { return m_all_flashed; }
private:
	int m_num_flashes = 0;
	bool m_all_flashed = false;
	Octopus m_grid[NUM_GRID_ROWS][NUM_GRID_COLS];
	bool is_valid_pos(int row, int col);
	void flash_octopuses(void);
};

void OctopusGrid::display(void)
{
	for (int row = 0; row < NUM_GRID_ROWS; row++) {
		for (int col = 0; col < NUM_GRID_COLS; col++) {
			Octopus octopus = m_grid[row][col];
			printf("%d%s ", octopus.energy_level, octopus.flashed ? "*" : " ");
		}
		printf("\n");
	}
	printf("\n");
}

void OctopusGrid::add_octopus(Octopus octopus)
{
	m_grid[octopus.pos.row][octopus.pos.col] = octopus;
}

bool OctopusGrid::is_valid_pos(int row, int col)
{
	return (row >= 0 && row < NUM_GRID_ROWS && col >= 0 && col < NUM_GRID_COLS);
}

void OctopusGrid::flash_octopuses(void)
{
	int cur_num_flashes = m_num_flashes;
	bool flashing = true;
	while (flashing) {
		// Go through all octopuses and see which ones flashed
		bool flashed = false;
		for (int row = 0; row < NUM_GRID_ROWS; row++) {
			for (int col = 0; col < NUM_GRID_COLS; col++) {
				Octopus& octopus = m_grid[row][col];
				if (octopus.energy_level > 9 && !octopus.flashed) {
					// Flash octopus
					octopus.flashed = true;
					m_num_flashes++;
					flashed = true;

					// Increase energy level of neighbors
					for (auto n: NEIGHBOR_INDEXES) {
						int n_row = row + n[0];
						int n_col = col + n[1];
						if (is_valid_pos(n_row, n_col)) {
							m_grid[n_row][n_col].energy_level++;
						}
					}
				}
			}
		}

		// Continue until no more flash
		if (!flashed) {
			flashing = false;
		}
	}

	if (m_num_flashes - cur_num_flashes == NUM_GRID_ROWS * NUM_GRID_COLS) {
		m_all_flashed = true;
	}
}

void OctopusGrid::step(void)
{
	// First increase energy level of all octopuses by 1 and clear
	// flash status from previous steps
	for (int row = 0; row < NUM_GRID_ROWS; row++) {
		for (int col = 0; col < NUM_GRID_COLS; col++) {
			Octopus& octopus = m_grid[row][col];
			octopus.flashed = false;
			octopus.energy_level++;
		}
	}

	// Handle octopus flashes
	flash_octopuses();

	// Reset flashed octopuses back to 0
	for (int row = 0; row < NUM_GRID_ROWS; row++) {
		for (int col = 0; col < NUM_GRID_COLS; col++) {
			Octopus& octopus = m_grid[row][col];
			if (octopus.flashed) {
				octopus.energy_level = 0;
			}
		}
	}
}

int main()
{
	OctopusGrid grid;

	int row, col = 0;
	std::ifstream input("../day11/input.txt");
	for (std::string line; std::getline(input, line); ) {
		col = 0;
		for (char c: line) {
			Octopus octopus;
			octopus.energy_level = c - '0';
			octopus.pos.row = row;
			octopus.pos.col = col;
			grid.add_octopus(octopus);
			col++;
		}
		row++;
	}

	int step = 0;
	while (!grid.did_all_flash_together()) {
		grid.step();
		if (++step == 100) {
			printf("Part 1: %d\n", grid.get_num_flashes());
		}
	}

	printf("Part 2: %d\n", step);
	return 0;
}
