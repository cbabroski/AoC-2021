#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

std::pair<std::string, std::string> decode_insertion_rule(const std::string& rule)
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


///// BRUTE FORCE /////
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

///// RECURSIVE /////
void add_elements_for_pair(int num_steps, const std::string& element_pair,
			   const std::unordered_map<std::string, char>& insertion_rule_map,
			   std::unordered_map<char, long>& element_count)
{
	if (num_steps <= 0) {
		return;
	}

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

std::unordered_map<char, long> recursive_count_elements(
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

	return element_count;
}

///// OPTIMIZED /////
std::unordered_map<char, long> optimized_count_elements(
		int num_steps, const std::string& polymer_template,
		const std::vector<std::string>& insertion_rules)
{
	// Create map of insertion rules for quicker access
	std::unordered_map<std::string, char> insertion_rule_map;
	for (std::string rule: insertion_rules) {
		auto p = decode_insertion_rule(rule);
		std::string insertion_match = p.first;
		char insertion_char = p.second[0];
		insertion_rule_map[insertion_match] = insertion_char;
	}

	// Start map of pair and element counts based on template.
	// Keep a map of pair counts rather than needing to update a string.
	std::unordered_map<std::string, long> pair_counts;
	std::unordered_map<char, long> element_count;
	element_count[polymer_template[0]]++;
	for (int c = 0; c < polymer_template.size() - 1; c++) {
		std::string el_pair = polymer_template.substr(c, 2);
		pair_counts[el_pair]++;
		element_count[el_pair[1]]++;
	}

	// Continue to update pair and element counts based on new pairs that are created
	// as new elements are inserted.
	std::unordered_map<std::string, long> pair_count_copy;
	for (int i = 0; i < num_steps; i++) {
		pair_count_copy = pair_counts;
		for (auto p: pair_count_copy) {
			if (p.second <= 0) {
				continue;
			}

			std::string el_pair = p.first;
			long count = p.second;
			char insertion = insertion_rule_map[el_pair];

			// Count new element
			element_count[insertion] += count;

			// Two new pairs are created after insertion
			std::string first_new_pair = "";
			first_new_pair.push_back(el_pair[0]);
			first_new_pair.push_back(insertion);
			std::string second_new_pair = "";
			second_new_pair.push_back(insertion);
			second_new_pair.push_back(el_pair[1]);

			// Since an element is being inserted and creates two new pairs,
			// need to reset/subtract count from the old pair.
			pair_counts[el_pair] -= count;

			// Then add the count of the two new pairs created
			pair_counts[first_new_pair] += count;
			pair_counts[second_new_pair] += count;
		}
	}

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

	std::ifstream input("../day14/input.txt");
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
	printf("Part 1 brute force: %ld\n", get_solution(element_count_part1, polymer_template));

	std::unordered_map<char, long> element_count_part2_test = recursive_count_elements(
		10, polymer_template, insertion_rules);
	printf("Part 1 recursive: %ld\n", get_solution(element_count_part2_test, polymer_template));

	std::unordered_map<char, long> element_count_part2 = optimized_count_elements(
		40, polymer_template, insertion_rules);
	printf("Part 2 optimized: %ld\n", get_solution(element_count_part2, polymer_template));
	return 0;
}
