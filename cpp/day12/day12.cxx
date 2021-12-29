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
	// can only be visited once (-1 for error)
	int count_paths(const std::string& src, const std::string& dst);

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

int CaveGraph::count_paths(const std::string& src, const std::string &dst)
{
	static int path_count = 0;
	static std::vector<std::string> visited;

	// Have we reached the destination?
	visited.push_back(src);
	if (src.compare(dst) == 0) {
		path_count++;
	} else {
		// If not search all neighbors recursively
		for (const std::string& n: m_neighbors[src]) {
			if (is_cave_small(n)) {
				// TODO first solve part 2 then figure out how to also have
				// part 1 get printed
				auto found = std::find(visited.begin(), visited.end(), n);
				if (found != visited.end()) {
					// Small cave has already been visited so skip
					continue;
				}
			}
			count_paths(n, dst);
		}
	}

	// Lastly, backtrack and look for other paths
	auto last_visited_src = std::find(visited.begin(), visited.end(), src);
	visited.erase(last_visited_src);
	return path_count;
}

int main()
{
	CaveGraph cave;
	std::string connection;
	std::ifstream input("../day12/sample-input1.txt");
	while (input >> connection) {
		cave.add_connection(connection);
	}

	cave.display_connections();
	cave.display_caves_and_neighbors();
	printf("Part 1: %d\n", cave.count_paths("start", "end"));
	// TODO for part 2 we can now visit ONE small cave that is not start or end twice
	return 0;
}
