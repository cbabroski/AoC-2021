#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <cctype>

class CaveGraph
{
public:
	// Adds a connection given an input connection string of form "src-dst"
	void add_connection(std::string connection);

	// Prints all cave connections parsed from the input
	void display_connections(void);

	// Prints all caves and their neighbors
	void display_caves_and_neighbors(void);

	// Counts the total number of paths from src to dst given that small caves
	// can only be visited once (-1 for error).
	int count_paths(const std::string& src, const std::string &dst, bool allow_small_retry);

private:
	// Cave connections parsed from input
	std::vector<std::string> m_connections;

	// Map of all neighbors connected to a given cave
	std::unordered_map<std::string, std::vector<std::string>> m_neighbors;

	// Split input connection string
	int get_conn_src_and_dst(const std::string& connection, std::string& src,
				 std::string& dst);

	// Whether or not the cave is a small or big one
	bool is_cave_small(const std::string& cave);

	// Prints all neighbors of a given cave
	void display_neighbors(const std::string& cave);

	// Helper function for recursive search
	int count(int& path_count, const std::string& src, const std::string &dst,
		  bool allow_small_retry,
		  std::unordered_map<std::string, int>& num_times_cave_visited);
};

bool CaveGraph::is_cave_small(const std::string& cave)
{
	for (auto c: cave) {
		if (std::isupper(static_cast<unsigned char>(c))) {
			return false;
		}
	}
	return true;
}

void CaveGraph::display_neighbors(const std::string& cave)
{
	printf("%s (%s): ", cave.c_str(), is_cave_small(cave) ? "small" : "big");
	for (auto n: m_neighbors[cave]) {
		printf("%s (%s), ", n.c_str(), is_cave_small(n) ? "small" : "big");
	}
	printf("\n");
}

void CaveGraph::display_caves_and_neighbors(void)
{
	printf("Neighbors:\n");
	for (auto c: m_neighbors) {
		display_neighbors(c.first);
	}
	printf("\n");
}

int CaveGraph::get_conn_src_and_dst(const std::string& connection, std::string& src,
				    std::string& dst)
{
	std::string delimiter = "-";
	auto delimiter_pos = connection.find(delimiter);
	if (delimiter_pos == std::string::npos) {
		printf("'%s' not found in string '%s'.\n", delimiter.c_str(), connection.c_str());
		return -1;
	}

	src = connection.substr(0, delimiter_pos);
	dst = connection.substr(delimiter_pos + 1, connection.length() - delimiter_pos - 1);
	return 0;
}

void CaveGraph::add_connection(std::string connection)
{
	std::string src, dst;
	if (get_conn_src_and_dst(connection, src, dst) != 0) {
		printf("Could not get connections from string '%s'.\n", connection.c_str());
		return;
	}

	// Two way connections
	m_connections.push_back(connection);
	m_neighbors[src].push_back(dst);
	m_neighbors[dst].push_back(src);
}

void CaveGraph::display_connections(void)
{
	printf("Cave connections:\n");
	for (auto c: m_connections) {
		printf("%s\n", c.c_str());
	}
	printf("\n");
}

int CaveGraph::count(int& path_count, const std::string& src, const std::string &dst,
		     bool allow_small_retry,
		     std::unordered_map<std::string, int>& num_times_cave_visited)
{
	// Stop and backtrack when the destination has been reached
	num_times_cave_visited[src]++;
	if (src == dst) {
		path_count++;
		goto backtrack;
	}

	// Determine whether or not this cave can continue to be searched
	if (is_cave_small(src) && num_times_cave_visited[src] > 1) {
		if (!allow_small_retry) {
			goto backtrack;
		}

		if (src == "start" || src == "end") {
			goto backtrack;
		}

		if (num_times_cave_visited[src] > 2) {
			goto backtrack;
		}

		bool another_small_cave_was_already_visited_twice = false;
		for (auto it: num_times_cave_visited) {
			if (is_cave_small(it.first) && it.second > 1) {
				if (it.first != "start" && it.first != "end" && it.first != src) {
					another_small_cave_was_already_visited_twice = true;
					break;
				}
			}
		}

		if (another_small_cave_was_already_visited_twice) {
			goto backtrack;
		}
	}

	// Recursively search through all neighbor paths
	for (const std::string& n: m_neighbors[src]) {
		count(path_count, n, dst, allow_small_retry, num_times_cave_visited);
	}

backtrack:
	num_times_cave_visited[src]--;
	return path_count;
}

int CaveGraph::count_paths(const std::string& src, const std::string &dst, bool allow_small_retry)
{
	int path_count = 0;
	std::unordered_map<std::string, int> num_times_cave_visited = {};
	count(path_count, src, dst, allow_small_retry, num_times_cave_visited);
	return path_count;
}

int main()
{
	CaveGraph cave;
	std::string connection;
	std::ifstream input("../day12/input.txt");
	while (input >> connection) {
		cave.add_connection(connection);
	}

	cave.display_connections();
	cave.display_caves_and_neighbors();
	printf("Part 1: %d\n", cave.count_paths("start", "end", false));
	printf("Part 2: %d\n", cave.count_paths("start", "end", true));

	return 0;
}
