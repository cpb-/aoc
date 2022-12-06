
use std::io;
use std::io::prelude::*;
use std::io::BufReader;

const NB: usize = 4;

fn main() -> io::Result<()> {

	let mut reader = BufReader::new(io::stdin());

	let mut strline = String::new();
	reader.read_line(& mut strline).expect("Data should be provided on standard input");

	let mut nb_chars: usize = 0;
	let mut cs: [char; NB] = [' '; NB];

	for c in strline.trim().chars() {

		nb_chars += 1;

		// insert c in cs array.
		let mut i: usize = 0;
		while i < (NB - 1) {
			cs[i] = cs [i+1];
			i += 1;
		}
		cs[NB - 1] = c;

		if nb_chars < NB {
			continue;
		}

		let mut found: bool = false;
		i = 0;
		while i < (NB - 1) {
			let mut j: usize = i + 1;
			while j < NB {
				if cs[i] == cs [j] {
					found = true;
					break;
				}
				j += 1;
			}	
			if found {
				break;
			}
			i += 1;
		}
		if found {
			continue;
		}
		println!("{}", nb_chars);
		break;
	}
	Ok(())
}
