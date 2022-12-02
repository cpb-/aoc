*
use std::io;
use std::io::prelude::*;
use std::io::BufReader;



fn main() -> io::Result<()> {

	let reader = BufReader::new(io::stdin());
	let mut sum: u32 = 0;

	for line in reader.lines() {

		let content = line.unwrap();
		let mut chars = content.chars();

		let him = chars.next().unwrap();
		chars.next();
		let me = chars.next().unwrap();

		sum += compute_round(him, me);
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
