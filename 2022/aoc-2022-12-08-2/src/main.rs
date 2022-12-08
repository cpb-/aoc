
use std::io;
use std::io::prelude::*;
use std::io::BufReader;


fn main() -> io::Result<()> {

	let reader = BufReader::new(io::stdin());

	let mut trees: Vec<Vec<char>> = vec![];
	let mut score: Vec<Vec<u32>> = vec![];

	for line in reader.lines() {
		trees.push(line.unwrap().chars().collect());
	}

	let mut y: usize;
	let mut x: usize;

	let height: usize = trees.len();
	let width: usize = trees[0].len();

	// Create the score array
	y = 0;
	while y < height {
		score.push(vec![]);
		x = 0;
		while x < width {
			score[y].push(1);
			x += 1;
		}
		y += 1;
	}

	// Traverse all cells
	y = 0;
	while y < height {
		score.push(vec![]);
		x = 0;
		while x < width {
			score[y][x] = compute_score(&trees, x, y);
			x += 1;
		}
		y += 1;
	}

	let mut max: u32 = 0;
	y = 0;
	while y < height {
		score.push(vec![]);
		x = 0;
		while x < width {
			if score[y][x] > max {
				max = score[y][x];
			}
			x += 1;
		}
		y += 1;
	}
	


	println!("Max score: {}", max);

	Ok(())
}



fn compute_score(trees: &Vec<Vec<char>>, x: usize, y: usize) -> u32 {

	let mut score: u32 = 1;

	score *= compute_north_view(trees, x, y);
	score *= compute_sud_view(trees, x, y);
	score *= compute_east_view(trees, x, y);
	score *= compute_west_view(trees, x, y);

	return score;
}


fn compute_north_view(trees: &Vec<Vec<char>>, x: usize, y: usize) -> u32 {

	let mut view: u32 = 0;
	let mut i: usize = y;
	while i > 0 {
		i -= 1;
		view += 1;
		if trees[i][x] >= trees[y][x] {
			break;
		}
	}
	return view;

}

fn compute_sud_view(trees: &Vec<Vec<char>>, x: usize, y: usize) -> u32 {

	let mut view: u32 = 0;
	let mut i: usize = y;
	while i < trees.len() - 1 {
		i += 1;
		view += 1;
		if trees[i][x] >= trees[y][x] {
			break;
		}
	}
	return view;

}

fn compute_west_view(trees: &Vec<Vec<char>>, x: usize, y: usize) -> u32 {

	let mut view: u32 = 0;
	let mut i: usize = x;
	while i > 0 {
		i -= 1;
		view += 1;
		if trees[y][i] >= trees[y][x] {
			break;
		}
	}
	return view;

}

fn compute_east_view(trees: &Vec<Vec<char>>, x: usize, y: usize) -> u32 {

	let mut view: u32 = 0;
	let mut i: usize = x;
	while i < trees[y].len() - 1 {
		i += 1;
		view += 1;
		if trees[y][i] >= trees[y][x] {
			break;
		}
	}
	return view;
}
