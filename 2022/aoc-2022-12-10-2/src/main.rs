
use std::io;
use std::io::prelude::*;
use std::io::BufReader;


fn main() -> io::Result<()> {

	let reader = BufReader::new(io::stdin());

	let mut cycles:i32 = 0;
	let mut x:i32 = 1;

	let mut sum: i32 = 0;

	for line in reader.lines() {
		let strline = line.unwrap();

		if strline.starts_with("noop") {
			sum += inc_cycles(&mut cycles, x);
			continue;
		}

		if strline.starts_with("addx ") {
			let value: i32 = strline[5..].parse::<i32>().unwrap();
			sum += inc_cycles(&mut cycles, x);
			sum += inc_cycles(&mut cycles, x);
			x += value;
		}
	}

	println!("Sum = {}", sum);

	Ok(())
}


fn inc_cycles(cycles: &mut i32, x: i32) -> i32 {
	*cycles += 1;
	if ((*cycles % 40) >= x) && ((*cycles % 40) < x + 3) {
		print!("#");
	} else {
		print!(".");
	}

	if (*cycles % 40) == 0 {
		println!("");
	}

	if (*cycles % 40) == 20 {
		return *cycles * x;
	}
	return 0;
}



