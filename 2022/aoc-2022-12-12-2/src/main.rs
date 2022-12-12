
use std::io;
use std::io::prelude::*;
use std::io::BufReader;

struct Point {
	x: usize,
	y: usize
}

struct PointLevel {
	x: usize,
	y: usize,
	l: usize
}

fn main() -> io::Result<()> {

	let reader = BufReader::new(io::stdin());

	let mut map: Vec<Vec<char>> = vec![];

	for line in reader.lines() {
		map.push(line.unwrap().chars().collect());
	}

	let height : usize = map.len();
	let width  : usize = map[0].len();

	println!("Width: {}, Height: {})", width, height);

	let mut current: Point = Point {x: 0, y:0};

	let mut y: usize = 0;
	while y < height {
		let mut x: usize = 0;
		while x < width {
			if map[y][x] == 'S' {
				map[y][x] = 'a';
			}
			if map[y][x] == 'E' {
				current.x = x;
				current.y = y;
				map[y][x] = 'z';
			}
			x += 1;
		}
		y += 1;
	}

	println!("Starting at ({}, {})", current.x, current.y);

	println!("Nb steps = {}", explore(&map, current, width, height));

	Ok(())
}


fn explore(map: &Vec<Vec<char>>, pos: Point, width: usize, height: usize) -> usize
{
	let mut to_explore: Vec<PointLevel> = vec![];
	let mut explored: Vec<Point> = vec![];

	to_explore.push(PointLevel{x: pos.x, y: pos.y, l: 0});

	while to_explore.len() > 0 {

		let p = to_explore.remove(0);

		// Destination reached ?
		if map[p.y][p.x] == 'a' {
			return p.l;
		}

		// Already explored ?
		let mut i: usize = 0;
		while i < explored.len() {
			if (explored[i].x == p.x) && (explored[i].y == p.y) {
					break;
			}
			i += 1;
		}
		if i < explored.len() {
			continue;
		}

		explored.push(Point{x: p.x, y: p.y});

		if p.y > 0 {
			if map[p.y - 1][p.x] as u32 >= map[p.y][p.x] as u32 - 1 {
				to_explore.push(PointLevel {x: p.x, y: p.y -1, l: p.l + 1})
			}
		}
		if p.y < height - 1 {
			if map[p.y + 1][p.x] as u32 >= map[p.y][p.x] as u32 - 1 {
				to_explore.push(PointLevel {x: p.x, y: p.y + 1, l: p.l + 1})
			}
		}
		if p.x > 0 {
			if map[p.y][p.x - 1] as u32 >= map[p.y][p.x] as u32 - 1 {
				to_explore.push(PointLevel {x: p.x - 1, y: p.y, l: p.l + 1})
			}
		}
		if p.x < width - 1 {
			if map[p.y][p.x + 1] as u32 >= map[p.y][p.x] as u32 - 1 {
				to_explore.push(PointLevel {x: p.x + 1, y: p.y, l: p.l + 1})
			}
		}

	}
	return 0;
}

