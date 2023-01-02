// SPDX-License-Identifier: GPL-2.0-only

use std::io;
use std::io::prelude::*;
use std::io::BufReader;


fn main() -> io::Result<()> {

	let reader = BufReader::new(io::stdin());
	let mut sum: u32 = 0;

	for line in reader.lines() {

		let content: Vec<char> = line.unwrap().chars().collect();

		sum += compute_round(content[0], content[2]);
	}

	println!("My score = {}", sum);

	Ok(())
}


fn compute_round(him: char, me: char) -> u32 {

	let mut score: u32 = 0;
	
	match me {
		'X' => {
			score += 1;
			match him {
				'A' => score += 3,
				'B' => score += 0,
				_ /*'C'*/ => score += 6,
			};
		},
		'Y' => {
			score += 2;
			match him {
				'A' => score += 6,
				'B' => score += 3,
				_ /*'C'*/ => score += 0,
			};
		},
		_ /* 'Z' */=> {
			score += 3;
			match him {
				'A' => score += 0,
				'B' => score += 6,
				_ /*'C'*/ => score += 3,
			};

		},
	}
	return score;
}
