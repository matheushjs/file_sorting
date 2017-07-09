#include <iostream>
#include <vector>
#include <thread>

#include "mydatabase.h"

using namespace std;


Person::Person() {}
Person::Person(string name, long int id) : d_name(name), d_id(id) {}

string Person::name() {
	return d_name;
}

long int Person::id() {
	return d_id;
}

void Person::print() {
	cout << d_name << '\n';
	cout << d_id << '\n';
}

bool Person::operator< (const Person &p2){
	return this->d_id < p2.d_id ? true : false;
}

bool Person::operator== (const Person &p2){
	return this->d_id == p2.d_id ? true : false;
}

bool Person::operator> (const Person &p2){
	return this->d_id > p2.d_id ? true : false;
}

void Person::read(istream &fp){
	char buf[500];

	fp.getline(buf, 500, '\0');
	this->d_name.assign(buf);

	fp.read((char *) &this->d_id, sizeof(long int));
}

void Person::write(ostream &fp){
	string &str = this->d_name;
	long int &num = this->d_id;

	fp.write(str.c_str(), str.size()+1);
	fp.write((char *) &num, sizeof(long int));
}



MyDatabase::MyDatabase() : s_filename("database.db") {
	d_fp.open(s_filename, d_fp.out | d_fp.in);

	if(!d_fp.is_open()){
		cout << "Creating new file.\n";
		d_fp.open(s_filename, d_fp.in | d_fp.out | d_fp.trunc);
	}
}

Person MyDatabase::readPerson(){
	Person p;
	p.read(d_fp);
	return p;
}

void MyDatabase::writePerson(Person &p){
	p.write(d_fp);
}


void MyDatabase::readFromInputFile(istream &fp){
	string str;
	long int num;

	d_fp.clear();
	d_fp.seekp(0, d_fp.end);
	do {
		if(fp >> num && fp >> str){
			d_fp.write(str.c_str(), str.size()+1);
			d_fp.write((char *) &num, sizeof(long int));
		}
	} while(!fp.eof());
	d_fp.flush();
}

void MyDatabase::printAll(){
	Person p;

	d_fp.clear();
	d_fp.seekg(0, d_fp.beg);

	do {
		p = readPerson();

		if(!d_fp.eof()) p.print();
		else break;

	} while(true);
}

#define PARENT(X) ((X-1)/2)
#define LEFT(X) (X*2+1)
#define RIGHT(X) (X*2+2)

void MyDatabase::heapify_up(vector<Person> &vec, int index){
	while(index != 0) {
		if(vec[PARENT(index)] < vec[index]){
			swap(vec[PARENT(index)], vec[index]);
			index = PARENT(index);
		} else break;
	}
}

void MyDatabase::heapify_down(vector<Person> &vec, int index, int size){
	int largest, left, right;

	while(true){
		left = LEFT(index);
		right = RIGHT(index);
		largest = index;

		if(left < size && vec[left] > vec[largest])
			largest = left;

		if(right < size && vec[right] > vec[largest])
			largest = right;

		if(largest != index){
			swap(vec[index], vec[largest]);
			index = largest;
		} else break;
	}
}

void MyDatabase::normal_heapsort(ostream &fp){
	vector<Person> vec;
	Person p;

	d_fp.clear();
	d_fp.seekg(0, ios::beg);

	do {
		p = readPerson();

		if(!d_fp.eof()) {
			vec.push_back(p);
			this->heapify_up(vec, vec.size()-1);
		} else break;
	} while(true);

	for(int i = vec.size()-1; i > 0; i--){
		swap(vec[0], vec[i]);
		heapify_down(vec, 0, i);
	}

	for(Person &p: vec){
		p.write(fp);
	}
}

void MyDatabase::p_readPeople(MyDatabase *db){
	unique_lock<mutex> lock(db->d_queueMut, defer_lock);
	vector<Person> vec;
	Person p;

	db->d_fp.clear();
	db->d_fp.seekg(0, ios::beg);

	do {
		vec.clear();

		// Reads a block of 100 Person
		for(int i = 0; i < 100; i++){
			p.read(db->d_fp);
			if(!db->d_fp.eof()){
				vec.push_back(p);
			} else break;
		}

		// Adds the block of Person to the queue
		while(!lock.try_lock());
		cout << "Read block\n";

		if(db->d_fp.eof())
			break;

		for(Person &p: vec){
			db->d_queue.push(p);
		}

		lock.unlock();
	} while (true);

	db->d_procEnded = true;
}

void MyDatabase::p_writeSorted(){

}

void MyDatabase::p_buildHeap(){
	unique_lock<mutex> lock(d_queueMut, defer_lock);
	d_vec.clear();

	while(true){
		// Lock queue
		while(!lock.try_lock());
		cout << "BuildHeap\n";

		// Add 1 Person from the queue to the heap
		if(d_queue.size() != 0){
			d_vec.push_back(d_queue.front());
			d_queue.pop();
			heapify_up(d_vec, d_vec.size()-1);
		}

		// Unlock queue
		lock.unlock();

		// Verify end-of-process
		if(d_procEnded == true){
			// Add all elements from queue
			while(d_queue.size() != 0){
				d_vec.push_back(d_queue.front());
				d_queue.pop();
				heapify_up(d_vec, d_vec.size()-1);
			}
			// Exit
			break;
		}
	}
}

void MyDatabase::p_popSorted(){

}

void MyDatabase::parallel_heapsort(ostream &fp){
	d_procEnded = false;

	thread thr(MyDatabase::p_readPeople, this);
	p_buildHeap();
	thr.join();

	// Sequential sort. Yet.
	for(int i = d_vec.size()-1; i > 0; i--){
		swap(d_vec[0], d_vec[i]);
		heapify_down(d_vec, 0, i);
	}

	for(Person &p: d_vec)
		p.print();
}
