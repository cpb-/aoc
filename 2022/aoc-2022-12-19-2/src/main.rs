use std::io;
use std::io::prelude::*;
use std::io::BufReader;

struct Blueprint {
	id: u64,
	ore_robot_cost: u64,
	clay_robot_cost: u64,
	obsidian_robot_ore_cost: u64,
	obsidian_robot_clay_cost: u64,
	geode_robot_ore_cost: u64,
	geode_robot_obsidian_cost: u64
}


#[derive(Copy, Clone)]
struct Status {
	ore: u64,
	clay: u64,
	obs: u64,
	geode: u64,
	r_ore: u64,
	r_clay: u64,
	r_obs: u64,
	r_geode: u64,
	minute: u64
}


const DURATION: u64 = 32;

const MAX_ORE_ROBOTS: u64 = 12;
const MAX_CLAY_ROBOTS: u64 = 12;
const MAX_OBS_ROBOTS: u64 = 12;
const MAX_GEODE_ROBOTS: u64 = 24;

fn main() -> io::Result<()> {

	let reader = BufReader::new(io::stdin());
	let mut blueprints: Vec<Blueprint> = vec![];

	let mut id = 0;
	for line in reader.lines() {

		id += 1;

		let strline = line.unwrap();
		let words: Vec<&str> = strline.split(" ").collect();

		let ore_robot_cost = words[6].parse::<u64>().unwrap();
		let clay_robot_cost = words[12].parse::<u64>().unwrap();
		let obsidian_robot_ore_cost = words[18].parse::<u64>().unwrap();
		let obsidian_robot_clay_cost = words[21].parse::<u64>().unwrap();
		let geode_robot_ore_cost = words[27].parse::<u64>().unwrap();
		let geode_robot_obsidian_cost = words[30].parse::<u64>().unwrap();

		blueprints.push(Blueprint {
			id: id,
			ore_robot_cost: ore_robot_cost,
			clay_robot_cost: clay_robot_cost,
			obsidian_robot_ore_cost: obsidian_robot_ore_cost,
			obsidian_robot_clay_cost: obsidian_robot_clay_cost,
			geode_robot_ore_cost: geode_robot_ore_cost,
			geode_robot_obsidian_cost: geode_robot_obsidian_cost,
		});
	}

	let mut product: u64 = 1;

	for i in 0..3 {
		let bp = &blueprints[i];

		println!("Blueprint #{} ({},{},{}+{},{}+{})",
			bp.id,
			bp.ore_robot_cost,
			bp.clay_robot_cost,
			bp.obsidian_robot_ore_cost, bp.obsidian_robot_clay_cost,
			bp.geode_robot_ore_cost, bp.geode_robot_obsidian_cost);

		let q: u64 = compute_quality(&bp);
		println!("Blueprint #{} => {}", bp.id, q);
		product *= q;
	}

	println!("Product: {}", product);

	Ok(())
}


fn compute_quality(bp: &Blueprint) -> u64 {

	let mut dfs: Vec<Status> = vec![];

	let mut st: Status = Status {
			ore: 0,
			clay: 0,
			obs: 0,
			geode: 0,
			r_ore: 1,
			r_clay: 0,
			r_obs: 0,
			r_geode: 0,
			minute: 0 };

	dfs.push(st);

	let mut max_geode: u64 = 0;

	while dfs.len() > 0 {
		st = dfs.pop().unwrap();

		if st.minute > DURATION  {
			continue;
		}
		if st.geode > max_geode {
			max_geode = st.geode;
			println!("Max: minute {}, ({}, {}, {}, {}) ({}, {}, {}, {})",
				st.minute,
				st.ore, st.clay, st.obs, st.geode,
				st.r_ore, st.r_clay, st.r_obs, st.r_geode);
		}

		// What is the maximal geode numbers for this branch? Be optimistic, say we build a geode robot each remaining seconds.
		let mut optimist: u64 = st.geode;
		let mut r_optimist: u64 = st.r_geode;
		for _ in st.minute ..= DURATION  {
			optimist += r_optimist;
			r_optimist += 1;
		}
		// Prune.
		if optimist <= max_geode {
			continue;
		}

		// Passive wait.
		dfs.push(Status {
			ore: st.ore + st.r_ore, 
			clay: st.clay + st.r_clay, 
			obs: st.obs + st.r_obs, 
			geode: st.geode + st.r_geode, 
			r_ore: st.r_ore, 
			r_clay: st.r_clay, 
			r_obs: st.r_obs, 
			r_geode: st.r_geode, 
			minute: st.minute + 1
		});
		// Build an ore robot
		if (st.ore >= bp.ore_robot_cost) && (st.r_ore < MAX_ORE_ROBOTS) {
			dfs.push(Status {
				ore: st.ore + st.r_ore - bp.ore_robot_cost, 
				clay: st.clay + st.r_clay, 
				obs: st.obs + st.r_obs, 
				geode: st.geode + st.r_geode, 
				r_ore: st.r_ore + 1, 
				r_clay: st.r_clay, 
				r_obs: st.r_obs, 
				r_geode: st.r_geode, 
				minute: st.minute + 1
			});
		}
		// Build a clay robot
		if (st.ore >= bp.clay_robot_cost) && (st.r_clay < MAX_CLAY_ROBOTS) {
			// Only if we need more clay.
//			if (st.clay < bp.obsidian_robot_clay_cost) && (st.obs < bp.geode_robot_obsidian_cost) {
			
				dfs.push(Status {
					ore: st.ore + st.r_ore - bp.clay_robot_cost, 
					clay: st.clay + st.r_clay, 
					obs: st.obs + st.r_obs, 
					geode: st.geode + st.r_geode, 
					r_ore: st.r_ore, 
					r_clay: st.r_clay + 1, 
					r_obs: st.r_obs, 
					r_geode: st.r_geode, 
					minute: st.minute + 1
				});
//			}
		}
		// Build an obsidian robot
		if (st.ore >= bp.obsidian_robot_ore_cost) && (st.clay >= bp.obsidian_robot_clay_cost) && (st.r_obs < MAX_OBS_ROBOTS) {
			// Only if we need more obs.
//			if st.obs < bp.geode_robot_obsidian_cost {
				dfs.push(Status {
					ore: st.ore + st.r_ore - bp.obsidian_robot_ore_cost, 
					clay: st.clay + st.r_clay - bp.obsidian_robot_clay_cost, 
					obs: st.obs + st.r_obs, 
					geode: st.geode + st.r_geode, 
					r_ore: st.r_ore, 
					r_clay: st.r_clay, 
					r_obs: st.r_obs + 1, 
					r_geode: st.r_geode, 
					minute: st.minute + 1
				});
//			}
		}
		// Build a geode robot
		if (st.ore >= bp.geode_robot_ore_cost) && (st.obs >= bp.geode_robot_obsidian_cost) && (st.r_geode < MAX_GEODE_ROBOTS) {
			dfs.push(Status {
				ore: st.ore + st.r_ore - bp.geode_robot_ore_cost, 
				clay: st.clay + st.r_clay, 
				obs: st.obs + st.r_obs - bp.geode_robot_obsidian_cost, 
				geode: st.geode + st.r_geode, 
				r_ore: st.r_ore, 
				r_clay: st.r_clay, 
				r_obs: st.r_obs, 
				r_geode: st.r_geode + 1, 
				minute: st.minute + 1
			});
		}
	}

	return max_geode;
}



