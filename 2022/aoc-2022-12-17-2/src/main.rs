
use std::io;
use std::io::prelude::*;
use std::io::BufReader;
//use std::collections::HashMap;


const WIDTH: usize = 7;

const HISTORY_DEPTH: usize = 20;

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

	let mut history_lines: Vec<Vec<Vec<char>>> = vec![];
	let mut history_state: Vec<(usize, usize)> = vec![]; // (jet idx, nb_rocks)


	let mut reader = BufReader::new(io::stdin());

	let mut line: String = String::new();
	reader.read_line(& mut line).expect("Data should be provided on standard input");
	let mut jets: Vec<char> = line.chars().collect();
	jets.pop();

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


		nb_rocks += 1;

/*
		if is_in_history(&lines, nb_rocks, idx, &history_lines, &history_state) {
			break;
		}

		store_in_history(&lines, nb_rocks, idx, &mut history_lines, &mut history_state);

		// The 1745-cycle starts after 232.
		// 1E12 % 1745 = 1010.
*/

		if (nb_rocks == 1010) || (nb_rocks == 2755) {
			print!("Rock #{}...", nb_rocks);
			println!(" x = {}, y = {}, top = {}", x, y, top);
			display_state(&lines, 30, top);
		}

// Then echo "scale=0; (1000000000000/1745) *  2778 + 1596" |bc -l

	}

	Ok(())
}


fn display_state(lines: &Vec<Vec<char>>, depth: usize, top: usize) {

	let mut j = top;
	while (j >= 1) && (j + depth > lines.len()) {
		for i in 0..WIDTH {
			print!("{}", lines[j - 1][i]);
		}
		println!("");
		j -= 1;
	}
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

fn is_in_history(lines: &Vec<Vec<char>>, nb_rocks: usize, idx: usize, history_lines: &Vec<Vec<Vec<char>>>, history_state: &Vec<(usize, usize)>) -> bool {

	let mut h: usize = 0;
	let mut different: bool = true;

	// println!("is_in_history({})", nb_rocks + 1);

	while h < history_lines.len() {
		if (history_state[h].0 % 5 == nb_rocks % 5) && (history_state[h].1 == idx) {
			different = false;
			let l = lines.len() - 1;
			for j in 0..HISTORY_DEPTH {
				if j > l {
					break;
				}
				for i in 0..7 {
					if history_lines[h][j][i] != lines[l - j][i] {
						different = true;
						break;
					}
				}
				if different {
					break;
				}
			}
		}
		if ! different {
			println!("Rocks #{} and #{} states are the same", history_state[h].0, nb_rocks);
			return true;
		}
		h += 1;
	}
	return false;
}


fn store_in_history(lines: &Vec<Vec<char>>, nb_rocks: usize, idx: usize, history_lines: &mut Vec<Vec<Vec<char>>>, history_state: &mut Vec<(usize, usize)>) {


	history_state.push((nb_rocks, idx));

	let l: usize = lines.len() - 1;
	let mut v: Vec<Vec<char>> = vec![];

	for j in 0..HISTORY_DEPTH {
		if (j > l) {
			break;
		}
		let mut line: Vec<char> = vec![];
		for i in 0..WIDTH {
			line.push(lines[l - j][i]);
		}
		v.push(line);
	}
	if ! v.is_empty() {
		history_lines.push(v);
	}
}

