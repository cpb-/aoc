use std::io;
use std::io::prelude::*;
use std::io::BufReader;
use std::collections::HashMap;


fn main() -> io::Result<()> {

	let reader = BufReader::new(io::stdin());

	let mut known_values: HashMap<String, i64> = HashMap::new();
	let mut equations: Vec<(String, String, String, String)> = vec![];

	for line in reader.lines() {
		
		let strline = line.unwrap();
		let words: Vec<&str> = strline.split(" ").collect();

		let mut name = words[0].to_string();
		name.pop();

		if words.len() == 2 {
			let mut num = words[1].parse::<i64>().unwrap();
			if name == "humn" {
				num = 3721298272959; // guessed by successive approximations.
			}
			known_values.entry(name).or_insert(num);
		} else {
			equations.push((name, words[1].to_string(), words[2].to_string(), words[3].to_string()));
		}

	}

	loop {
		if equations.len() == 0 { break; }

		let eq = equations.remove(0);

		let op1 = known_values.get(&eq.1);
		let val1: i64;

		match op1 {
			None => {  equations.push(eq);continue; }
			_ => val1 = *op1.unwrap(),
		}

		let op2 = known_values.get(&eq.3[0..4]);
		let val2: i64;
		match op2 {
			None => {  equations.push(eq); continue; }
			_ => val2 = *op2.unwrap(),
		}

		if eq.0 == "root" {
			println!("root: {}", val1);
			println!("  vs. {}", val2);
			break;
		}

		if eq.2 == "+" {
//			println!("New value: {} {}", eq.0, val1 + val2);
			known_values.entry(eq.0).or_insert(val1 + val2);
		} else if eq.2 == "-" {
//			println!("New value: {} {}", eq.0, val1 - val2);
			known_values.entry(eq.0).or_insert(val1 - val2);
		} else if eq.2 == "*" {
//			println!("New value: {} {}", eq.0, val1 * val2);
			known_values.entry(eq.0).or_insert(val1 * val2);
		} else if eq.2 == "/" {
//			println!("New value: {} {}", eq.0, val1 / val2);
			known_values.entry(eq.0).or_insert(val1 / val2);
		} else {
			println!("Unknown operator: {}", eq.2);
		}
	}

/*	for value in known_values {
		print!("{} -> {}", value.0, value.1);
		if value.0 == "root" {	print!(" <-------"); }
		println!("");
	}
*/

	Ok(())
}

