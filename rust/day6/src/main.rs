const NUM_DAYS: i64 = 256;
const CYCLE_LEN: usize = 9;

fn main() {
    let input: Vec<i64> = include_str!("../input.txt")
        .trim()
        .split(',')
        .map(|x| x.parse().unwrap())
        .collect();

    let mut fish_at_cycle_day: [i64; CYCLE_LEN] = [0; CYCLE_LEN];
    for cycle_day in input {
        fish_at_cycle_day[cycle_day as usize] += 1;
    }

    for _ in 0..NUM_DAYS {
        let mut new_fish_at_cycle_day: [i64; CYCLE_LEN] = [0; CYCLE_LEN];

        // First decrement tickers for all fish, not counting rollover (day 0s)
        for cycle_day in 1..CYCLE_LEN {
                new_fish_at_cycle_day[cycle_day - 1] = fish_at_cycle_day[cycle_day];
        }

        // Handle rollover day 0s separately if there are any. Same number of new
        // fish are created as old fish rollover.
        if fish_at_cycle_day[0] > 0 {
            new_fish_at_cycle_day[6] += fish_at_cycle_day[0];
            new_fish_at_cycle_day[8] += fish_at_cycle_day[0];
        }

        fish_at_cycle_day = new_fish_at_cycle_day;
    }

    let total_fish: i64 = fish_at_cycle_day.iter().sum();
    println!("Num lanternfish after {} days: {}", NUM_DAYS, total_fish);
}
