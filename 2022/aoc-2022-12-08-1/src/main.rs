
use std::io;
use std::io::prelude::*;
use std::io::BufReader;


fn main() -> io::Result<()> {

	let reader = BufReader::new(io::stdin());

	let mut trees: Vec<Vec<char>> = vec![];
	let mut visible: Vec<Vec<bool>> = vec![];

	for line in reader.lines() {
		trees.push(line.unwrap().chars().collect());
	}

	let mut y: usize;
	let mut x: usize;

	let height: usize = trees.len();
	let width: usize = trees[0].len();

	// Create the visible array
	y = 0;
	while y < height {
		visible.push(vec![]);
		x = 0;
		while x < width {
			visible[y].push(false);
			x += 1;
		}
		y += 1;
	}

	// external cells are visibles
	x = 0;
	while x < width {
		visible[0][x] = true;
		visible[height - 1][x] = true;
		x += 1;
	}
	y = 1;
	while y < height - 1 {
		visible[y][0] = true;
		visible[y][width - 1] = true;
		y += 1;
	}

	
	y = 1;
	while y < height - 1 {
		let mut max: u32 = trees[y][0].to_digit(10).unwrap();
		x = 1;
		loop {
			let value: u32 = trees[y][x].to_digit(10).unwrap();
			if value > max {
				max = value;
				visible[y][x] = true;
			}
			if x == width - 2 {
				break;
			}
			x += 1;
		}

		max = trees[y][width - 1].to_digit(10).unwrap();
		x = width - 2;
		loop {
			let value: u32 = trees[y][x].to_digit(10).unwrap();
			if value > max {
				visible[y][x] = true;
				max = value;
			}
			if x == 1 {
				break;
			}
			x -= 1;
		}

		y += 1;
	}

	x = 1;
	while x < width - 1 {
		let mut max: u32 = trees[0][x].to_digit(10).unwrap();
		y = 1;
		loop {
			let value: u32 = trees[y][x].to_digit(10).unwrap();
			if value > max {
				visible[y][x] = true;
				max = value;
			}
			if y == height - 2 {
				break;
			}
			y += 1;
		}

		max = trees[height - 1][x].to_digit(10).unwrap();
		y = height - 2;
		loop {
			let value: u32 = trees[y][x].to_digit(10).unwrap();
			if value > max {
				visible[y][x] = true;
				max = value;
			}
			if y == 1 {
				break;
			}
			y -= 1;
		}

		x += 1;
	}

	let mut visibles: i32 = 0;
	y = 0;
	while y < height {
		x = 0;
		while x < width {
			if visible[y][x] {
				visibles += 1;
			}
			x += 1;
		}
		y += 1;
	}

	println!("Nb visible: {}", visibles);

	Ok(())
}

