#include "btree.hpp"
#include <iostream>
#include <cstring>

using namespace std;

#define MAX_HEIGHT 64

btree::btree(){
	root = new page(LEAF);
	height = 1;
};

void btree::insert(char *key, uint64_t val){
	page* node_path[MAX_HEIGHT];
	int path_length = 0;
	
	auto curr_node = root;
	
	// 루트부터 리프까지의 경로를 배열에 저장
	while(curr_node && curr_node->get_type() == INTERNAL){
		node_path[path_length++] = curr_node;
		curr_node = (page *)curr_node->find(key);
	}
	if(!curr_node){
		cout << "Can't find target leaf" << '\n';
		return;
	}
	node_path[path_length++] = curr_node;

	// 리프부터 루트 방향으로 split propagation 처리
	for(int i = path_length - 1; i >= 0; i--){
		const size_t key_size = strlen(key) + 1;
		const uint16_t record_size = sizeof(uint16_t) + key_size + sizeof(val);

		curr_node = node_path[i];
		if(!curr_node->is_full(record_size)){
			curr_node->insert(key, val);
			return;
		}

		char* parent_key = nullptr;
		val = (uint64_t)curr_node->split(key, val, &parent_key);
		key = parent_key;
	}
	
	// 루트까지 split이 전파된 경우 새로운 루트 생성
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
