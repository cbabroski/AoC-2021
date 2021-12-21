#include <cstdio>
#include <vector>
#include <algorithm>
#include <math.h>

int main()
{
	int num = 0;
	std::vector<int> values;

	FILE *fp = fopen("../day7/input.txt", "r");
	if (!fp) {
		printf("Failed to open input file.\n");
		return 1;
	}
	while (fscanf(fp, "%d,", &num) > 0) {
		values.push_back(num);
	}
	fclose(fp);
	std::sort(values.begin(), values.end());

	// Part 1 - the majority are near the median so for linear fuel usage
	// that's the most efficient reference
	int total_fuel = 0;
	int median = values[values.size() / 2];
	for (auto val: values) {
		if (val > median) {
			total_fuel += (val - median);
		} else {
			total_fuel += (median - val);
		}
	}
	printf("Part 1, total fuel: %d\n", total_fuel);

	// Part 2 - the mean is the mid point so less fuel is used for exponential steps away
	int mean = 0;
	for (auto val: values) {
		mean += val;
	}
	mean = round((float)mean / values.size());

	// The mean itself is sometimes not the correct answer, so test around the mean
	// to find the lowest fuel cost and use that
	int total_fuels[3];
	int idx = 0;
	for (int i = mean - 1; i <= mean + 1; i++) {
		total_fuel = 0;
		for (auto val: values) {
			int num_steps = 0;
			if (val > i) {
				num_steps = val - i;
			} else {
				num_steps = i - val;
			}

			int fuel_for_steps = 0;
			for (int j = 1; j <= num_steps; j++) {
				fuel_for_steps += j;
			}
			total_fuel += fuel_for_steps;
		}
		total_fuels[idx] = total_fuel;
		idx++;
	}

	std::sort(std::begin(total_fuels), std::end(total_fuels));
	printf("Part 2, total fuel: %d\n", total_fuels[0]);
	return 0;
}
