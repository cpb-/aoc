use std::io;
use std::io::prelude::*;
use std::io::BufReader;


const SIZE: usize = 50;

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

	while grid[y][x] == ' ' { x +=1; }
	
	let mut face: usize = compute_face(x, y);
	x = x % 50;
	y = y % 50;

	println!("Starting at face {} ({}, {})", face, x, y);

	while ! path.trim().is_empty() {

		let mut ch: char = path.remove(0);

		let mut nb_steps: u32 = 0;
		while ch.is_digit(10) {
			nb_steps *= 10;
			nb_steps += ch as u32 - 48; // '0'
			ch = path.remove(0);
		}

		let mut turn = ch;

		println!("Read {} {}", nb_steps, ch);

		let mut new_x: usize;
		let mut new_y: usize;
		let mut new_face: usize;

		for _ in 0..nb_steps {

			new_x = x;
			new_y = y;
			new_face = face;

			if heading == 0 { // Right
				if new_x % SIZE != SIZE - 1 {
					new_x += 1;
				} else {
					match face {
						1 => { new_face = 2; new_x = 0 },
						2 => { new_face = 5; new_x = SIZE - 1; new_y = SIZE - 1 - y; heading = 2 },
						3 => { new_face = 2; new_y = SIZE - 1; new_x = y; heading = 3 },
						4 => { new_face = 5; new_x = 0 },
						5 => { new_face = 2; new_x = SIZE - 1; new_y = SIZE - 1 - y; heading = 2 },
						_ => { new_face = 5; new_x = y; new_y = SIZE - 1; heading = 3 },
					}
				}

			} else 	if heading == 2 { // Left
				if new_x % SIZE != 0 {
					new_x -= 1;
				} else {
					match face {
						1 => { new_face = 4; new_x = 0; new_y = SIZE - 1 - y; heading = 0 },
						2 => { new_face = 1; new_x = SIZE - 1 },
						3 => { new_face = 4; new_y = 0; new_x = y; heading = 1 },
						4 => { new_face = 1; new_x = 0; new_y = SIZE - 1 - y; heading = 0 },
						5 => { new_face = 4; new_x = SIZE - 1 },
						_ => { new_face = 1; new_x = y; new_y = 0; heading = 1 },
					}
				}

			} else if heading == 1 { // Down
				if new_y % SIZE != SIZE - 1 {
					new_y += 1;
				} else {
					match face {
						1 => { new_face = 3; new_y = 0 },
						2 => { new_face = 3; new_x = SIZE - 1; new_y = x; heading = 2 },
						3 => { new_face = 5; new_y = 0 },
						4 => { new_face = 6; new_y = 0 },
						5 => { new_face = 6; new_x = SIZE - 1 ; new_y = x; heading = 2 },
						_ => { new_face = 2; new_y = 0 },
					}
				}

			} else if heading == 3 { // Up
				if new_y % SIZE != 0 {
					new_y -= 1;
				} else {
					match face {
						1 => { new_face = 6; new_x = 0; new_y = x; heading = 0 },
						2 => { new_face = 6; new_y = SIZE - 1 },
						3 => { new_face = 1; new_y = SIZE - 1 },
						4 => { new_face = 3; new_y = x; new_x = 0; heading = 0 },
						5 => { new_face = 3; new_y = SIZE - 1 },
						_ => { new_face = 4; new_y = SIZE - 1 },
					}
				}
			}


			let ch: char = get_content(&grid, new_x, new_y, new_face);

			if ch == '.' {
				x = new_x;
				y = new_y;
				face = new_face;
				println!("to face {} ({}, {}), heading {}", face, x, y, heading);
				continue;
			}
			if ch == '#' {
				println!("Wall at face {} ({}, {})", face, new_x, new_y);
				continue;
			}
			println!("Unexpected char {} at ({}, {})", ch, new_x, new_y);
		}

		match turn {
			'R' => heading = (heading + 1) % 4,
			'L' => heading = (heading + 3) % 4,
			 _  => println!("End of data"),
		};

		println!("New heading {}", heading)

	}

	println!("Ending at face {} (x = {}, y = {}, heading = {})", face, x, y, heading);

	println!("Password: {}", compute_password(face, x + 1, y + 1, heading));

	Ok(())
}


fn get_content(grid: &Vec<Vec<char>>, x: usize, y: usize, face: usize) -> char {
	
	if SIZE == 50 { // full input

		match face {
			1 => return grid[y][x + SIZE],
			2 => return grid[y][x + 2 * SIZE],
			3 => return grid[y + SIZE][x + SIZE],
			4 => return grid[y + 2 * SIZE][x],
			5 => return grid[y + 2 * SIZE][x + SIZE],
			_ => return grid[y + 3 * SIZE][x],
		}
	}
	return ' ';
}


fn compute_face(x: usize, y: usize) -> usize {
	if SIZE == 50 { // full input

		if y < SIZE {
			if x < 2 * SIZE { return 1; }
			return 2;
		}
		if y < 2 * SIZE { return 3; }
		if y < 3 * SIZE {
			if x < SIZE { return 4; }
			return 5;
		}
		return 6;
	}
	return 0;
}

fn compute_password(face: usize, x: usize, y: usize, heading: usize) -> usize {
	
	if SIZE == 50 { // full input

		match face {
			1 => return 1000 * y + 4 * (x + SIZE) + heading,
			2 => return 1000 * y + 4 * (x + 2 * SIZE) + heading,
			3 => return 1000 * (y + SIZE) + 4 * (x + SIZE) + heading,
			4 => return 1000 * (y + 2 * SIZE) + 4 * x + heading,
			5 => return 1000 * (y + 2 * SIZE) + 4 * (x + SIZE) + heading,
			_ => return 1000 * (y + 3 * SIZE) + 4 * x  + heading,
		}
	}
	return 0;
}


