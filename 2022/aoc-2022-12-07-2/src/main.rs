
use std::io;
use std::io::prelude::*;
use std::io::BufReader;


#[derive(PartialEq)]
enum FileType {
	Regular,
	Directory
}	


struct TreeNode {
	name: String,
	filetype: FileType,
	size: usize,
	subdirs: Vec<TreeNode>,
	files: Vec<TreeNode>
}


impl TreeNode {
	pub fn new(nm: String, ft: FileType, sz: usize) -> TreeNode {
		return TreeNode {
			name: nm,
			filetype: ft,
			size: sz,
			subdirs: vec![],
			files: vec![]
		};
	}

	pub fn add_child(&mut self, child: TreeNode) {
		if child.filetype == FileType::Regular {
			self.files.push(child);
		} else {
			self.subdirs.push(child);
		}
	}
}


fn main() -> io::Result<()> {

	let reader = BufReader::new(io::stdin());

	let mut pwd: Vec<String> = vec![];

	let mut rootfs: TreeNode = TreeNode::new("/".to_string(), FileType::Directory, 0);

	let mut _total: usize = 0;

	let mut to_remove: usize = 0;

	for result in reader.lines() {

		let line:String = result.unwrap();
		
		if line.trim().is_empty() {
			break;
		}
		if line.starts_with("$ cd /") {
			// Already there
			continue;
		}
		if line.starts_with("$ cd") {
			if &line[5..] == ".." {
				pwd.pop();
			} else {
				pwd.push(line[5..].to_string());
			}
		} else if line.starts_with("$ ls") {
			// nothing
		} else if line.starts_with("dir ") {
			// add line[4...]
			let new_node: TreeNode = TreeNode::new(line[4..].to_string(), FileType::Directory, 0);
			// println!("Add dir {}", new_node.name);
			add_child_to(&pwd, & mut rootfs, new_node);
		} else {
			// add size (1st field)
			let mut itr = line.split(' ');
			let new_size: usize = itr.next().unwrap().to_string().parse::<usize>().unwrap();
			let new_name: String = itr.next().unwrap().to_string();
			let new_node: TreeNode = TreeNode::new(new_name, FileType::Regular, new_size);
			// println!("Add file {}", new_node.name);
			add_child_to(&pwd, & mut rootfs, new_node);
		}
	}
	compute_directory_size(& mut rootfs);

	let needed: usize = rootfs.size - 40000000;

	find_directory_to_remove(& mut rootfs, needed, &mut to_remove);

	println!("To Remove: {}", to_remove);

	Ok(())
}


fn add_child_to(pwd: &[String], current: &mut TreeNode, node: TreeNode)  {

	if !pwd.is_empty() {
		for mut sub in &mut *current.subdirs {
			if sub.name == pwd[0] {
				add_child_to(&pwd[1..], &mut sub, node);
				return;
			}
		}
	} else {
		current.add_child(node);
	}
}


fn compute_directory_size(current: &mut TreeNode) {

	current.size = 0;
	for file in &mut *current.files {
		current.size += file.size;
	}
	for mut dir in &mut *current.subdirs {
		compute_directory_size(&mut dir);
		current.size += dir.size;
	}

}


fn find_directory_to_remove(current: &mut TreeNode, needed: usize, to_remove: &mut usize) {

	for mut dir in &mut *current.subdirs {
		find_directory_to_remove(&mut dir, needed, to_remove);
	}
	if current.size >= needed {
		if (*to_remove == 0) || (*to_remove > current.size) {
			*to_remove = current.size;
		}
	}
}


