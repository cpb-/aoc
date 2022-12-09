
use std::io;
use std::io::prelude::*;
use std::io::BufReader;

#[derive(Copy, Clone)]
struct Point {
	x: i32,
	y: i32
}



fn main() -> io::Result<()> {

	let reader = BufReader::new(io::stdin());

	let mut tail: Point = Point{x: 0, y: 0};
	let mut head: Point = Point{x: 0, y: 0};

	let mut visited: Vec<Point> = vec![];

	for line in reader.lines() {
		let strline = line.unwrap();
		let nb = strline[2..].parse::<usize>().unwrap();

		if strline.starts_with("R ") {
			compute_move(&mut head, &mut tail, 1, 0, nb, &mut visited);
		}
		if strline.starts_with("L ") {
			compute_move(&mut head, &mut tail, -1, 0, nb, &mut visited);
		}
		if strline.starts_with("U ") {
			compute_move(&mut head, &mut tail, 0, -1, nb, &mut visited);
		}
		if strline.starts_with("D ") {
			compute_move(&mut head, &mut tail, 0, 1, nb, &mut visited);
		}
	}
	println!("---- visited -----");
	let mut i: usize = 0;
	while i < visited.len() {
		println!("({}, {})", visited[i].x, visited[i].y);
		i += 1;
	}
	
	println!("Nb visited: {}", visited.len());

	Ok(())
}

fn compute_move(head: &mut Point, tail: &mut Point, dx: i32, dy: i32, nb: usize, visited: &mut Vec<Point>) {
//println!("H:({}, {}), T:({}, {}), dx:{}, dy:{}, nb:{}", head.x, head.y, tail.x, tail.y, dx, dy, nb);
	let mut i: usize = 0;
	while i < nb {
println!("H:({}, {}), T:({}, {}), dx:{}, dy:{}", head.x, head.y, tail.x, tail.y, dx, dy);

		head.x += dx;
		head.y += dy;

		if tail.x == head.x {
			if (tail.y - head.y) > 1 {
				tail.y -= 1;
			} else if (tail.y - head.y) < -1 {
				tail.y += 1;
			}
		} else if tail.y == head.y {
			if (tail.x - head.x) > 1 {
				tail.x -= 1;
			} else if (tail.x - head.x) < -1 {
				tail.x += 1;
			}
		} else {
			if ((tail.y - head.y).abs() > 1) || ((tail.x - head.x).abs() > 1) {
				if (tail.y - head.y) > 0 {
					tail.y -= 1;
				} else if (tail.y - head.y) < 0 {
					tail.y += 1;
				}
				if (tail.x - head.x) > 0 {
					tail.x -= 1;
				} else if (tail.x - head.x) < 0 {
					tail.x += 1;
				}
			}
		}
println!("  -->  H:({}, {}), T:({}, {}), dx:{}, dy:{}", head.x, head.y, tail.x, tail.y, dx, dy);
	
		let mut found: bool = false;
		let mut j: usize = 0;
		while j < visited.len() {
			if (visited[j].x == tail.x) && (visited[j].y == tail.y) {
				found = true;
				break;
			}
			j += 1;
		}
		if ! found {
			visited.push(*tail);
		}

		i += 1;

	}
}



