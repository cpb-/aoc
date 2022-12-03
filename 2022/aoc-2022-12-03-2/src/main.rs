
use std::io;
use std::io::prelude::*;
use std::io::BufReader;



fn main() -> io::Result<()> {

	let mut reader = BufReader::new(io::stdin());
	let mut sum: u32 = 0;
	
	loop {
		let mut line_1 = String::new();
		let mut line_2 = String::new();
		let mut line_3 = String::new();

		reader.read_line(& mut line_1).expect("Data should be provided on standard input");
		reader.read_line(& mut line_2).expect("Data should be provided on standard input");
		let nb = reader.read_line(& mut line_3).expect("Data should be provided on standard input");

		sum += find_badge_priority(line_1, line_2, line_3);

		if nb == 0 {
			break;
		}
	}

	println!("Priority sum = {}", sum);

	Ok(())
}


fn find_badge_priority(mut line_1: String, line_2: String, line_3: String) -> u32 {

	loop {
		let res1 = line_1.pop();
		if res1.is_none() {
			break;
		}
		let c1 : char = res1.unwrap();
		if !c1.is_alphabetic() {
			continue;
		}
		if line_2.find(c1).is_none() {
			continue;
		}
		if line_3.find(c1).is_none() {
			continue;
		}
		return compute_priority(c1);
	}
	return 0;
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
