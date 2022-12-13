
use std::io;
use std::io::prelude::*;
use std::io::BufReader;
use std::cmp::Ordering;

fn main() -> io::Result<()> {

	let mut reader = BufReader::new(io::stdin());
	let mut lines: Vec<Vec<char>> = vec![];

	loop {

        let mut line1 = String::new();
		let nb = reader.read_line(& mut line1).expect("Data should be provided on standard input");
		if nb == 0 {
			break;
		}
        let mut line2 = String::new();
		let nb = reader.read_line(& mut line2).expect("Data should be provided on standard input");
		if nb == 0 {
			break;
		}

        let mut line3 = String::new();
		let nb = reader.read_line(& mut line3).expect("Data should be provided on standard input");
		assert!(line3.trim().is_empty());

		lines.push(line1.trim().chars().collect());
		lines.push(line2.trim().chars().collect());

		if nb == 0 {
			break;
		}
	}

	lines.push("[[2]]".to_string().chars().collect());
	lines.push("[[6]]".to_string().chars().collect());

	lines.sort_by(|a, b| compare_lines((&a).to_vec(), (&b).to_vec()));

	let mut idx: usize = 0;
	while idx < lines.len() {
			if same_lines(&(lines[idx]), "[[2]]".to_string().chars().collect()) {
				println!(" Index 2 = {}", idx + 1);
			}
			if same_lines(&(lines[idx]), "[[6]]".to_string().chars().collect()) {
				println!(" Index 6 = {}", idx + 1);
			}
			idx += 1;
	}

	Ok(())
}


fn same_lines(line1: &Vec<char>, line2: Vec<char>) -> bool {
	
	let mut i = 0;
	if line1.len() != line2.len() {
			return false;
	}
	while i < line1.len() {
		if line2[i] != line1[i] {
				return false;
		}
		i +=1;
	}
	return true;
}


fn compare_lines(mut line1: Vec<char>, mut line2: Vec<char>) -> Ordering {

	loop {
		if line1.len() == 0 {
			if line2.len() == 0 {
				return Ordering::Equal;
			}
			return Ordering::Less;
		}
		if line2.len() == 0 {
			return Ordering::Greater;
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
				return Ordering::Less;
			}
			if num2 < num1 {
				return Ordering::Greater;
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
			return Ordering::Less;
		}
		if (line1[0] != ']') && (line2[0] == ']') {
			return Ordering::Greater;
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
}
