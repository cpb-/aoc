
use std::io;
use std::io::prelude::*;
use std::io::BufReader;
use std::collections::VecDeque;
//use std::collections::HashSet;


#[derive(Clone)]
struct Valve {
	name: String,
	flow_rate: u32,
	tunnels: Vec<usize>,
	tunnels_name: Vec<String>
}


const MAX_MINUTES: u32 = 30;

fn main() -> io::Result<()> {

	let reader = BufReader::new(io::stdin());

	let mut valves: Vec<Valve> = vec![];
	let mut total_flow: u32 = 0;

        for line in reader.lines() {
		let mut strline = line.unwrap();

		strline.replace_range(0..6, "");
		let name: String = strline[0..2].trim().to_string();

		strline.replace_range(0..17, "");
		let substr: Vec<&str> = strline.split(";").collect();
		let rate = substr[0].parse::<u32>().unwrap();
		total_flow += rate;

		let mut tunnelstr = substr[1].to_string();
		tunnelstr.replace_range(0..22, "");
		if tunnelstr.starts_with("s") {
			tunnelstr.replace_range(0..1, "");
		}
		tunnelstr.replace_range(0..1, "");
		
		let mut tunnels: Vec<String> = vec![];
		let tunnel_list : Vec<&str> = tunnelstr.split(", ").collect();
		for tunnel in tunnel_list {
			tunnels.push(tunnel.trim().to_string());
		}
		valves.push(Valve{name: name.to_string(), flow_rate: rate, tunnels: vec![], tunnels_name: tunnels});
	}

	// Direct access to valve indexes instead of names.
	for i in 0..valves.len() {
		// println!("From valve {}", i);
		for j in 0..valves[i].tunnels_name.len() {
			for k in 0..valves.len() {
				if valves[i].tunnels_name[j].eq(&valves[k].name) {
					// println!("  -> {}", k);
					valves[i].tunnels.push(k);
					break;
				}
			}
		}
	}

	println!("Starting: {} valves, flow max: {}", valves.len(), total_flow);

	let mut best_score: u32 = 0;
	
	search_path(&valves, &mut best_score, total_flow);

	println!("Best score {}", best_score);


	Ok(())
}



fn search_path(valves: &Vec<Valve>, best: &mut u32, total_flow: u32) {


	let mut queue: VecDeque<(usize, u32, u32, u64)> = VecDeque::new(); // node, minute, score, open valves as bitmap

	let mut seen: Vec<(usize, u32, u32, u64)> = vec![]; // node, minute, score, open valves as bitmap

	queue.push_back((0, 1, 0, 0));

	while let Some(step) = queue.pop_front() { // pop.front -> BFS, pop.back -> DFS

		let valve_num: usize = step.0;
		let minute: u32 = step.1;
		let mut score: u32 = step.2;
		let open_map: u64 = step.3;

		if minute > MAX_MINUTES  {
			// End of Branch
			continue;
		}
		if score + total_flow * (MAX_MINUTES - minute + 1) < *best {
			// The score at this point is too low for the branch being interesting.
			continue;
		}
		// Compute the flow for this minute.
		let mut flow: u32 = 0;
		for i in 0 .. 64 {
			if (open_map & (1 << i)) != 0 {
				flow += valves[i].flow_rate;
			}
		}
		if flow == total_flow {
			// Maximum pressure reached.
			score += flow * (MAX_MINUTES - minute + 1);
			if score > *best {
				// No need to explore something else.
				println!("Maximum flow reached. Score = {}", score);
				*best = score;
			}
			continue;
		}
		if score > *best {
			println!("Best score {}, flow {}, minute {}", score, flow, minute);
			*best = score;
		}

		if seen.contains(&(valve_num, 0, 0, open_map)) {
			continue;
		}
		seen.push((valve_num, 0, 0, open_map));

		//println!("valve: {}, minute: {}; score: {}, flow: {}, open_map: {:x}", valve_num, minute, score, flow, open_map);

		if valves[valve_num].flow_rate > 0 {
			// Test opening the current valve
			if (open_map & (1 << valve_num)) == 0 {
				queue.push_back((valve_num, minute + 1, score + flow, open_map | (1 << valve_num)));
			}
		}

		// Run to other valves without opening the current one.
		for v in 0 .. valves[valve_num].tunnels.len() {
			queue.push_back((valves[valve_num].tunnels[v], minute + 1, score + flow, open_map));
		}
	}
}
