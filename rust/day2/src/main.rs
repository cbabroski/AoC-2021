use std::fs::File;
use std::io::{BufRead, BufReader};
use std::str::FromStr;

const COURSE_PATH: &str = "course.txt";

#[derive(Debug)]
struct Position {
    horizontal: i32,
    depth: i32,
    aim: i32,
}

enum Direction {
    Forward,
    Up,
    Down,
}

struct CourseInput {
    direction: Direction,
    amount: i32,
}

impl FromStr for Direction {
    type Err = ();
    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s {
            "forward" => Ok(Direction::Forward),
            "up" => Ok(Direction::Up),
            "down" => Ok(Direction::Down),
            _ => Err(()),
        }
    }
}

fn main() {
    let mut pos = Position {
        horizontal: 0,
        depth: 0,
        aim: 0,
    };

    let f = File::open(COURSE_PATH).unwrap();
    let reader = BufReader::new(f);

    for line in reader.lines() {
        let raw_input_string = line.unwrap().trim().to_string();
        let raw_input: Vec<&str> = raw_input_string.split(' ').collect();
        let input = CourseInput {
            direction: Direction::from_str(raw_input[0]).unwrap(),
            amount: raw_input[1].parse().unwrap(),
        };

        match input.direction {
            Direction::Forward => {
                pos.horizontal += input.amount;
                pos.depth += pos.aim * input.amount;
            }
            Direction::Up => pos.aim -= input.amount,
            Direction::Down => pos.aim += input.amount,
        }
    }

    println!("{:?}, multiple: {}", pos, pos.horizontal * pos.depth);
}
