
use std::io;
use std::io::prelude::*;
use std::io::BufReader;

#[derive(PartialEq)]
enum Operation {
	None,
	Add,
	Multiply,
	Square
}


//#[derive(Copy, Clone)]
struct Monkey {
	number: usize,
	items: Vec<u32>,
	operation: Operation,
	value: u32,
	divisibility: u32,
	send_if_true: usize,
	send_if_false: usize,
	activity: u32
}

impl Monkey {
	pub fn new() -> Monkey {
		return Monkey {
			number: 0,
			items: vec![],
			operation: Operation::None,
			value: 0,
			divisibility: 0,
			send_if_true: 0,
			send_if_false: 0,
			activity: 0
		} 
	}
}


fn main() -> io::Result<()> {

	let mut reader = BufReader::new(io::stdin());

	let mut monkeys: Vec<Monkey> = vec![];

	loop {
		let monkey = read_monkey(&mut reader);
		monkeys.push(monkey.unwrap());
		if reader.buffer().is_empty() {
			break;
		}
	}

	let nb_monkeys = monkeys.len();
	let mut round: usize = 0;
	while round < 20 {
		let mut i: usize = 0;
		while i < nb_monkeys {
			while monkeys[i].items.len() > 0 {

				monkeys[i].activity += 1;

				let mut item = monkeys[i].items.remove(0);

				if monkeys[i].operation == Operation::Add {
					item += monkeys[i].value;
				} else if monkeys[i].operation == Operation::Multiply {
					item *= monkeys[i].value;
				} else if monkeys[i].operation == Operation::Square {
					item *= item;
				}
				item /= 3;

				if item % monkeys[i].divisibility == 0 {
					let dest = monkeys[i].send_if_true;
					monkeys[dest].items.push(item);
				} else {
					let dest = monkeys[i].send_if_false;
					monkeys[dest].items.push(item);
				}
			}

			i += 1;

		}
		round += 1;
	}
	

	for monkey in monkeys {
		println!("Monkey {}, activity {}", monkey.number, monkey.activity);
	}

	Ok(())
}



fn read_monkey<R: BufRead>(reader: &mut R) -> Result<Monkey, &'static str> {

	let mut monkey = Monkey::new();

	let mut line = String::new();

	reader.read_line(& mut line).expect("Data should be provided on standard input");
	assert!(line.starts_with("Monkey "));
	
	line.pop(); // \n
	line.pop(); //  :
	monkey.number = line[7..].to_string().parse::<usize>().unwrap();

	line = "".to_string();
	reader.read_line(& mut line).expect("Data should be provided on standard input");
	assert!(line.starts_with("  Starting items: "));
	line.pop();
	line = line[18..].to_string();
	for num in line.split(',') {
		monkey.items.push(num.trim().parse::<u32>().unwrap());
	}

	line = "".to_string();
	reader.read_line(& mut line).expect("Data should be provided on standard input");
	line.pop();
	if line.starts_with("  Operation: new = old + ") {
		monkey.operation = Operation::Add;
		monkey.value = 	line[25..].to_string().parse::<u32>().unwrap();
	} else if line.starts_with("  Operation: new = old * old") {
                monkey.operation = Operation::Square;
	} else if line.starts_with("  Operation: new = old * ") {
                monkey.operation = Operation::Multiply;
                monkey.value =  line[25..].to_string().parse::<u32>().unwrap();
	}
	
	line = "".to_string();
	reader.read_line(& mut line).expect("Data should be provided on standard input");
	assert!(line.starts_with("  Test: divisible by "));
	line.pop(); // \n
	monkey.divisibility = line[21..].to_string().parse::<u32>().unwrap();
	
	line = "".to_string();
	reader.read_line(& mut line).expect("Data should be provided on standard input");
	assert!(line.starts_with("    If true: throw to monkey "));
	line.pop(); // \n
	monkey.send_if_true = line[29..].to_string().parse::<usize>().unwrap();
	
	line = "".to_string();
	reader.read_line(& mut line).expect("Data should be provided on standard input");
	assert!(line.starts_with("    If false: throw to monkey "));
	line.pop(); // \n
	monkey.send_if_false = line[30..].to_string().parse::<usize>().unwrap();
	
	line = "".to_string();
	reader.read_line(& mut line).expect("Data should be provided on standard input");
	assert!(line.trim().is_empty());

	println!("Monkey {} Ok", monkey.number);
	return Ok(monkey);
}
