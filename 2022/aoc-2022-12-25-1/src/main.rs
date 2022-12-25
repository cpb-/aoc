use std::io;
use std::io::prelude::*;
use std::io::BufReader;


fn main() -> io::Result<()> {

	let reader = BufReader::new(io::stdin());

	let mut sum: i64 = 0;

	for line in reader.lines() {
		let strline = line.unwrap();
		let digits: Vec<char> = strline.trim().chars().collect();
		
		let value: i64 = snafu_to_decimal(&digits);

		println!("{}   {}   {}", strline, value, decimal_to_snafu(value));
		sum += value;
	}

	println!("sum = {}   {} ", sum, decimal_to_snafu(sum));

	Ok(())
}

fn snafu_to_decimal(digits: &Vec<char>) -> i64 {

	let mut value: i64 = 0;
	for idx in 0 .. digits.len() {
		value *= 5;
		match digits[idx] {
			'2' => value += 2,
			'1' => value += 1,
			'0' => value += 0,
			'-' => value -= 1,
			 _  => value -= 2,
		}
	}
	return value;
}

fn decimal_to_snafu(snafu: i64) -> String {
	
	let mut value: i64 = snafu;
	let mut result: String = String::new();

	while value > 0 {
		match value % 5 {
			0 =>  { result.insert(0, '0'); value = value / 5; },
			1 =>  { result.insert(0, '1'); value = value / 5; },
			2 =>  { result.insert(0, '2'); value = value / 5; },
			3 =>  { result.insert(0, '='); value = value / 5 + 1; },
			/* 4 */ 
			_  =>  { result.insert(0, '-'); value = value / 5 + 1; },
		}
	}
	return result;
}
