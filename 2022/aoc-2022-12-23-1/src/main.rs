use std::io;
use std::io::prelude::*;
use std::io::BufReader;
//use std::collections::HashMap;

const NB_ROUNDS: usize = 10;

fn main() -> io::Result<()> {

	let mut reader = BufReader::new(io::stdin());

	let mut elves:  Vec<(i32, i32)> = vec![];

	let mut row: i32 = 0;

	loop {
		let mut line: String = String::new();
		reader.read_line(&mut line).expect("Data should be provided on standard input.");
		if line.trim().is_empty() {
			break;
		}
		let mut ch: Vec<char> = line.trim().chars().collect();
		for col in 0..ch.len() {
			if ch[col] == '#' {
				elves.push((col as i32, row));
			}
		}

		row += 1;
	}

	let nb_elves: i32 = elves.len() as i32;

	println!("Initial");
	for a in 0..elves.len() {
		print!("({}, {})", elves[a].0, elves[a].1);
	}
	println!("");

	let mut directions: Vec<usize> = vec![0, 1, 2, 3];

	for round in 0..NB_ROUNDS {

		let mut proposal: Vec<(i32, i32)> = vec![];
		for a in 0..elves.len() {

			let mut b: usize;

			let mut neighbour: bool = false;
			b=0;
			while b < elves.len() {
				if b != a {
					if ((elves[b].0 == elves[a].0 - 1) && ((elves[b].1 == elves[a].1) || (elves[b].1 == elves[a].1 + 1) || (elves[b].1 == elves[a].1 - 1))
					 || (elves[b].0 == elves[a].0 + 1) && ((elves[b].1 == elves[a].1) || (elves[b].1 == elves[a].1 + 1) || (elves[b].1 == elves[a].1 - 1))
					 || (elves[b].0 == elves[a].0) && ((elves[b].1 == elves[a].1 + 1) || (elves[b].1 == elves[a].1 - 1))) {
						neighbour = true;
						 break;
					}
				}
				b += 1;
			}
			if ! neighbour {
				proposal.push((elves[a].0, elves[a].1));
				continue;
			}


			let mut proposition_done: bool = false;

			for dir in 0..4 {
				if directions[dir] == 0 {
					// Try North
					b  = 0;
					while b < elves.len() {
						if (elves[b].1 == elves[a].1 - 1) && ((elves[b].0 == elves[a].0 - 1) || (elves[b].0 == elves[a].0) || (elves[b].0 == elves[a].0 + 1)) {
							break;
						}
						b += 1;
					}
					if b == elves.len() {
						proposal.push((elves[a].0, elves[a].1 - 1));
						proposition_done = true;
					}
				} else if directions[dir] == 1 {
					// Try South
					b  = 0;
					while b < elves.len() {
						if (elves[b].1 == elves[a].1 + 1) && ((elves[b].0 == elves[a].0 - 1) || (elves[b].0 == elves[a].0) || (elves[b].0 == elves[a].0 + 1)) {
							break;
						}
						b += 1;
					}
					if b == elves.len() {
						proposal.push((elves[a].0, elves[a].1 + 1));
						proposition_done = true;
					}
				} else if directions[dir] == 2 {
					// Try West
					b  = 0;
					while b < elves.len() {
						if (elves[b].0 == elves[a].0 - 1) && ((elves[b].1 == elves[a].1 - 1) || (elves[b].1 == elves[a].1) || (elves[b].1 == elves[a].1 + 1)) {
							break;
						}
						b += 1;
					}
					if b == elves.len() {
						proposal.push((elves[a].0 - 1, elves[a].1));
						proposition_done = true;
					}
				} else if directions[dir] == 3 {
					// Try East
					b  = 0;
					while b < elves.len() {
						if (elves[b].0 == elves[a].0 + 1) && ((elves[b].1 == elves[a].1 - 1) || (elves[b].1 == elves[a].1) || (elves[b].1 == elves[a].1 + 1)) {
							break;
						}
						b += 1;
					}
					if b == elves.len() {
						proposal.push((elves[a].0 + 1, elves[a].1));
						proposition_done = true;
					}
				}
				if proposition_done { break; }
			}
			if ! proposition_done {
				// Can't move.
				proposal.push((elves[a].0, elves[a].1));
			}
		}
	
		assert_eq!(elves.len(), proposal.len());

		for a in 0..proposal.len() {
			let mut collision: bool = false;
			for b in a + 1..proposal.len() {
				if (proposal[a].0 == proposal[b].0) && (proposal[a].1 == proposal[b].1) {
					proposal[b] = elves[b];
					collision = true;
				}
			}
			if collision {
				proposal[a] = elves[a];
			}
			elves[a] = proposal[a];
		}

		println!("Round #{}", round + 1);
		for a in 0..elves.len() {
			print!("({}, {})", elves[a].0, elves[a].1);
		}
		println!("");

		let dir = directions.remove(0);
		directions.push(dir);

	}

	let mut min: (i32, i32) = elves[0];
	let mut max: (i32, i32) = elves[0];

	for elf in elves {
		if elf.0 < min.0 { min.0 = elf.0; }
		if elf.1 < min.1 { min.1 = elf.1; }
		if elf.0 > max.0 { max.0 = elf.0; }
		if elf.1 > max.1 { max.1 = elf.1; }
	}



	println!("{} * {} - {} = {}", max.0 - min.0 + 1, max.1 - min.1 + 1, nb_elves, (max.0 - min.0 + 1) * (max.1 - min.1 + 1) - nb_elves);

	Ok(())
}


