
use std::io;
use std::io::prelude::*;
use std::io::BufReader;

#[derive(Copy, Clone)]
struct Point {
	x: i32,
	y: i32
}

const NB_KNOTS:usize = 10;


fn main() -> io::Result<()> {

	let reader = BufReader::new(io::stdin());

	let mut knots: Vec<Point> = vec![];
	let mut visited: Vec<Point> = vec![];

	let mut i: usize = 0;
	while i < NB_KNOTS {
		knots.push(Point{x: 0, y: 0});
		i += 1;
	}

	for line in reader.lines() {
		let strline = line.unwrap();
		let nb = strline[2..].parse::<usize>().unwrap();

		if strline.starts_with("R ") {
			compute_move(&mut knots, 1, 0, nb, &mut visited);
		}
		if strline.starts_with("L ") {
			compute_move(&mut knots, -1, 0, nb, &mut visited);
		}
		if strline.starts_with("U ") {
			compute_move(&mut knots, 0, -1, nb, &mut visited);
		}
		if strline.starts_with("D ") {
			compute_move(&mut knots, 0, 1, nb, &mut visited);
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

fn compute_move(knots: &mut Vec<Point>, dx: i32, dy: i32, nb: usize, visited: &mut Vec<Point>) {
//println!("H:({}, {}), T:({}, {}), dx:{}, dy:{}, nb:{}", head.x, head.y, tail.x, tail.y, dx, dy, nb);
	let mut i: usize = 0;
	while i < nb {
//println!("H:({}, {}), T:({}, {}), dx:{}, dy:{}", head.x, head.y, tail.x, tail.y, dx, dy);

		knots[0].x += dx;
		knots[0].y += dy;

		let mut k: usize = 0;
		while k < knots.len() - 1 {

			if knots[k+1].x == knots[k].x {
				if (knots[k+1].y - knots[k].y) > 1 {
					knots[k+1].y -= 1;
				} else if (knots[k+1].y - knots[k].y) < -1 {
					knots[k+1].y += 1;
				}
			} else if knots[k+1].y == knots[k].y {
				if (knots[k+1].x - knots[k].x) > 1 {
					knots[k+1].x -= 1;
				} else if (knots[k+1].x - knots[k].x) < -1 {
					knots[k+1].x += 1;
				}
			} else {
				if ((knots[k+1].y - knots[k].y).abs() > 1) || ((knots[k+1].x - knots[k].x).abs() > 1) {
					if (knots[k+1].y - knots[k].y) > 0 {
						knots[k+1].y -= 1;
					} else if (knots[k+1].y - knots[k].y) < 0 {
						knots[k+1].y += 1;
					}
					if (knots[k+1].x - knots[k].x) > 0 {
						knots[k+1].x -= 1;
					} else if (knots[k+1].x - knots[k].x) < 0 {
						knots[k+1].x += 1;
					}
				}
			}

			k += 1;
		}

		let mut found: bool = false;
		let mut j: usize = 0;
		while j < visited.len() {
			if (visited[j].x == knots[NB_KNOTS - 1].x) && (visited[j].y == knots[NB_KNOTS - 1].y) {
				found = true;
				break;
			}
			j += 1;
		}
		if ! found {
			visited.push(knots[NB_KNOTS]);
		}
		i += 1;
	}
}



