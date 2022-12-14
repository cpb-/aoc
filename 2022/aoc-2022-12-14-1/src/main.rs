
use std::io;
use std::io::prelude::*;
use std::io::BufReader;

struct Point {
	x: u32,
	y: u32
}


fn main() -> io::Result<()> {

	let reader = BufReader::new(io::stdin());

	let mut polylines: Vec<Vec<Point>> = vec![];
	let mut max_y: u32 = 0;
        for line in reader.lines() {
		let strline = line.unwrap();
		let points: Vec<&str> = strline.split(" -> ").collect();
		let mut polyline: Vec<Point> = vec![];
		for point in points {
			let coords: Vec<&str> = point.split(",").collect();
			let x: u32 = coords[0].parse::<u32>().unwrap();
			let y: u32 = coords[1].parse::<u32>().unwrap();
			if y > max_y {
				max_y = y;
			}
			polyline.push(Point{x: x, y: y});
		}
		polylines.push(polyline);

        }

	println!("Y max : {}", max_y);

	let mut units: u32 = 0;
	let mut sands: Vec<Point> = vec![];

	loop {
		if ! try_position(Point {x:500, y:0}, &polylines, &mut sands, max_y) {
			break;
		}

		units += 1;
	}
	println!("Nb units: {}", units);

	Ok(())
}

fn try_position(p: Point, rock: &Vec<Vec<Point>>, sand: &mut Vec<Point>, max_y: u32) -> bool {

	let mut pos: Point = Point{x: p.x, y: p.y};

	if pos.y > max_y {
		return false;
	}

	if occuped_cell(&pos, rock, sand) {
		return false;
	}
	
	pos.y += 1;
	if ! occuped_cell(&pos, rock, sand) {
		return try_position(pos, rock, sand, max_y);
	}
	pos.x -= 1;
	if ! occuped_cell(&pos, rock, sand) {
		return try_position(pos, rock, sand, max_y);
	}
	pos.x += 2;
	if ! occuped_cell(&pos, rock, sand) {
		return try_position(pos, rock, sand, max_y);
	}


//	println!("Sand: ({}, {})", p.x, p.y);
	sand.push(p);

	return true;
}


fn occuped_cell(p: &Point, rock: &Vec<Vec<Point>>, sand: &Vec<Point>) -> bool {

	if sand_cell(&p, sand) {
//	println!("<sand>");
	return true;
	}
	if rock_cell(&p, rock) {
//	println!("<rock {}, {}>", p.x, p.y);
		return true;
	}
	return false;
}


fn sand_cell(p: &Point, sand: &Vec<Point>) -> bool {
	
	for s in sand {
		if (s.x == p.x) && (s.y == p.y) {
			return true;
		}
	}
	return false;
}


fn rock_cell(p: &Point, rock: &Vec<Vec<Point>>) -> bool {
	

	for polyline in rock {
		let mut i: usize = 1;
		while i < polyline.len() {
			if on_segment(&(polyline[i - 1]), &(polyline[i]), p) {
//				println!("Segment {},{} -> {},{}", polyline[i - 1].x, polyline[i - 1].y, polyline[i].x, polyline[i].y);
				return true;
			}
			i += 1;
		}
	}
	return false;
}


fn on_segment(a: &Point, b: &Point, p: &Point) -> bool {

	if a.x <= b.x {
		if a.y <= b.y {
			return (a.x <= p.x) && (p.x <= b.x) && (a.y <= p.y) && (p.y <= b.y);
		} else {
			return (a.x <= p.x) && (p.x <= b.x) && (a.y >= p.y) && (p.y >= b.y);
		}
	} else {
		if a.y <= b.y {
			return (a.x >= p.x) && (p.x >= b.x) && (a.y <= p.y) && (p.y <= b.y);
		} else {
			return (a.x >= p.x) && (p.x >= b.x) && (a.y >= p.y) && (p.y >= b.y);
		}
	}
}
