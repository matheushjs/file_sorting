#include <iostream>
#include <vector>

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

Person MyDatabase::readPerson(){
	Person p;
	long int id;
	char buf[500];

	d_fp.getline(buf, 500, '\0');
	d_fp.read((char *) &id, sizeof(long int));

	if(!d_fp.eof()){
		p = Person(buf, id);
	}

	return p;
}

MyDatabase::MyDatabase() : s_filename("database.db") {
	d_fp.open(s_filename, d_fp.out | d_fp.in);

	if(!d_fp.is_open()){
		cout << "Creating new file.\n";
		d_fp.open(s_filename, d_fp.in | d_fp.out | d_fp.trunc);
	}
}

void MyDatabase::storeNewPeople(istream &fp){
	string str;
	long int num;

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
	d_fp.seekg(0, d_fp.beg);
	Person p;

	do {
		p = readPerson();

		if(!d_fp.eof()) p.print();
		else break;

	} while(true);
}

void MyDatabase::normal_heapsort(ostream &fp){
	vector<Person> vec;

	// Read all

}
