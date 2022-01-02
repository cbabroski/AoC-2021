#include <cstdio>
#include <vector>
#include <queue>
#include <string>

class TransparentPaper
{
public:
	// Parse an input file containing dot positions and fold instructions
	void parse_input(std::string filename);

	// Perform and consume a fold instruction. Returns how many remaining
	// folds can still be done based on fold instructions. Returns -1 on error.
	int fold(void);

	// Returns how many dots are visible on the paper/grid
	int get_num_visible_dots(void);

	// Prints out the current paper/grid with dots
	void display(void);
private:
	int m_max_x_value, m_max_y_value = 0;
	bool m_grid_constructed = false;
	std::vector<std::vector<int>> m_grid;
	std::vector<std::pair<int, int>> m_dots;
	std::queue<std::pair<int, int>> m_fold_instructions;

	// Add dot based on position
	void add_dot(int x, int y);

	// Add fold instruction based on position. (0, y) folds left vertical axis,
	// (x, 0) folds up along horizontal axis.
	void add_fold_instruction(int x, int y);

	// Helpers to place dots on the grid and resize if necessary
	void construct_grid(void);
	void resize_grid(void);
};

void TransparentPaper::add_dot(int x, int y)
{
	if (x > m_max_x_value) {
		m_max_x_value = x;
	}

	if (y > m_max_y_value) {
		m_max_y_value = y;
	}

	m_dots.push_back(std::make_pair(x, y));
}

void TransparentPaper::add_fold_instruction(int x, int y)
{
	m_fold_instructions.push(std::make_pair(x, y));
}

void TransparentPaper::construct_grid(void)
{
	// num rows = max y + 1
	// num cols = max x + 1
	m_grid.resize(m_max_y_value + 1, std::vector<int>(m_max_x_value + 1));

	int x, y = 0;
	for (auto p: m_dots) {
		x = p.first;
		y = p.second;
		m_grid[y][x] = 1;
	}

	m_grid_constructed = true;
}

void TransparentPaper::display(void)
{
	if (!m_grid_constructed) {
		construct_grid();
	}

	for (int row = 0; row < m_grid.size(); row++) {
		for (int col = 0; col < m_grid[0].size(); col++) {
			if (m_grid[row][col] == 1) {
				printf("#");
			} else {
				printf(".");
			}
		}
		printf("\n");
	}
	printf("\n");
}

int TransparentPaper::get_num_visible_dots(void)
{
	int num_visible_dots = 0;
	for (int row = 0; row < m_grid.size(); row++) {
		for (int col = 0; col < m_grid[0].size(); col++) {
			if (m_grid[row][col] == 1) {
				num_visible_dots++;
			}
		}
	}
	return num_visible_dots;
}

void TransparentPaper::resize_grid(void)
{
	if (!m_grid_constructed) {
		construct_grid();
	}

	int max_x, max_y = 0;
	for (int row_y = 0; row_y < m_grid.size(); row_y++) {
		for (int col_x = 0; col_x < m_grid[0].size(); col_x++) {
			if (m_grid[row_y][col_x] == 1) {
				if (row_y > max_y) {
					max_y = row_y;
				}
				if (col_x > max_x) {
					max_x = col_x;
				}
			}
		}
	}

	m_max_y_value = max_y;
	m_max_x_value = max_x;
	m_grid.resize(m_max_y_value + 1, std::vector<int>(m_max_x_value + 1));
}

int TransparentPaper::fold(void)
{
	if (!m_grid_constructed) {
		construct_grid();
	}

	std::pair<int, int> p = m_fold_instructions.front();
	int fold_x = p.first;
	int fold_y = p.second;
	for (int row_y = 0; row_y < m_grid.size(); row_y++) {
		for (int col_x = 0; col_x < m_grid[0].size(); col_x++) {
			// Fold left
			if (fold_x != 0 && col_x > fold_x) {
				if (m_grid[row_y][col_x] == 1) {
					// Clear current value
					m_grid[row_y][col_x] = 0;

					// Set new value
					m_grid[row_y][fold_x - (col_x - fold_x)] = 1;
				}
			}

			// Fold up
			if (fold_y != 0 && row_y > fold_y) {
				if (m_grid[row_y][col_x] == 1) {
					// Clear current value
					m_grid[row_y][col_x] = 0;

					// Set new value
					m_grid[fold_y - (row_y - fold_y)][col_x] = 1;
				}
			}
		}
	}

	resize_grid();
	m_fold_instructions.pop();
	return m_fold_instructions.size();
}

void TransparentPaper::parse_input(std::string filename)
{
	FILE *fp = fopen(filename.c_str(), "r");
	if (!fp) {
		printf("Failed to open %s.\n", filename.c_str());
		return;
	}

	int x, y;
	while (fscanf(fp, "%d,%d\n", &x, &y) > 0) {
		add_dot(x, y);
	}

	char axis;
	int pos;
	while (fscanf(fp, "fold along %c=%d\n", &axis, &pos) > 0) {
		if (axis == 'x') {
			x = pos;
			y = 0;
		} else if (axis == 'y') {
			x = 0;
			y = pos;
		} else {
			printf("no axis '%c'\n", axis);
			continue;
		}

		add_fold_instruction(x, y);
	}

	fclose(fp);
}

int main()
{
	TransparentPaper paper;
	paper.parse_input("../day13/sample-input.txt");

	std::string code;
	int num_dots_after_first_fold = 0;
	int num_remaining_folds = 0;
	do {
		num_remaining_folds = paper.fold();
		paper.display();
		if (!num_dots_after_first_fold) {
			num_dots_after_first_fold = paper.get_num_visible_dots();
		}
	} while (num_remaining_folds > 0);

	printf("part 1: %d\n", num_dots_after_first_fold);
	printf("Part 2: %s\n", code.c_str());
	return 0;
}
