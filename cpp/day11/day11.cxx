#include <fstream>
#include <vector>

#define NUM_GRID_ROWS	(10)
#define NUM_GRID_COLS	(10)

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
	int get_neighbors(Octopus octopus, std::vector<Octopus>& neighbors);
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

// FIXME/Note: for now get_neighbors returns copies of neighboring octopuses, so can't use them
// directly as references to m_grid and instead need to use their positions to update the real
// ones.
int OctopusGrid::get_neighbors(Octopus octopus, std::vector<Octopus>& neighbors)
{
	int row = 0;
	int col = 0;

	if (octopus.pos.row < 0 || octopus.pos.col < 0 ||
	    octopus.pos.row >= NUM_GRID_ROWS || octopus.pos.col >= NUM_GRID_COLS) {
		printf("%s: invalid input params.\n", __func__);
		return -1;
	}

	// Up
	row = octopus.pos.row - 1;
	col = octopus.pos.col;
	if (row >= 0 && col >= 0) {
		neighbors.push_back(m_grid[row][col]);
	}

	// Left
	row = octopus.pos.row;
	col = octopus.pos.col - 1;
	if (row >= 0 && col >= 0) {
		neighbors.push_back(m_grid[row][col]);
	}

	// Right
	row = octopus.pos.row;
	col = octopus.pos.col + 1;
	if (row < NUM_GRID_ROWS && col < NUM_GRID_COLS) {
		neighbors.push_back(m_grid[row][col]);
	}

	// Down
	row = octopus.pos.row + 1;
	col = octopus.pos.col;
	if (row < NUM_GRID_ROWS && col < NUM_GRID_COLS) {
		neighbors.push_back(m_grid[row][col]);
	}

	// Diagonal left up
	row = octopus.pos.row - 1;
	col = octopus.pos.col - 1;
	if (row >= 0 && col >= 0) {
		neighbors.push_back(m_grid[row][col]);
	}

	// Diagonal left down
	row = octopus.pos.row + 1;
	col = octopus.pos.col - 1;
	if (row < NUM_GRID_ROWS && col >= 0) {
		neighbors.push_back(m_grid[row][col]);
	}

	// Diagonal right up
	row = octopus.pos.row - 1;
	col = octopus.pos.col + 1;
	if (row >= 0 && col < NUM_GRID_COLS) {
		neighbors.push_back(m_grid[row][col]);
	}

	// Diagonal right down
	row = octopus.pos.row + 1;
	col = octopus.pos.col + 1;
	if (row < NUM_GRID_ROWS && col < NUM_GRID_COLS) {
		neighbors.push_back(m_grid[row][col]);
	}

	return 0;
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
					std::vector<Octopus> neighbors;
					get_neighbors(octopus, neighbors);
					for (Octopus n: neighbors) {
						m_grid[n.pos.row][n.pos.col].energy_level++;
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
