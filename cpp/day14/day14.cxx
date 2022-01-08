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

// Too slow for > 15 steps
std::unordered_map<char, long> brute_force_count_elements(
		int num_steps, const std::string& polymer_template,
		const std::vector<std::string>& insertion_rules)
{
	std::string polymer_start = polymer_template;
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

	std::unordered_map<char, long> element_count;
	for (char e: polymer_start) {
		element_count[e]++;
	}

	return element_count;
}

void add_elements_for_pair(int num_steps, const std::string& element_pair,
			   const std::unordered_map<std::string, char>& insertion_rule_map,
			   std::unordered_map<char, long>& element_count)
{
	if (num_steps <= 0) {
		return;
	}

	// TODO if element_pair not in map, return?

	/* char insertion_char = insertion_rule_map[element_pair]; */
	char insertion_char = insertion_rule_map.at(element_pair);
	element_count[insertion_char]++;
	num_steps--;

	std::string next_pair = "";
	next_pair.push_back(element_pair[0]);
	next_pair.push_back(insertion_char);
	add_elements_for_pair(num_steps, next_pair, insertion_rule_map, element_count);

	next_pair = "";
	next_pair.push_back(insertion_char);
	next_pair.push_back(element_pair[1]);
	add_elements_for_pair(num_steps, next_pair, insertion_rule_map, element_count);
}

std::unordered_map<char, long> optimized_count_elements(
		int num_steps, const std::string& polymer_template,
		const std::vector<std::string>& insertion_rules)
{
	std::unordered_map<char, long> element_count;
	for (char e: polymer_template) {
		element_count[e]++;
	}

	// Create map of insertion rules for quicker access
	std::unordered_map<std::string, char> insertion_rule_map;
	for (std::string rule: insertion_rules) {
		auto p = decode_insertion_rule(rule);
		std::string insertion_match = p.first;
		char insertion_char = p.second[0];
		insertion_rule_map[insertion_match] = insertion_char;
	}

	// Recursively search through element pairs as new ones are added
	for (int c = 0; c < polymer_template.size() - 1; c++) {
		std::string two_chars = polymer_template.substr(c, 2);
		add_elements_for_pair(num_steps, two_chars, insertion_rule_map, element_count);
	}

	printf("finished counting elements\n");
	return element_count;
}

long get_solution(std::unordered_map<char, long>& element_count,
		  const std::string& polymer_template)
{
	// Find least and most common elements
	long most_common_num = 0;
	long least_common_num = element_count[polymer_template[0]];
	for (auto element: element_count) {
		if (element.second > most_common_num) {
			most_common_num = element.second;
		}

		if (element.second < least_common_num) {
			least_common_num = element.second;
		}
	}

	return most_common_num - least_common_num;
}

int main()
{
	std::string polymer_template;
	std::vector<std::string> insertion_rules;

	std::ifstream input("../day14/sample-input.txt");
	for (std::string line; std::getline(input, line); ) {
		if (line == "") {
			continue;
		}

		if (polymer_template.size() == 0) {
			polymer_template = line;
			continue;
		}
		
		insertion_rules.push_back(line);
	}

	std::unordered_map<char, long> element_count_part1 = brute_force_count_elements(
		10, polymer_template, insertion_rules);
	printf("Part 1: %ld\n", get_solution(element_count_part1, polymer_template));

	// TODO remove when fast
	std::unordered_map<char, long> element_count_part2_test = optimized_count_elements(
		10, polymer_template, insertion_rules);
	printf("Part 2 for 10 (TEST): %ld\n", get_solution(element_count_part2_test,
								polymer_template));

	std::unordered_map<char, long> element_count_part2 = optimized_count_elements(
		40, polymer_template, insertion_rules);
	printf("Part 2: %ld\n", get_solution(element_count_part2, polymer_template));
	return 0;
}
