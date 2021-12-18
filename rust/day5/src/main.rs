use std::collections::HashMap;

fn get_all_points_in_line(
    skip_diagonals: bool,
    start: (i32, i32),
    end: (i32, i32),
) -> Vec<(i32, i32)> {
    let (x1, y1) = start;
    let (x2, y2) = end;
    let mut points: Vec<(i32, i32)> = Vec::new();

    if skip_diagonals && !(x1 == x2 || y1 == y2) {
        return points;
    }

    if x1 == x2 {
        // Vertical lines
        let y_vals;
        if y1 > y2 {
            y_vals = y2..(y1 + 1);
        } else {
            y_vals = y1..(y2 + 1);
        }

        for y in y_vals {
            points.push((x1, y));
        }
    } else {
        // All other lines
        let x_vals;
        let m: i32 = (y2 - y1) / (x2 - x1);
        let b: i32 = y1 - (m * x1);

        if x1 > x2 {
            x_vals = x2..(x1 + 1);
        } else {
            x_vals = x1..(x2 + 1);
        }

        for x in x_vals {
            let y = (m * x) + b;
            points.push((x, y));
        }
    }

    points
}

fn get_num_occurrences(coords: Vec<(i32, i32)>) -> HashMap<(i32, i32), usize> {
    let mut num_occurrences = HashMap::new();
    for &coord in &coords {
        if num_occurrences.contains_key(&coord) {
            continue;
        }
        num_occurrences.insert(coord, coords.iter().filter(|&&c| c == coord).count());
    }
    num_occurrences
}

// TODO this is extremely slow and there is likely a much better way to do this...
fn main() {
    let mut input_coords: Vec<(i32, i32)> = Vec::new();
    let mut vent_coords: Vec<(i32, i32)> = Vec::new();
    let mut vent_coords_inc_diags: Vec<(i32, i32)> = Vec::new();

    // Map input coordinates into tuples and save
    include_str!("../sample-input.txt")
        .trim()
        .split('\n')
        .for_each(|input| {
            input
                .split(" -> ")
                .flat_map(|coord| coord.split(','))
                .map(|x| x.parse().unwrap())
                .collect::<Vec<i32>>()
                .chunks(2)
                .for_each(|coord| input_coords.push((coord[0], coord[1])));
        });

    // Iterate over input coordinates in sets of 2 at a time filling in the missing coordinates
    for coord_pair in input_coords.chunks(2) {
        vent_coords.append(&mut get_all_points_in_line(
            true,
            coord_pair[0],
            coord_pair[1],
        ));

        vent_coords_inc_diags.append(&mut get_all_points_in_line(
            false,
            coord_pair[0],
            coord_pair[1],
        ));
    }

    // Count how many times there is more than 1 occurrence of a point to get the final score
    let num_occurrences = get_num_occurrences(vent_coords);
    let count = num_occurrences.into_values().filter(|&v| v > 1).count();
    println!("part 1 answer: {}", count);

    let num_occurrences_inc_diags = get_num_occurrences(vent_coords_inc_diags);
    let count_inc_diags = num_occurrences_inc_diags
        .into_values()
        .filter(|&v| v > 1)
        .count();
    println!("part 2 answer: {}", count_inc_diags);
}
