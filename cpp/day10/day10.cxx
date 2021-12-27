#include <vector>
#include <string>
#include <fstream>

static std::string get_complete_set(const char& brace)
{
	std::string completed;
	switch (brace) {
		case '(':
			completed = "()";
			break;
		case '[':
			completed = "[]";
			break;
		case '{':
			completed = "{}";
			break;
		case '<':
			completed = "<>";
			break;
		default:
			completed = "ERROR";
			break;
	}

	return completed;
}

static std::string build_expected_line(const std::string& input_line)
{
	// For all open brackets, close the brackets and insert into the expected string
	std::string expected_line = "";
	const std::string closing_brackets = ")]}>";
	for (int i = 0; i < input_line.length(); i++) {
		char c = input_line[i];
		if (closing_brackets.find(c) == std::string::npos) {
			std::string complete_set = get_complete_set(c);
			expected_line.insert(i, complete_set);
		}
	}

	return expected_line;
}

static int get_error_score(const char& brace)
{
	switch (brace) {
		case ')':
			return 3;
		case ']':
			return 57;
		case '}':
			return 1197;
		case '>':
			return 25137;
		default:
			printf("Unknown char %c, can't calculate error score.\n", brace);
			return -1;
	}
}

static int get_completion_points(const char& brace)
{
	switch (brace) {
		case ')':
			return 1;
		case ']':
			return 2;
		case '}':
			return 3;
		case '>':
			return 4;
		default:
			printf("Unknown char %c, can't calculate completion score.\n", brace);
			return -1;
	}
}

int main()
{
	std::vector<std::string> input_lines;
	std::vector<std::string> expected_lines;
	std::vector<int> incomplete_line_indexes;

	// Save input lines and build expected lines
	std::string input_line;
	std::ifstream input("../day10/input.txt");
	while (input >> input_line) {
		std::string expected_line = build_expected_line(input_line);
		input_lines.push_back(input_line);
		expected_lines.push_back(expected_line);
	}

	// Part 1: compare the input lines to expected lines looking for mismatched
	// characters and tally up the syntax error score based on those mismatched chars.
	int syntax_error_score = 0;
	for (int i = 0; i < input_lines.size(); i++) {
		bool corrupted_line = false;
		for (int j = 0; j < input_lines[i].length(); j++) {
			char input = input_lines[i][j];
			char expected = expected_lines[i][j];
			if (input != expected) {
				corrupted_line = true;
				syntax_error_score += get_error_score(input);
			}
		}

		if (!corrupted_line) {
			incomplete_line_indexes.push_back(i);
		}
	}

	printf("Part 1: %d\n", syntax_error_score);


	// Part 2: Caculate completion scores by looking at the characters added to
	// create the expected lines. Then sort the scores and take the middle one.
	std::vector<long> completion_scores;
	for (int i: incomplete_line_indexes) {
		long completion_score = 0;
		for (int j = input_lines[i].length(); j < expected_lines[i].length(); j++) {
			completion_score *= 5;
			completion_score += get_completion_points(expected_lines[i][j]);
		}
		completion_scores.push_back(completion_score);
	}

	std::sort(completion_scores.begin(), completion_scores.end());
	size_t middle = completion_scores.size() / 2;
	printf("Part 2: %ld\n", completion_scores[middle]);

	return 0;
}
