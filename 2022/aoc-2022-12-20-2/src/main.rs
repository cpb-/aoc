use std::io;
use std::io::prelude::*;
use std::io::BufReader;


fn main() -> io::Result<()> {

	let reader = BufReader::new(io::stdin());

	let mut numbers: Vec<(i64, usize)> = vec![];

	let mut i: usize = 0;
	 for line in reader.lines() {
		
		let strline = line.unwrap();
		let num = strline.parse::<i64>().unwrap();
		numbers.push((num * 811589153, i));
		i += 1;
	}

	for _ in 1..=10 {
		mix_file(&mut numbers);
	}

	let l = numbers.len();
	let mut idx = 0;
	while idx < l {
		if numbers[idx].0 == 0 {
			break;
		}
		idx += 1;
	}
	let idx_1000 = (idx + 1000) % l;
	let idx_2000 = (idx + 2000) % l;
	let idx_3000 = (idx + 3000) % l;
	let sum = numbers[idx_1000].0  + numbers[idx_2000].0 + numbers[idx_3000].0;

	println!(" Sum: {}", sum);

	Ok(())
}


fn mix_file(numbers: &mut Vec<(i64, usize)>)
{
	let l = numbers.len();

	for idx in 0..l {

		let mut i: usize = 0;
		while i < l {
			if numbers[i].1 == idx {
				break;
			}
			i += 1;
		}
		assert_ne!(i, l);

		let (value, orig) = numbers[i];

		let mut n = numbers[i].0 % (l -1) as i64;
		numbers.remove(i);

		while n < 0 {
			n += l as i64 - 1;
		}
		i += n as usize;
		while i >= l {
			i -= l - 1;
		}
		numbers.insert(i, (value, orig));

	}

//	for i in 0..l { print!("{}, ", numbers[i].0); }
//	println!("");
}
