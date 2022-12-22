use std::io;
use std::io::prelude::*;
use std::io::BufReader;
//use std::collections::HashMap;


fn main() -> io::Result<()> {

	let mut reader = BufReader::new(io::stdin());

	let mut grid:  Vec<Vec<char>> = vec![];

	loop {
		let mut line: String = String::new();
		reader.read_line(&mut line).expect("Data should be provided on standard input.");

		if line.trim().is_empty() {
			break;
		}
		grid.push(line.chars().collect());
	}

	let mut path: String = String::new();
	reader.read_line(&mut path).expect("Data should be provided on standard input.");


	let mut y: usize = 0;
	let mut x: usize = 0;
	let mut heading: usize = 0; // 0->R, 1->D, 2->L, 3->T

	while grid[y][x] == ' ' {x +=1; }

	while ! path.trim().is_empty() {

		let mut ch: char = path.remove(0);

		let mut nb_steps: u32 = 0;
		while ch.is_digit(10) {
			nb_steps *= 10;
			nb_steps += ch as u32 - 48; // '0'
			ch = path.remove(0);
		}
		let mut new_heading: usize = heading;
		match ch {
			'R' => new_heading = (heading + 1) % 4,
			'L' => new_heading = (heading + 3) % 4,
			 _  => println!("End of data"),
		};

		let mut new_x: usize;
		let mut new_y: usize;

		for _ in 0..nb_steps {
			new_x = x;
			new_y = y;
			if heading == 0 { // Right
				loop {
					if new_x < grid[y].len() - 1 {
						new_x += 1;
					} else {
						new_x = 0;
					}
					if grid[y][new_x] == '.' {
						x = new_x;
						println!("R to ({}, {})", x, y);
						break;
					}
					if grid[y][new_x] == '#' {
						break;
					}
				}
			}
			if heading == 1 { // Down
				loop {
					if new_y < grid.len() - 1 {
						new_y += 1;
					} else {
						new_y = 0;
					}
					if x >= grid[new_y].len() {
						continue;
					}
					if grid[new_y][x] == '.' {
						y = new_y;
						println!("D to ({}, {})", x, y);
						break;
					}
					if grid[new_y][x] == '#' {
						break;
					}
				}
			}
			if heading == 2 { // Left
				loop {
					if new_x > 0 {
						new_x -= 1;
					} else {
						new_x = grid[y].len() - 1;
					}
					if grid[y][new_x] == '.' {
						x = new_x;
						println!("L to ({}, {})", x, y);
						break;
					}
					if grid[y][new_x] == '#' {
						break;
					}
				}
			}
			if heading == 3 { // Up
				loop {
					if new_y > 0 {
						new_y -= 1;
					} else {
						new_y = grid.len() - 1;
					}
					if x >= grid[new_y].len() {
						continue;
					}
					if grid[new_y][x] == '.' {
						y = new_y;
						println!("U to ({}, {})", x, y);
						break;
					}
					if grid[new_y][x] == '#' {
						break;
					}
				}
			}
		}

		heading = new_heading;

	}

	println!("Coords: (x = {}, y = {}, heading = {})", x, y, heading);

	println!("Password: {}", 1000 * (y + 1) + 4 * (x + 1) + heading);

	Ok(())
}

