#include <fstream>
#include <vector>

// Fills in neighbor positions (pairs of <row, col>)
static int get_neighbors(int row, int col, const std::vector<std::vector<int>>& grid,
			 std::vector<std::pair<int, int>>& neighbors)
{
	if (row < 0 || col < 0 || row >= grid.size() || col >= grid[0].size()) {
		printf("%s: invalid input params.\n", __func__);
		return -1;
	}

	int up_row = row - 1;
	if (up_row >= 0) {
		neighbors.push_back(std::make_pair(up_row, col));
	}

	int left_col = col - 1;
	if (left_col >= 0) {
		neighbors.push_back(std::make_pair(row, left_col));
	}

	int right_col = col + 1;
	if (right_col < grid[0].size()) {
		neighbors.push_back(std::make_pair(row, right_col));
	}

	int down_row = row + 1;
	if (down_row < grid.size()) {
		neighbors.push_back(std::make_pair(down_row, col));
	}

	return 0;
}

static bool is_low_point(int row, int col, std::vector<std::vector<int>>& grid)
{
	// A point is a low point if it is smaller than all of its neighbors
	std::vector<std::pair<int, int>> neighbors; // pair<row, col>
	get_neighbors(row, col, grid, neighbors);
	for (auto n: neighbors) {
		if (grid[row][col] >= grid[n.first][n.second]) {
			return false;
		}
	}
	return true;
}

// Recursively count neighboring basin points (not 9 and haven't been counted already)
static int get_num_nearby_basin_points(int row, int col,
				       const std::vector<std::vector<int>>& grid)
{
	if (row < 0 || col < 0 || row >= grid.size() || col >= grid[0].size()) {
		printf("%s: invalid input params.\n", __func__);
		return -1;
	}

	// Keeps track of visited points as recursion does its job
	static std::vector<std::pair<int, int>> visited;

	std::vector<std::pair<int, int>> neighbors;
	get_neighbors(row, col, grid, neighbors);

	int basin_size = 0;
	for (auto n: neighbors) {
		if (grid[n.first][n.second] != 9) {
			if (std::find(visited.begin(), visited.end(), n) == visited.end()) {
				// Point has not been visited before so count it and
				// count its basin point neighbors
				visited.push_back(std::make_pair(n.first, n.second));
				basin_size += 1 + get_num_nearby_basin_points(
							n.first, n.second, grid);
			}
		}
	}

	return basin_size;
}

int main()
{
	std::vector<std::vector<int>> heightmap;
	std::vector<int> low_points;
	std::vector<int> basin_sizes;

	// Parse input into heightmap
	std::ifstream input("../day9/input.txt");
	for (std::string line; std::getline(input, line); ) {
		std::vector<int> line_nums;
		for (char c: line) {
			line_nums.push_back(c - '0');
		}
		heightmap.push_back(line_nums);
	}

	// Find low points and basins
	for (int row = 0; row < heightmap.size(); row++) {
		for (int col = 0; col < heightmap[0].size(); col++) {
			if (is_low_point(row, col, heightmap))
			{
				// All low points are at the bottom of a basin
				low_points.push_back(heightmap[row][col]);
				int basin_size = get_num_nearby_basin_points(row, col, heightmap);
				if (basin_size > 0) {
					basin_sizes.push_back(basin_size);
				}
			}
		}
	}

	// Part 1: sum risk level of all low points
	int total_risk = 0;
	for (int p: low_points) {
		total_risk += p + 1;
	}

	printf("Part 1: %d\n", total_risk);

	// Part 2: find 3 largest basins and multiply their sizes together
	if (basin_sizes.size() >= 3) {
		std::sort(basin_sizes.begin(), basin_sizes.end(), std::greater<int>());
		printf("Part 2: %d\n", basin_sizes[0] * basin_sizes[1] * basin_sizes[2]);
	}

	return 0;
}
