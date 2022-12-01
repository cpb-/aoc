
use std::io;
use std::io::prelude::*;
use std::io::BufReader;


fn main() -> io::Result<()> {

	let mut reader = BufReader::new(io::stdin());
	let mut value: u32;
	let mut sum: u32;
	let mut max: u32;

	max = 0;
	sum = 0;
	loop {

		let mut line = String::new();
		let nb_bytes = reader.read_line(& mut line).expect("Error");

		if line.trim().is_empty() || (nb_bytes == 0) {
			if max < sum {
				max = sum;
			}
			sum = 0;
		} else {
			value = line.trim().parse::<u32>().expect("Unparsable");
			sum += value;
		}
		if nb_bytes == 0 {
			break;
		}
	}

	println!("Max = {}", max);

	Ok(())
}
