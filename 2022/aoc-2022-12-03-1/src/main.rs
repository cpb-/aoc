
use std::io;
use std::io::prelude::*;
use std::io::BufReader;



fn main() -> io::Result<()> {

	let reader = BufReader::new(io::stdin());
	let mut sum: u32 = 0;

	for line in reader.lines() {

		sum += find_priority_of_double_items(line.unwrap())

	}

	println!("Priority sum = {}", sum);

	Ok(())
}


fn find_priority_of_double_items(line: String) -> u32 {

	let half_len = line.len() / 2;

	let chars : Vec<_> = line.chars().collect();

	let mut i = 0;
	while i < half_len {

		let mut j = half_len;
		while j < line.len() {
			if chars[i] == chars[j] {
				break;
			}		
			j += 1;
		}
		if j < line.len() {
			break;
		}	
		i += 1;
	}

	return compute_priority(chars[i]);
}


fn compute_priority(item: char) -> u32 {

	let mut priority = 0;

	if item.is_ascii_lowercase() {
		priority = item as u32 - 'a' as u32 + 1;
	}

	if item.is_ascii_uppercase() {
		priority = item as u32 - 'A' as u32 + 27;
	}

	return priority;
}
