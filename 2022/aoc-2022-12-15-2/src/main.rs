
use std::io;
use std::io::prelude::*;
use std::io::BufReader;
use std::process::exit;

struct Sensor {
	x: i64,
	y: i64,
	r: i64
}

const MAX: i64 = 4000000;
//const MAX: i64 = 20;


fn main() -> io::Result<()> {

	let reader = BufReader::new(io::stdin());

	let mut sensors:Vec<Sensor> = vec![];

        for line in reader.lines() {
		let strline = line.unwrap();
		let substr: Vec<&str> = strline[12..].split(": closest beacon is at x=").collect();
		let scoord: Vec<&str> = substr[0].split(", ").collect();
		let sx: i64 = scoord[0].parse::<i64>().unwrap();
		let sy: i64 = scoord[1][2..].parse::<i64>().unwrap();
		let bcoord: Vec<&str> = substr[1].split(", ").collect();
		let bx: i64 = bcoord[0].parse::<i64>().unwrap();
		let by: i64 = bcoord[1][2..].parse::<i64>().unwrap();

		sensors.push(Sensor{x: sx, y: sy, r: (sx - bx).abs() + (sy - by).abs()});
        }


	for s1 in sensors.iter() {
		for  y in s1.y - s1.r - 1 .. s1.y + s1.r + 1 {
			if y < 0 {
				continue;
			}
			if y > MAX {
				break;
			}
	
			let x1 = s1.x - (s1.r + 1 - (s1.y - y).abs());
			let x2 = s1.x + (s1.r + 1 - (s1.y - y).abs());
			for x in [x1, x2] {
				if (x >= 0) && (x <= MAX) {
	
					let mut i: usize = 0;
					while i < sensors.len() {
						if (sensors[i].x-x).abs() + (sensors[i].y-y).abs() <= sensors[i].r {
							break;
						}
						i += 1;
					}
					if i == sensors.len() {
						println!("({}, {}) -> {}", x, y, 4000000 * x + y);
						exit(0);
					}
				}
			}
		}
	} 

	Ok(())
}

