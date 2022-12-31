
use std::io;
use std::io::prelude::*;
use std::io::BufReader;


#[derive(Clone)]
struct Valve {
	name: String,
	flow_rate: u32,
	tunnels: Vec<(usize, u32)>,
	tunnels_name: Vec<String>,
	disabled: bool,
}

struct Node {
	flow: u32,
	neighbours: Vec<(usize, u32)>
}


const MAX_MINUTES: u32 = 30;

fn main() -> io::Result<()> {

	let reader = BufReader::new(io::stdin());

	let mut valves: Vec<Valve> = vec![];

        for line in reader.lines() {
		let mut strline = line.unwrap();

		strline.replace_range(0..6, "");
		let name: String = strline[0..2].trim().to_string();

		strline.replace_range(0..17, "");
		let substr: Vec<&str> = strline.split(";").collect();
		let rate = substr[0].parse::<u32>().unwrap();

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
		valves.push(Valve{name: name.to_string(), flow_rate: rate, tunnels: vec![], tunnels_name: tunnels, disabled: false});
	}

	let mut start: usize = 0;

	// Direct access to valve indexes instead of names.
	for i in 0..valves.len() {
		if valves[i].name == "AA" {
			start = i;
		}
		for j in 0..valves[i].tunnels_name.len() {
			for k in 0..valves.len() {
				if valves[i].tunnels_name[j].eq(&valves[k].name) {
					valves[i].tunnels.push((k, 1));
					break;
				}
			}
		}
	}

	/* Remove valves with zero flow rate, and update tunnels around them. */

//	let mut nb_nodes: usize = 0;

	for v in 0 .. valves.len() {

		if (valves[v].flow_rate == 0) && (v != start) {

			for i in 0 .. valves[v].tunnels.len() - 1 {
				let a = valves[v].tunnels[i].0;

				for j in i + 1 .. valves[v].tunnels.len() {
					let b = valves[v].tunnels[j].0;
					let d = valves[v].tunnels[i].1 + valves[v].tunnels[j].1;
					valves[a].tunnels.push((b, d));
					valves[b].tunnels.push((a, d));
				}
			}
			for i in 0 .. valves[v].tunnels.len() {
				let a = valves[v].tunnels[i].0;
				let mut idx: usize = 0;
				while idx < valves[a].tunnels.len() {
					if valves[a].tunnels[idx].0 == v {
						break;
					}
					idx += 1;
				}
				assert_ne!(idx, valves[a].tunnels.len());
				valves[a].tunnels.remove(idx);
			}
			valves[v].disabled = true;
//		} else {
//			nb_nodes += 1;
		}
	}

	let mut graph: Vec<Node> = vec![];
	let mut new_index: Vec<usize> = vec![];
	let mut old_index: Vec<usize> = vec![];

	for v in 0..valves.len() {
		new_index.push(graph.len());
		if valves[v].disabled {
			continue;
		}
		let nd = Node{flow: valves[v].flow_rate, neighbours: vec![]};
		old_index.push(v);
		graph.push(nd);
	}
	for n in 0 .. graph.len() {
		let v = old_index[n];
		for i in 0 .. valves[v].tunnels.len() {
			let nb = new_index[valves[v].tunnels[i].0];
			let wg = valves[v].tunnels[i].1;
			graph[n].neighbours.push((nb, wg));
		}
	}

	start = new_index[start];

/*	for n in 0 .. graph.len() {
		println!("{} flow={} :", n, graph[n].flow);
		for i in 0 .. graph[n].neighbours.len() {
			println!("   {} ({})",
				graph[n].neighbours[i].0,
				graph[n].neighbours[i].1);
		}
	}
*/
	assert_eq!(graph.len(), 16);

	let mut distances: Vec<Vec<u32>> = vec![];
	for n in 0 .. graph.len() {
		distances.push(dikjstra(&graph, n));
		distances[n][n] = 0;
	}


	let mut flow: Vec<u32> = vec![];

	let mut me: u32 = 0;
	while me < (1 << graph.len()) {
		flow.push(find_best(&graph, &distances, start, MAX_MINUTES - 4, me as u16));
		me += 1;
		if me%1000 == 0 {
			println!("Running... {}", me);
		}
	}
	
	let mut best: u32 = 0;
	me = 0;
	while me < (1 << graph.len()) {
		
		let elephant: u16 = ! ((me & 0xFFFF) as u16);
		if flow[(me & 0xFFFF) as usize] + flow[elephant as usize] > best {
			best = flow[me as usize] + flow[elephant as usize];
		}
		me += 1;
	}

	println!("Best score = {}", best);


	Ok(())
}


fn dikjstra(graph: &Vec<Node>, start: usize) -> Vec<u32> {

	let mut distances: Vec<u32> = vec![];
	let mut visited: Vec<bool> = vec![];
	let mut cost: Vec<Vec<u32>> = vec![];
	let mut count: usize = 0;

	for i in 0 .. graph.len() {
		cost.push(vec![]);
		for j in 0 .. graph.len() {
			cost[i].push(u32::MAX);
			for k in graph[i].neighbours.clone() {
				if j == k.0 {
					cost[i][j] = k.1;
				}
			}
		}
	}

/*	println!("Cost table");	
	for i in 0..cost.len() {
		print!("{}: ", i);
		for j in 0 .. cost[i].len() {
			print!(" {} ", cost[i][j]);
		}
		println!("");
	}
*/
	for i in 0 .. graph.len() {
		distances.push(cost[start][i]);
		visited.push(false);
	}
	visited[start] = true;
	count += 1;

	while count < graph.len() - 1 {

		let mut min: u32 = u32::MAX;

		let mut idx: usize = 0;

		for i in 0 .. graph.len() {
			if (distances[i] < min) && (! visited[i]) {
				min = distances[i];
				idx = i;
			}
		}
		visited[idx] = true;

		for i in 0 .. graph.len() {
			if ! visited[i] {
				if (cost[idx][i] < u32::MAX) && (min < u32::MAX) && (min + cost[idx][i] < distances[i]) {
					distances[i] = min + cost[idx][i];
				}
			}
		}
		count += 1;
	}

	return distances;
}



fn find_best(graph: &Vec<Node>, distances: &Vec<Vec<u32>>, start: usize, left: u32, open: u16) -> u32 {

	let mut best: u32 = 0;

	for n in 0 .. graph.len() {
		if open & (1 << n) != 0 {
			continue;
		}
		let d = distances[start][n];
		if left <= d {
			continue;
		}
		let mut f = graph[n].flow * (left - 1 - d);

		f += find_best(&graph, distances, n, left - 1 - d, open | (1 << n));
		
		if f > best {
			best = f;
		}
	}

	return best;
}
