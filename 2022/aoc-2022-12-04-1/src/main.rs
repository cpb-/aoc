
use std::io;
use std::io::prelude::*;
use std::io::BufReader;



fn main() -> io::Result<()> {

	let reader = BufReader::new(io::stdin());
	let mut sum: u32 = 0;

	for line in reader.lines() {

		sum += count_fully_contained(line.unwrap())

	}

	println!("Number of fully contained = {}", sum);

	Ok(())
}


fn count_fully_contained(line: String) -> u32 {

	let pair: Vec<&str> = line.split(',').collect();

	let mut valstr:Vec<&str> = pair[0].split('-').collect();

	let a: u32 = valstr[0].parse().unwrap();
	let b: u32 = valstr[1].parse().unwrap();

	valstr = pair[1].split('-').collect();

	let c: u32 = valstr[0].parse().unwrap();
	let d: u32 = valstr[1].parse().unwrap();
	
	if (a <= c && b >= d) || (c <= a && d >= b) {
		return 1;
	}

	return 0;
}

