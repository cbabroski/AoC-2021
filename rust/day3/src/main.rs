use std::cmp::Ordering;

const INPUT_NUM_BITS: u16 = 12;

fn part_1() {
    // Will break if input format is not correct, assumes same size, no error handling...
    let raw_input = include_str!("../input.txt");

    let mut num_rows = 0;
    let mut sums: Vec<u32> = Vec::new();
    let mut val: u32;
    let mut first_line = true;

    for line in raw_input.lines() {
        for (i, c) in line.trim().chars().enumerate() {
            val = c.to_digit(10).unwrap();
            if first_line {
                sums.push(val);
            } else {
                sums[i] += val;
            }
        }

        num_rows += 1;
        first_line = false;
    }

    // Reverse sums so the index of each sum can be shifted to select the correct bit in gamma
    // (e.g. sums = [bit0, bit1, bit2, ...], gamma = ... bit2 bit1 bit0).
    sums.reverse();

    let mut gamma: u32 = 0;
    let mut epsillon: u32 = 0;
    for (i, sum) in sums.iter().enumerate() {
        match sum.cmp(&(num_rows / 2)) {
            Ordering::Greater => gamma |= 1 << i,
            _ => epsillon |= 1 << i,
        }
    }

    //println!("reversed sums: {:?}, num_rows: {}", sums, num_rows);
    println!("gamma: {}, epsillon: {}", gamma, epsillon);
    println!("power consumption: {}", gamma * epsillon);
}

#[derive(PartialEq)]
enum RatingType {
    OXY,
    C02,
}

fn get_bit_pos(bit_pos: u16, bits: u16) -> u16 {
    (bits >> (INPUT_NUM_BITS - bit_pos)) & 0x01
}

fn get_rating(rating_type: RatingType, mut input: Vec<u16>) -> u16 {
    let mut bit_pos = 1;
    while input.len() > 1 {
        // Sum all values at the current bit position under inspection
        let sum_of_bit_pos_vals = input
            .iter()
            .fold(0, |acc, &x| acc + get_bit_pos(bit_pos, x));

        // Use the calculated sum to determine if 1 or 0 is the most common value
        let mut most_common_value;
        if (sum_of_bit_pos_vals as f32) < (input.len() as f32 / 2.0) {
            most_common_value = 0;
        } else {
            most_common_value = 1;
        }

        if rating_type == RatingType::C02 {
            // Opposite criteria
            most_common_value = !most_common_value & 0x01;
        }

        // Filter out numbers that don't match bit criteria
        input = input
            .iter()
            .filter(|&&x| get_bit_pos(bit_pos, x) == most_common_value)
            .copied()
            .collect();

        // Continue on and look at the next bit position
        bit_pos += 1;
    }

    input.pop().unwrap()
}

fn part_2() {
    let raw_input = include_str!("../input.txt");
    let input: Vec<u16> = raw_input
        .lines()
        .map(|x| u16::from_str_radix(x, 2).unwrap())
        .collect();

    let oxy_gen_rating = get_rating(RatingType::OXY, input.to_vec());
    let c02_scrub_rating = get_rating(RatingType::C02, input.to_vec());
    println!(
        "oxygen generator rating: {}, CO2 scrubber rating: {}",
        oxy_gen_rating, c02_scrub_rating
    );
    println!("life support rating: {}", oxy_gen_rating as u32 * c02_scrub_rating as u32);
}

fn main() {
    part_1();
    part_2();
}
