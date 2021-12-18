use std::cmp::Ordering;
use std::fs;

const REPORT_PATH: &str = "report.txt";

fn part_1() {
    let mut prev: u32;
    let mut cur: u32 = 0;
    let mut larger = 0;
    let mut first_measurement = true;
    let mut result_string;

    let report = fs::read_to_string(REPORT_PATH).unwrap().trim().to_string();
    let report_data = report.split('\n');
    for val in report_data {
        prev = cur;
        cur = val.parse().unwrap();

        if first_measurement {
            first_measurement = false;
            println!("{} (N/A - no previous measurement)", cur);
            continue;
        }

        match cur.cmp(&prev) {
            Ordering::Greater => {
                larger += 1;
                result_string = String::from("increased")
            }

            Ordering::Less => result_string = String::from("decreased"),
            Ordering::Equal => result_string = String::from("no change"),
        }

        println!("{} ({})", cur, result_string);
    }

    println!("\n{} measurements were larger than the previous", larger);
}

fn part_2() {
    let mut prev_win_sum: u32;
    let mut cur_win_sum: u32 = 0;
    let mut first_measurement = true;
    let mut sums_larger_than_prev: u32 = 0;

    let report = fs::read_to_string(REPORT_PATH).unwrap().trim().to_string();
    let data: Vec<u32> = report.split('\n').map(|x| x.parse().unwrap()).collect();
    let windows = data.windows(3);

    for window in windows {
        prev_win_sum = cur_win_sum;
        cur_win_sum = window.iter().sum();

        if first_measurement {
            first_measurement = false;
            continue;
        }

        if cur_win_sum > prev_win_sum {
            sums_larger_than_prev += 1;
        }
    }

    println!(
        "{} window sums larger than the previous sum.",
        sums_larger_than_prev
    );
}

fn main() {
    part_1();
    part_2();
}
