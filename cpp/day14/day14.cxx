#include <fstream>
#include <string>
#include <vector>

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
	insertion_char = rule.substr(delimiter_pos + 4, rule.length() - delimiter_pos - 4);
	printf("made ins point: %s, ins char: %s\n", insertion_point.c_str(),
		insertion_char.c_str());
	return std::make_pair(insertion_point, insertion_char);
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

	const int num_steps = 1; // TODO make 10 when ready
	for (int i = 0; i < num_steps; i++) {
		std::string polymer_template_new = polymer_template;
		for (std::string rule: insertion_rules) {
			printf("insertion rule: %s\n", rule.c_str());
			auto p = decode_insertion_rule(rule);
			std::string insertion_point = p.first;
			std::string insertion_char = p.second;

			// TODO insertion idx is based on original string,
			// so it's invalid for new string
			auto insertion_idx = polymer_template.find(insertion_point);
			if (insertion_idx != std::string::npos) {
				polymer_template_new.insert(insertion_idx + 1, insertion_char);
			}
		}
		polymer_template = polymer_template_new;
		printf("%s\n", polymer_template.c_str());
	}

	return 0;
}
