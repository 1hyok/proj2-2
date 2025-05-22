#include "btree.hpp"
#include <iostream> 
#include <vector>

using namespace std;

btree::btree(){
	root = new page(LEAF);
	height = 1;
};

void btree::insert(char *key, uint64_t val){
	// Please implement this function in project 3.)
	auto curr_node = root;
	vector<page*> node_path;
	
	while(curr_node && curr_node->get_type() == INTERNAL){
		node_path.push_back(curr_node);
		curr_node = (page *)curr_node->find(key);
	}
	if(!curr_node){
		cout << "Can't find target leaf" << '\n';
		return;
	}
	node_path.push_back(curr_node);

	while(!node_path.empty()){
		const size_t key_size = strlen(key) + 1;
		const uint16_t record_size = sizeof(uint16_t) + key_size + sizeof(val);

		curr_node = node_path.back();
		if(!curr_node->is_full(record_size)){
			curr_node->insert(key, val);
			return;
		}
		node_path.pop_back();

		char** parent_key = nullptr;
		val = (uint64_t)curr_node->split(key, val, parent_key);
		key = *parent_key;
	}
	
	auto tmp = root;
	root = new page(INTERNAL);
	root->insert(key, val);
	root->set_leftmost_ptr(tmp);
	height++;
}

uint64_t btree::lookup(char *key){
	// Please implement this function in project 3.
	uint64_t val = 0;
	val = (uint64_t)root;

	if(!val) return val;

	while(val && ((page *)val)->get_type() == INTERNAL){
		val = ((page *)val)->find(key);
	}

	if(!val) return val;
	return ((page *)val)->find(key);
}
