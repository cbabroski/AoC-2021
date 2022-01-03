#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

static std::pair<std::string, std::string> decode_insertion_rule(const std::string& rule)
{
	std::string delimiter = " -> ";
	auto delimiter_pos = rule.find(delimiter);
	if (delimiter_pos == std::string::npos) {
		printf("'%s' not found in string '%s'.\n", delimiter.c_str(), rule.c_str());
		return std::make_pair("N/A", "N/A");
	}

	std::string insertion_point;
	std::string insertion_char;
	insertion_point = rule.substr(0, delimiter_pos);
	insertion_char = rule.substr(delimiter_pos + delimiter.size(),
				     rule.length() - delimiter_pos - delimiter.size());
	return std::make_pair(insertion_point, insertion_char);
}

int main()
{
	std::string polymer_start;
	std::vector<std::string> insertion_rules;

	std::ifstream input("../day14/input.txt");
	for (std::string line; std::getline(input, line); ) {
		if (line == "") {
			continue;
		}

		if (polymer_start.size() == 0) {
			polymer_start = line;
			continue;
		}
		
		insertion_rules.push_back(line);
	}

	// TODO too slow to finish part 2
	// TODO when optimized, put in function so can run with different num steps
	// for both parts
	const int num_steps = 10;
	for (int step = 0; step < num_steps; step++) {
		// Form new polymer starting with the first element
		std::string polymer = "";
		polymer.push_back(polymer_start[0]);

		// Iterate over the starting polymer 2 chars at a time and
		// look up the insertion rule for each set of chars
		for (int c = 0; c < polymer_start.size() - 1; c++) {
			std::string two_chars = polymer_start.substr(c, 2);
			for (std::string rule: insertion_rules) {
				auto p = decode_insertion_rule(rule);
				std::string insertion_point = p.first;
				std::string insertion_char = p.second;
				if (insertion_point == two_chars) {
					// Due to element overlaps only need to push 2 chars
					polymer.push_back(insertion_char[0]);
					polymer.push_back(insertion_point[1]);
					break;
				}
			}
		}

		// Repeat with new polymer
		polymer_start = polymer;
	}

	// Find quantities of each element in the new polymer
	std::unordered_map<char, long> element_count;
	for (char e: polymer_start) {
		element_count[e]++;
	}

	// Find least and most common elements
	long most_common_num = 0;
	long least_common_num = element_count[polymer_start[0]];
	for (auto element: element_count) {
		if (element.second > most_common_num) {
			most_common_num = element.second;
		}

		if (element.second < least_common_num) {
			least_common_num = element.second;
		}
	}

	printf("Part 1: %ld\n", most_common_num - least_common_num);
	return 0;
}
