
use std::io;
use std::io::prelude::*;
use std::io::BufReader;


struct Coord {
	x: usize,
	y: usize,
	z: usize
}

#[derive(PartialEq)]
enum Matter {
	Air,
	Rock,
	Water
}


fn main() -> io::Result<()> {

	let reader = BufReader::new(io::stdin());

	let mut cubes: Vec<Coord> = vec![];

	// coords are all small positives
	let mut xmax: usize = 0;
	let mut ymax: usize = 0;
	let mut zmax: usize = 0;

	let mut grid: Vec<Vec<Vec<Matter>>> = vec![];

	 for line in reader.lines() {
		let strline = line.unwrap();
		let coords : Vec<&str> = strline.split(",").collect();
		let x: usize = coords[0].parse::<usize>().unwrap();
		let y: usize = coords[1].parse::<usize>().unwrap();
		let z: usize = coords[2].parse::<usize>().unwrap();
		cubes.push(Coord{x: x, y: y, z: z});

		if x > xmax { xmax = x; }
		if y > ymax { ymax = y; }
		if z > zmax { zmax = z; }

	}
	
	for i in 0..=xmax + 1 { 
		grid.push(vec![]);
		for j in 0..=ymax + 1 {
			grid[i].push(vec![]);
			for _ in 0..=zmax + 1 {
				grid[i][j].push(Matter::Air);
			}
		}
	}

	for i in 0..cubes.len() {
		grid[cubes[i].x][cubes[i].y][cubes[i].z] = Matter::Rock;
	}


	let mut surface : i64 = cubes.len() as i64 * 6;

	for i in 0..cubes.len() - 1 {
		for j in i+1..cubes.len() {
			let mut distance: i64 = (cubes[i].x as i64 - cubes[j].x as i64).abs();
			distance += (cubes[i].y as i64 - cubes[j].y as i64).abs();
			distance += (cubes[i].z as i64 - cubes[j].z as i64).abs();

			if distance == 1 {
				surface -= 2;
			}
		}
	}

	println!("Surface #1 = {}", surface);

	// Breadth First Walk when flooding water into grid.

	let mut flood: Vec<Coord> = vec![];

	grid[xmax + 1][ymax + 1][zmax + 1] = Matter::Water;
	flood.push(Coord{x: xmax + 1, y: ymax + 1, z: zmax + 1});

	while flood.len() > 0 {
		let pos = flood.remove(0);
		// println!("Water in ({}, {}, {})", pos.x, pos.y, pos.z);

		if (pos.x > 0) && (grid[pos.x - 1][pos.y][pos.z] == Matter::Air) {
			grid[pos.x - 1][pos.y][pos.z] = Matter::Water;
			flood.push(Coord{x: pos.x - 1, y: pos.y, z: pos.z});
		}
		if (pos.x < xmax + 1) && (grid[pos.x + 1][pos.y][pos.z] == Matter::Air) {
			grid[pos.x + 1][pos.y][pos.z] = Matter::Water;
			flood.push(Coord{x: pos.x + 1, y: pos.y, z: pos.z});
		}
		if (pos.y > 0) && (grid[pos.x][pos.y - 1][pos.z] == Matter::Air) {
			grid[pos.x][pos.y - 1][pos.z] = Matter::Water;
			flood.push(Coord{x: pos.x, y: pos.y - 1, z: pos.z});
		}
		if (pos.y < ymax + 1) && (grid[pos.x][pos.y + 1][pos.z] == Matter::Air) {
			grid[pos.x][pos.y + 1][pos.z] = Matter::Water;
			flood.push(Coord{x: pos.x, y: pos.y + 1, z: pos.z});
		}
		if (pos.z > 0) && (grid[pos.x][pos.y][pos.z - 1] == Matter::Air) {
			grid[pos.x][pos.y][pos.z - 1] = Matter::Water;
			flood.push(Coord{x: pos.x, y: pos.y, z: pos.z - 1});
		}
		if (pos.z < zmax + 1) && (grid[pos.x][pos.y][pos.z + 1] == Matter::Air) {
			grid[pos.x][pos.y][pos.z + 1] = Matter::Water;
			flood.push(Coord{x: pos.x, y: pos.y, z: pos.z + 1});
		}
	}


	for i in 0..cubes.len() {
		if (cubes[i].x > 0) && (grid[cubes[i].x - 1][cubes[i].y][cubes[i].z] == Matter::Air) {
			surface -= 1;
		}
		if (cubes[i].x < xmax + 1) && (grid[cubes[i].x + 1][cubes[i].y][cubes[i].z] == Matter::Air) {
			surface -= 1;
		}
		if (cubes[i].y > 0) && (grid[cubes[i].x][cubes[i].y - 1][cubes[i].z] == Matter::Air) {
			surface -= 1;
		}
		if (cubes[i].y < ymax + 1) && (grid[cubes[i].x][cubes[i].y + 1][cubes[i].z] == Matter::Air) {
			surface -= 1;
		}
		if (cubes[i].z > 0) && (grid[cubes[i].x][cubes[i].y][cubes[i].z - 1] == Matter::Air) {
			surface -= 1;
		}
		if (cubes[i].z < zmax + 1) && (grid[cubes[i].x][cubes[i].y][cubes[i].z + 1] == Matter::Air) {
			surface -= 1;
		}
	}

	println!("Surface #2 = {}", surface);

	Ok(())
}

