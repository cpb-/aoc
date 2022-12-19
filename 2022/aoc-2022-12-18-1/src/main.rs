
use std::io;
use std::io::prelude::*;
use std::io::BufReader;


struct Coord {
	x: i64,
	y: i64,
	z: i64
}


fn main() -> io::Result<()> {

	let reader = BufReader::new(io::stdin());

	let mut cubes: Vec<Coord> = vec![];

	 for line in reader.lines() {
		let strline = line.unwrap();
		let coords : Vec<&str> = strline.split(",").collect();
		let x: i64 = coords[0].parse::<i64>().unwrap();
		let y: i64 = coords[1].parse::<i64>().unwrap();
		let z: i64 = coords[2].parse::<i64>().unwrap();
		cubes.push(Coord{x: x, y: y, z: z});
	}

	let mut surface : i64 = cubes.len() as i64 * 6;

	for i in 0..cubes.len() - 1 {
		for j in i+1..cubes.len() {
			let mut distance: i64 = (cubes[i].x - cubes[j].x).abs();
			distance += (cubes[i].y - cubes[j].y).abs();
			distance += (cubes[i].z - cubes[j].z).abs();

			if distance == 1 {
				surface -= 2;
			}
		}
	}

	println!("Surface #1 = {}", surface);

	Ok(())
}

