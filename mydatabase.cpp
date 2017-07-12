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

	d_size = 0;
	Person p;
	while(true){
		p.read(d_fp);
		if(d_fp.eof()) break;
		d_size++;
	}

	d_fp.clear();
}

int MyDatabase::size(){
	return d_size;
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
	Person p;

	db->d_fp.clear();
	db->d_fp.seekg(0, ios::beg);

	while(true){
		p.read(db->d_fp);
		if(db->d_fp.eof())
			break;
		db->d_heap.push_back(p);
	};
}

void MyDatabase::p_writeSorted(MyDatabase *db, ostream &fp){
	int holdValue, lastSize;

	lastSize = db->d_heap.size();

	while(lastSize > 0){
		holdValue = db->v_heapSize;
		// Write new elements
		while(holdValue < lastSize){
			lastSize -= 1;
			db->d_heap[lastSize].write(fp);
		}
	}
}

void MyDatabase::p_buildHeap(){
	int n;

	d_heapSize = 0;

	while(d_heapSize != d_size){
		// Add new elements to the heap
		n = d_heap.size();
		while(d_heapSize != n){
			heapify_up(d_heap, d_heapSize);
			d_heapSize += 1;
		}
	}
}

void MyDatabase::p_popSorted(){
	while(d_heapSize > 0){
		swap(d_heap[0], d_heap[d_heapSize - 1]);
		heapify_down(d_heap, 0, d_heapSize - 1);

		// d_heapSize must be decremented here, after the statements above.
		d_heapSize -= 1;
		v_heapSize -= 1;
	}
}

void MyDatabase::parallel_heapsort(ostream &fp){
	// Initialize heap to the exact size we want
	d_heap.clear();
	d_heap.reserve(d_size);

	thread thr(MyDatabase::p_readPeople, this);
	p_buildHeap();
	thr.join();

	v_heapSize = d_heapSize; // Sets up thread-shared variable
	thr = thread(MyDatabase::p_writeSorted, this, ref(fp));
	p_popSorted();
	thr.join();

	// Clears the memory reserved
	d_heap.clear();
	d_heap.shrink_to_fit();
}
