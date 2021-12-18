fn part_1() {
    const BOARD_WIDTH: usize = 5;
    const WIN_SUM: i32 = -(BOARD_WIDTH as i32);
    let (nums, boards) = include_str!("../input.txt").split_once("\n\n").unwrap();

    // Map numbers to draw to a vector of i32s in reverse order so they can be popped/"drawn"
    let mut nums_to_draw: Vec<i32> = nums
        .split(',')
        .map(|num| num.parse().unwrap())
        .rev()
        .collect();

    // Map all board strings into one 1d vector containing all board numbers
    let mut boards: Vec<i32> = boards
        .split("\n\n")
        .flat_map(|board| {
            board
                .split('\n')
                .flat_map(|line| line.split_whitespace().map(|num| num.parse().unwrap()))
        })
        .collect();

    while nums_to_draw.len() > 1 {
        // Draw a number and mark the boards
        let drawn_num = nums_to_draw.pop().unwrap();
        boards = boards
            .iter()
            .map(|&x| if x == drawn_num { -1 } else { x })
            .collect();

        // For each board: sum all of the rows and all of the columns. If
        // any of these sums is -BOARD_WIDTH then that board won bingo.
        for board in boards.chunks(BOARD_WIDTH.pow(2)) {
            for i in 0..BOARD_WIDTH {
                let row_sum: i32 = board.chunks(BOARD_WIDTH).nth(i).unwrap().iter().sum();
                let col_sum: i32 = board.iter().skip(i).step_by(BOARD_WIDTH).sum();
                if row_sum == WIN_SUM || col_sum == WIN_SUM {
                    // BINGO!
                    let unmarked_sum: i32 = board.iter().filter(|&&x| x != -1).sum();
                    println!("Part 1 score: {}", unmarked_sum * drawn_num);
                    return;
                }
            }
        }
    }
}

fn part_2() {
    const BOARD_WIDTH: usize = 5;
    const WIN_SUM: i32 = -(BOARD_WIDTH as i32);
    let (nums, boards) = include_str!("../input.txt").split_once("\n\n").unwrap();

    // Map numbers to draw to a vector of i32s in reverse order so they can be popped/"drawn"
    let mut nums_to_draw: Vec<i32> = nums
        .split(',')
        .map(|num| num.parse().unwrap())
        .rev()
        .collect();

    // Map all board strings into one 1d vector containing all board numbers
    let mut boards: Vec<i32> = boards
        .split("\n\n")
        .flat_map(|board| {
            board
                .split('\n')
                .flat_map(|line| line.split_whitespace().map(|num| num.parse().unwrap()))
        })
        .collect();

    while nums_to_draw.len() > 1 {
        // Draw a number and mark the boards
        let drawn_num = nums_to_draw.pop().unwrap();
        boards = boards
            .iter()
            .map(|&x| if x == drawn_num { -1 } else { x })
            .collect();

        // For each board: sum all of the rows and all of the columns. If
        // any of these sums is -BOARD_WIDTH then that board got bingo.
        let mut board_indxs_to_remove = Vec::new();
        for (board_idx, board) in boards.chunks(BOARD_WIDTH.pow(2)).enumerate() {
            for i in 0..BOARD_WIDTH {
                let row_sum: i32 = board.chunks(BOARD_WIDTH).nth(i).unwrap().iter().sum();
                let col_sum: i32 = board.iter().skip(i).step_by(BOARD_WIDTH).sum();
                if row_sum == WIN_SUM || col_sum == WIN_SUM {
                    // BINGO!
                    if boards.len() <= BOARD_WIDTH.pow(2) {
                        // Last board to win
                        let unmarked_sum: i32 = board.iter().filter(|&&x| x != -1).sum();
                        println!("Part 2 score: {}", unmarked_sum * drawn_num);
                        return;
                    } else {
                        board_indxs_to_remove.push(board_idx);
                        break;
                    }
                }
            }
        }

        // Remove all boards that won on the last draw. As each board is removed,
        // the board index to remove also has to be subtracted to match the number
        // of boards removed.
        for (loop_idx, board_idx) in board_indxs_to_remove.iter().enumerate() {
            boards.drain(
                (BOARD_WIDTH.pow(2) * (board_idx - loop_idx))
                    ..(BOARD_WIDTH.pow(2) * (board_idx + 1 - loop_idx)),
            );
        }
    }
}

fn main() {
    part_1();
    part_2();
}
