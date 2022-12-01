
use std::io;
use std::io::prelude::*;
use std::io::BufReader;


fn main() -> io::Result<()> {

	let mut reader = BufReader::new(io::stdin());
	let mut value: u32;
	let mut sum: u32 = 0;
	let mut max: [u32; 3] = [0;3];

	loop {

		let mut line = String::new();
		let nb_bytes = reader.read_line(& mut line).expect("Data should be provided on standard input");

		if line.trim().is_empty() {
			if max[0] < sum {
				max[2] = max[1];
				max[1] = max[0];
				max[0] = sum;
			} else if max[1] < sum {
				max[2] = max[1];
				max[1] = sum;
			} else if max[2] < sum {
				max[2] = sum;
			}
			sum = 0;
		} else {
			value = line.trim().parse::<u32>().expect("Lines should contains a u32");
			sum += value;
		}
		if nb_bytes == 0 {
			break;
		}
	}

	println!("Maximum calories for three Elves = {}", max[0] + max[1] + max[2]);

	Ok(())
}
