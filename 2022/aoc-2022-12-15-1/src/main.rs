
use std::io;
use std::io::prelude::*;
use std::io::BufReader;

struct Sensor {
	x: i32,
	y: i32,
	r: i32
}


struct Segment {
	x1: i32,
	x2: i32
}


const Y: i32 = 2000000;


fn main() -> io::Result<()> {

	let reader = BufReader::new(io::stdin());

	let mut sensors:Vec<Sensor> = vec![];

        for line in reader.lines() {
		let strline = line.unwrap();
		let substr: Vec<&str> = strline[12..].split(": closest beacon is at x=").collect();
		let scoord: Vec<&str> = substr[0].split(", ").collect();
		let sx: i32 = scoord[0].parse::<i32>().unwrap();
		let sy: i32 = scoord[1][2..].parse::<i32>().unwrap();
		let bcoord: Vec<&str> = substr[1].split(", ").collect();
		let bx: i32 = bcoord[0].parse::<i32>().unwrap();
		let by: i32 = bcoord[1][2..].parse::<i32>().unwrap();

		sensors.push(Sensor{x: sx, y: sy, r: (sx - bx).abs() + (sy - by).abs()});
        }


	let mut segments: Vec<Segment> = vec![];

	for s in sensors {
		if (s.y - Y).abs() > s.r {
			continue;
		}
		segments.push(Segment{x1 : s.x - s.r + (s.y - Y).abs(), x2: s.x + s.r - (s.y - Y).abs()});
	}

	for s in segments {
		println!("({} - {})", s.x1, s.x2);
	}

	Ok(())
}

