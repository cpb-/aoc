use std::io;
use std::io::prelude::*;
use std::io::BufReader;
use std::collections::VecDeque;
use std::collections::HashSet;


fn main() -> io::Result<()> {

	let reader = BufReader::new(io::stdin());

	let mut grid:  Vec<Vec<char>> = vec![];

	for line in reader.lines() {
		let strline = line.unwrap();
		grid.push(strline.chars().collect());
	}

	let minute = search_path(&grid, 1, 0, grid[0].len() - 2, grid.len() - 1);
	println!("Exit found after {} minutes", minute);
	
	Ok(())
}


fn search_path(grid: &Vec<Vec<char>>, startx: usize, starty: usize, endx: usize, endy: usize) -> usize {

	let mut test_queue: VecDeque<(usize, usize, usize)> = VecDeque::new(); // (x, y, minute)
	test_queue.push_back((startx, starty, 0));

	let mut history: HashSet<(usize, usize, usize)> = HashSet::new();
	history.insert((startx, starty, 0));

	while let Some(pos) = test_queue.pop_front() {

		let coord = (pos.0, pos.1);
		let minute = pos.2;

		if coord == (endx, endy) {
			return minute;
		}

		if coord.1 > 1
		 &&  ! blizzard_at(coord.0, coord.1 - 1, minute + 1, &grid)
		 &&  ! history.contains(&(coord.0, coord.1 - 1, minute + 1)) {
			history.insert((coord.0, coord.1 - 1, minute + 1));
			test_queue.push_back((coord.0, coord.1 - 1, minute + 1));
		}

		if coord.1 < grid.len() - 1
		 && ! blizzard_at(coord.0, coord.1 + 1, minute + 1, &grid)
		 && ! history.contains(&(coord.0, coord.1 + 1, minute + 1)) {
			history.insert((coord.0, coord.1 + 1, minute + 1));
			test_queue.push_back((coord.0, coord.1 + 1, minute + 1));
		}


		if coord.0 > 1
		 && ! blizzard_at(coord.0 - 1, coord.1, minute + 1, &grid)
		 && ! history.contains(&(coord.0 - 1, coord.1, minute + 1)) {
			history.insert((coord.0 - 1, coord.1, minute + 1));
			test_queue.push_back((coord.0 - 1, coord.1, minute + 1));
		}

		if coord.0 < grid[0].len() - 1
		 && ! blizzard_at(coord.0 + 1, coord.1, minute + 1, &grid)
		 && ! history.contains(&(coord.0 + 1, coord.1, minute + 1)) {
			history.insert((coord.0 + 1, coord.1, minute + 1));
			test_queue.push_back((coord.0 + 1, coord.1, minute + 1));
		}

		if  ! blizzard_at(coord.0, coord.1, minute + 1, &grid)
		 && ! history.contains(&(coord.0, coord.1, minute + 1)) {
			history.insert((coord.0, coord.1, minute + 1));
			test_queue.push_back((coord.0, coord.1, minute + 1));
		}

	}

	return 0;
}



fn blizzard_at(x: usize, y: usize, minute: usize, grid: &Vec<Vec<char>>)  -> bool {

	if grid[y][x] == '#' { // Forbidden.
		return true;
	}

	let height = grid.len();
	let width = grid[0].len();

	// Check if there was a blizzard pointing at us.
	let mut bliz: usize;

	if (y == 0) || (y == height - 1)  { // Start and exit points
		return false;
	}

	bliz = 1 + ((x - 1) + minute) % (width - 2);
	if grid[y][bliz] == '<' {
		return true;
	}
	bliz = 1 + ((x - 1) + (width - 2) - minute % (width - 2)) % (width - 2);
	if grid[y][bliz] == '>' {
		return true;
	}
	bliz = 1 + ((y - 1) + minute) % (height - 2);
	if grid[bliz][x] == '^' {
		return true;
	}
	bliz = 1 + ((y - 1) + (height - 2) - minute % (height - 2)) % (height - 2);
	if grid[bliz][x] == 'v' {
		return true;
	}
	return false;
}

