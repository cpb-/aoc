
use std::io;
use std::io::prelude::*;
use std::io::BufReader;

fn main() -> io::Result<()> {

	let mut reader = BufReader::new(io::stdin());

	let mut nb_stacks: u32 = 0;
	let mut stacks : Vec<String> = Vec::new();
	let mut i: u32;
	let mut nb_lines : i32 = 0;

	loop {
		let mut strline = String::new();
		reader.read_line(& mut strline).expect("Data should be provided on standard input");
		nb_lines += 1;

		if strline.trim().is_empty() { // empty line between header and orders.
			break;
		}

		if nb_stacks == 0 {
			// Initialize stacks.
			nb_stacks = (strline.len() as u32 + 1) / 4;
			i = 0;
			while i < nb_stacks {
				stacks.push("".to_string());
				i += 1;
			}
		}
		i = 0;
		while i < nb_stacks {
			// Fill stacks content.
			let sub : String = strline.chars().skip(i as usize* 4).take(4).collect();
			let mut iter = sub.chars();
			let char = iter.next();
			if !char.is_none() {
				if char.unwrap() == '[' {
					stacks[i as usize].insert(0, iter.next().unwrap());
				}
			}
			i += 1;
		}
	}

	loop {
		let mut strline = String::new();
		reader.read_line(& mut strline).expect("Data should be provided on standard input");
		nb_lines += 1;
		if strline.is_empty() { // end of file.
			break;
		}

		let words: Vec<&str> = strline.split(' ').collect();
		if words[0].eq("move") {
			println!("{}->{}->{}", words[1], words[3], words[5]);

			let nb_moves: u32 = words[1].parse::<u32>().expect("First field should be number of moves");
			let start: usize = words[3].parse::<usize>().expect("Second field should be starting stack");
			let end: usize = words[5].trim().parse::<usize>().expect("Third field should be ending stack");
			let mut mv: u32 = 0;
			while mv < nb_moves {
				let item = stacks[start - 1].pop();
				if item.is_some() {
					stacks[end -1].push(item.unwrap());
				}
				mv += 1;
			}
		}
	}

	i = 0;
	while i < nb_stacks {
		println!("{}", stacks[i as usize]);
		i += 1;
	}
	
	println!("Nb lines = {}", nb_lines);

	Ok(())
}


