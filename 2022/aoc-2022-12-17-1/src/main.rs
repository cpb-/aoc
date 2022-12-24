
use std::io;
use std::io::prelude::*;
use std::io::BufReader;
//use std::collections::HashMap;


const WIDTH: usize = 7;


fn main() -> io::Result<()> {


	let mut lines: Vec<Vec<char>> = vec![];

	let rocks: Vec<Vec<Vec<char>>> = vec![
		vec![
			vec![ '#', '#', '#', '#' ]
		], 

		vec! [
			vec![ '.', '#', '.'],
			vec![ '#', '#', '#'],
			vec![ '.', '#', '.']
		],

		vec! [
			vec![ '.', '.', '#'],
			vec![ '.', '.', '#'],
			vec![ '#', '#', '#']
		],

		vec! [
			vec![ '#'],
			vec![ '#'],
			vec![ '#'],
			vec![ '#']
		],

		vec! [
			vec![ '#', '#'],
			vec![ '#', '#']
		]

	];


	let mut reader = BufReader::new(io::stdin());

	let mut line: String = String::new();
	reader.read_line(& mut line).expect("Data should be provided on standard input");
	let jets: Vec<char> = line.trim().chars().collect();

	let mut idx: usize = 0;

	let mut top: usize = 0;

	let mut nb_rocks = 0;

	loop {
		let mut x: usize;
		let mut y: usize;

		let rock = &rocks[nb_rocks % 5];

		x = 2;
		y = top + 2 + rock.len();

		// print!("start ({}, {})", x, y);

		// Add needed lines
		while lines.len() <= y {
			lines.push(vec!['.'; WIDTH]);
		}

		loop {
			// Get jet value
			let jet: char = jets[idx];

			idx += 1;
			if idx == jets.len() {
				idx = 0;
			}

			let mut new_x: usize = x;

			// Apply jet...
			if jet == '<' {
				if new_x > 0 {
					new_x -= 1;
				}
			} else if jet == '>' {
				if new_x + rock[0].len() < WIDTH {
					new_x += 1;
				}
			} else {
				println!("******** UNKNOW MOVE ***************");
				std::process::exit(1);
			}
			// print!("{}...", jet);
			// ... except on collision.
			if ! collision(&lines, &rock, new_x, y) {
				// println!(" x = {}", new_x);
				x = new_x;
			} else {
				// println!(" collision");
			}

			// Fall down one unit except on collision.
			if y < 1 {
				break;
			}
			if collision(&lines, &rock, x, y - 1) {
				break;
			}
			y -= 1;
			// println!("Rock #{} ({},{})", nb_rocks +1, x, y);
		}


		// Store rock position
		for j in 0 .. rock.len() {
			for i in 0 .. rock[j].len() {
				if rock[j][i] == '#' {
					match nb_rocks % 5 {
						0 => lines[y - j][x + i] = 'A',
						1 => lines[y - j][x + i] = 'B',
						2 => lines[y - j][x + i] = 'C',
						3 => lines[y - j][x + i] = 'D',
						_ => lines[y - j][x + i] = 'E',
					}
				}
			}
		}

		if y + 1 > top {
			top = y + 1;
		}		


		print!("Rock #{}...", nb_rocks + 1);
		println!(" x = {}, y = {}, top = {}", x, y, top);
		nb_rocks += 1;
		if nb_rocks == 2022 {
			break;
		}
	}
/*
	let mut j = lines.len();
	while j > 0 {
		for i in 0..WIDTH {
			print!("{}", lines[j - 1][i]);
		}
		println!("");
		j -= 1;
	}
*/
	Ok(())
}


fn collision(lines: &Vec<Vec<char>>, rock: &Vec<Vec<char>>, x: usize, y: usize) -> bool {

	// println!("Test collision for rock w: {} h: {}, x: {}, y: {}", rock[0].len(), rock.len(), x, y);

	if y + 1 < rock.len() {
		// print!("rest at the bottom y={}", y);
		return true;
	}

	for j in 0..rock.len() {
		for i in 0..rock[j].len() {
			if rock[j][i] == '#' && lines[y - j][x + i] != '.' {
				return true;
			}
		}
	}
	return false;
}
