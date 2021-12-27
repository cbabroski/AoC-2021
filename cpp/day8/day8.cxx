#include <cstdio>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <cmath>

static bool are_segments_a_in_b(const std::string& a, const std::string& b)
{
	if (a.length() > b.length()) {
		printf("a has more segments than b. Expect that b should have more.\n");
		return false;
	}

	for (auto letter: a) {
		if (b.find(letter) == std::string::npos) {
			return false;
		}
	}

	return true;
}

// Returns number of similar/shared segments in a and b
static int get_num_similar_segments(const std::string& a, const std::string& b)
{
	int total_similarities = 0;
	std::string all_possible_segments = "abcdefg";
	for (auto segment: all_possible_segments) {
		bool found_in_a = false;
		bool found_in_b = false;
		if (a.find(segment) != std::string::npos) {
			found_in_a = true;
		}

		if (b.find(segment) != std::string::npos) {
			found_in_b = true;
		}

		if (found_in_a && found_in_b) {
			total_similarities++;
		}
	}

	return total_similarities;
}

static bool num_segments_is_unique(int num_segments)
{
	if (num_segments < 0 || num_segments > 7) {
		printf("%s: invalid input.\n", __func__);
		return false;
	}

	switch (num_segments) {
		case 2:
		case 3:
		case 4:
		case 7:
			return true;
		default:
			return false;
	}
}


// Returns number represented by segments, or -1 if not a unique number of segments
static int decode_unique_num_segments(const std::string& segments)
{
	int num_segments = segments.length();
	switch (num_segments) {
		case 2:
			return 1;
		case 3:
			return 7;
		case 4:
			return 4;
		case 7:
			return 8;
		default:
			return -1;
	}
}

// Returns number represented by segments, or -1 on error
static int decode_segments(const std::string& segments,
			   std::unordered_map<int, std::string>& unique_nums)
{
	int num_segments = segments.length();
	switch (num_segments) {
		case 2:
		case 3:
		case 4:
		case 7:
			return decode_unique_num_segments(segments);
		case 5:
		{
			// Length 5 and contains 7 -> 3
			if (are_segments_a_in_b(unique_nums[7], segments)) {
				return 3;
			}

			// To tell 2 and 5 apart we can compare the number of shared
			// segments with digit 4.
			int similarities = get_num_similar_segments(unique_nums[4], segments);
			if (similarities == 3) {
				return 5;
			} else if (similarities == 2) {
				return 2;
			} else {
				printf("Unknown similarities: %d\n", similarities);
			}

			return -1;
		}
		case 6:
		{
			// Length 6 and contains 4 -> 9
			if (are_segments_a_in_b(unique_nums[4], segments)) {
				return 9;
			}

			// Length 6, contains 1, not 9 -> 0
			if (are_segments_a_in_b(unique_nums[1], segments)) {
				return 0;
			}

			// Else has to be 6
			return 6;
		}
		default:
			printf("Unknown num segments %d\n", num_segments);
			return -1;
	}
}

int main()
{
	// Every 10 input values contains the input values of a single line
	std::vector<std::string> input_vals;

	// Every 4 output values contains the output values of a single line
	std::vector<std::string> output_vals;

	int num_input_lines = 0;
	bool output_val = false;
	std::string str;
	std::ifstream input;
	input.open("../day8/input.txt");
	while (input >> str)
	{
		if (str == "|") {
			output_val = true;
			continue;
		}

		if (output_val) {
			output_vals.push_back(str);
		} else {
			input_vals.push_back(str);
		}

		if (input.peek() == '\n') {
			output_val = false;
			num_input_lines++;
		}
	}
	input.close();

	// For part 1 count how many output values use a unique number of segments
	int num_unique_numbers = 0;
	int num_segments = 0;
	for (auto s: output_vals) {
		num_segments = s.length();
		if (num_segments_is_unique(num_segments)) {
			num_unique_numbers++;
		}
	}

	printf("Part 1 - num unique output numbers: %d\n", num_unique_numbers);

	// For part 2 compare segments from known unique numbers to others
	// to figure out what number is represented by the segments. Combine
	// the output digits and add to a total sum.
	const int num_inputs_per_line = 10;
	const int num_outputs_per_line = 4;
	int total_sum_of_outputs = 0;
	for (int line = 0; line < num_input_lines; line++) {
		// Save unique numbers from inputs in a map where the digit is the key
		// and segments are the value
		std::unordered_map<int, std::string> unique_numbers;
		int start = line * num_inputs_per_line;
		int end = start + num_inputs_per_line;
		for (int i = start; i < end; i++) {
			int decoded_num = decode_unique_num_segments(input_vals[i]);
			if (decoded_num != -1) {
				unique_numbers[decoded_num] = input_vals[i];
			}
		}

		// Decode outputs and combine with base 10 math
		start = line * num_outputs_per_line;
		end = start + num_outputs_per_line;
		int output_num = 0;
		for (int i = start; i < end; i++) {
			int num = decode_segments(output_vals[i], unique_numbers);
			output_num += num * pow(10, end - 1 - i);
		}

		// Add the digits of this line to the total sum
		total_sum_of_outputs += output_num;
	}

	printf("Part 2 - total sum of outputs: %d\n", total_sum_of_outputs);
	return 0;
}
