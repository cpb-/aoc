
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
			score += 0;
			match him {
				'A' => score += 3,
				'B' => score += 1,
				_ /*'C'*/ => score += 2,
			};
		},
		'Y' => {
			score += 3;
			match him {
				'A' => score += 1,
				'B' => score += 2,
				_ /*'C'*/ => score += 3,
			};
		},
		_ /* 'Z' */=> {
			score += 6;
			match him {
				'A' => score += 2,
				'B' => score += 3,
				_ /*'C'*/ => score += 1,
			};

		},
	}
	return score;
}
