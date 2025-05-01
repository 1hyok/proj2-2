#include "page.hpp"
#include <iostream> 
#include <cstring> 
#include <string.h>

using namespace std;

void put2byte(void *dest, uint16_t data){
	*(uint16_t*)dest = data;
}

uint16_t get2byte(void *dest){
	return *(uint16_t*)dest;
}

page::page(uint16_t type){
	hdr.set_num_data(0);
	hdr.set_data_region_off(PAGE_SIZE-1-sizeof(page*));
	hdr.set_offset_array((void*)((uint64_t)this+sizeof(slot_header)));
	hdr.set_page_type(type);
}

uint16_t page::get_type(){
	return hdr.get_page_type();
}

uint16_t page::get_record_size(void *record){
	uint16_t size = *(uint16_t *)record;
	return size;
}

char *page::get_key(void *record){
	char *key = (char *)((uint64_t)record+sizeof(uint16_t));
	return key;
}

uint64_t page::get_val(void *key){
	uint64_t val= *(uint64_t*)((uint64_t)key+(uint64_t)strlen((char*)key)+1);
	return val;
}

void page::set_leftmost_ptr(page *p){
	leftmost_ptr = p;
}

page *page::get_leftmost_ptr(){
	return leftmost_ptr;	
}

uint64_t page::find(char *key){
	// Please implement this function in project 2.
	uint64_t val = 0;

	const uint16_t *off_arr = (uint16_t *)(hdr.get_offset_array());
	for(int i = 0; i < hdr.get_num_data(); i++){
		const char *curr = get_key((void *)((uint64_t)this + off_arr[i]));
		if(!strcmp(curr, key)){
			val = get_val((void *)curr);
			cout << "\nFound Val:" << val << "\n";
			return val;
		}
	}

	cout << "There's no key " << key << "\n\n";
	return val;
}

bool page::insert(char *key,uint64_t val){
	// Please implement this function in project 2.
		
	const size_t key_size = strlen(key) + 1;
	const uint16_t record_size = sizeof(uint16_t) + key_size + sizeof(val);

	if(is_full(record_size)){
		cout << "Full! key:" << key << " val:" << val << "\n\n";
		return false;
	}
	if(find(key)){
		cout << "Redundancy! key:" << key << " val:" << val << "\n\n";
		return false;
	}
		
	uint16_t *off_arr = (uint16_t *)(hdr.get_offset_array());
	hdr.set_data_region_off(hdr.get_data_region_off()-record_size);
	const uint16_t offset = hdr.get_data_region_off() + 1;
	
	put2byte((void *)((uint64_t)this + offset), record_size);
	strcpy((char *)((uint64_t)this + offset + sizeof(record_size)), key);
	*(uint64_t *)((uint64_t)this + offset + sizeof(record_size) + key_size) = val;

	hdr.set_num_data(hdr.get_num_data() + 1);

	const uint32_t num_data = hdr.get_num_data();
	int insert_idx = num_data - 1;
	for(int i = 0; i < num_data - 1; i++)
		if(strcmp(get_key((void *)((uint64_t)this + off_arr[i])), key) >= 0){
			insert_idx = i;
			break;
		}

	for(int i = num_data - 1; i > insert_idx; i--)
		off_arr[i] = off_arr[i-1];
	off_arr[insert_idx] = offset;

	cout << "Inserted " << val << " to key " << key << "\n\n";
	return true;
}

page* page::split(char *key, uint64_t val, char** parent_key){
	// Please implement this function in project 3.
	page *new_page;
	return new_page;
}

bool page::is_full(uint64_t inserted_record_size){
	// Please implement this function in project 2.
	return sizeof(slot_header) + sizeof(uint16_t) * (hdr.get_num_data() + 1) > hdr.get_data_region_off() - inserted_record_size;
	// return true;
}

void page::defrag(){
	page *new_page = new page(get_type());
	int num_data = hdr.get_num_data();
	void *offset_array=hdr.get_offset_array();
	void *stored_key=nullptr;
	uint16_t off=0;
	uint64_t stored_val=0;
	void *data_region=nullptr;

	for(int i=0; i<num_data/2; i++){
		off= *(uint16_t *)((uint64_t)offset_array+i*2);	
		data_region = (void *)((uint64_t)this+(uint64_t)off);
		stored_key = get_key(data_region);
		stored_val= get_val((void *)stored_key);
		new_page->insert((char*)stored_key,stored_val);
	}	
	new_page->set_leftmost_ptr(get_leftmost_ptr());

	memcpy(this, new_page, sizeof(page));
	hdr.set_offset_array((void*)((uint64_t)this+sizeof(slot_header)));
	delete new_page;

}

void page::print(){
	uint32_t num_data = hdr.get_num_data();
	uint16_t off=0;
	uint16_t record_size= 0;
	void *offset_array=hdr.get_offset_array();
	void *stored_key=nullptr;
	uint64_t stored_val=0;

	printf("## slot header\n");
	printf("Number of data :%d\n",num_data);
	printf("offset_array : |");
	for(int i=0; i<num_data; i++){
		off= *(uint16_t *)((uint64_t)offset_array+i*2);	
		printf(" %d |",off);
	}
	printf("\n");

	void *data_region=nullptr;
	for(int i=0; i<num_data; i++){
		off= *(uint16_t *)((uint64_t)offset_array+i*2);	
		data_region = (void *)((uint64_t)this+(uint64_t)off);
		record_size = get_record_size(data_region);
		stored_key = get_key(data_region);
		stored_val= get_val((void *)stored_key);
		printf("==========================================================\n");
		printf("| data_sz:%u | key: %s | val :%lu | key_len:%lu\n",record_size,(char*)stored_key, stored_val,strlen((char*)stored_key));

	}
}




