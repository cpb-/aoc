
use std::io;
use std::io::prelude::*;
use std::io::BufReader;


fn main() -> io::Result<()> {

	let mut reader = BufReader::new(io::stdin());

	let mut index: usize = 0;
	let mut sum: usize = 0;

	loop {
	        let mut line1 = String::new();
	        let mut line2 = String::new();
	        let mut line3 = String::new();

		let nb = reader.read_line(& mut line1).expect("Data should be provided on standard input");
		if nb == 0 {
			break;
		}
		let nb = reader.read_line(& mut line2).expect("Data should be provided on standard input");
		if nb == 0 {
			break;
		}
		let nb = reader.read_line(& mut line3).expect("Data should be provided on standard input");
		if nb == 0 {
			break;
		}
		assert!(line3.trim().is_empty());
		index +=1 ;

		sum += compare_lines(line1.chars().collect(), line2.chars().collect(), index);

	}

	println!("Nb pairs: {}", index);
	println!("Sum of indexes: {}", sum);

	Ok(())
}

fn compare_lines(mut line1: Vec<char>, mut line2: Vec<char>, index: usize) -> usize {

	loop {
		if line1.len() == 0 {
			// println!("End of line 1 -> right order");
			return index;
		}
		if line2.len() == 0 {
			// println!("End of line 2 (and not 1) -> wrong order");
			return 0;
		}
		if line1[0].is_digit(10) && line2[0].is_digit(10) {
			let mut num1 = 0;
			while line1[0].is_digit(10) {
				num1 *= 10;
				num1 += line1.remove(0).to_digit(10).unwrap();
			}
			let mut num2 = 0;
			while line2[0].is_digit(10) {
				num2 *= 10;
				num2 += line2.remove(0).to_digit(10).unwrap();
			}
			if num1 < num2 {
				// println!("{} < {} -> right order", num1, num2);
				return index;
			}
			if num2 < num1 {
				// println!("{} < {} -> wrong order", num2, num1);
				return 0;
			}
			if line1[0] == ',' {
				line1.remove(0);
			}
			if line2[0] == ',' {
				line2.remove(0);
			}
			continue;
		}
		if (line1[0] == ']') && (line2[0] != ']') {
			return index;
		}
		if (line1[0] != ']') && (line2[0] == ']') {
			return 0;
		}
		if ((line1[0] == ']') && (line2[0] == ']')) || 
		   ((line1[0] == '[') && (line2[0] == '[')) ||
		   ((line1[0] == ',') && (line2[0] == ',')) {
			line1.remove(0);
			line2.remove(0);
			continue;
		}
		if (line1[0] == '[') && (line2[0].is_digit(10)) {
			line2.insert(0, '[');
			let mut i: usize = 1;
			while line2[i].is_digit(10) {
				i += 1;
			}
			line2.insert(i, ']');
			continue;
		}
		if (line2[0] == '[') && (line1[0].is_digit(10)) {
			line1.insert(0, '[');
			let mut i: usize = 1;
			while line1[i].is_digit(10) {
				i += 1;
			}
			line1.insert(i, ']');
			continue;
		}
		println!("Not handled: {}, {}", line1[0], line2[0]);
	}
//	return 0;
}
