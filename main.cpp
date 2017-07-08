#include <QCoreApplication>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class Person {
	string d_name;
	long int d_id;
public:
	Person() {}
	Person(string name, long int id) : d_name(name), d_id(id) {}

	string name() {
		return d_name;
	}

	long int id() {
		return d_id;
	}

	void print() {
		cout << d_name << '\n';
		cout << d_id << '\n';
	}
};

class MyDatabase {
	fstream d_fp;
	string s_filename;

	// Reads next person in the database file.
	Person readPerson(){
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

public:

	// Default constructor. Attemps to open the database file; creates a new
	//   one if it doesn't exist.
	MyDatabase() : s_filename("database.db") {
		d_fp.open(s_filename, d_fp.out | d_fp.in);

		if(!d_fp.is_open()){
			cout << "Creating new file.\n";
			d_fp.open(s_filename, d_fp.in | d_fp.out | d_fp.trunc);
		}
	}

	// The database file is a sequence of ID (long int) / name (string).
	// The input file 'fp' should be a sequence of ID (a string representing a number) and
	//   a name, separated by blank spaces.
	void storeNewPeople(istream &fp){
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

	// Prints all records in the database file.
	void printAll(){
		d_fp.seekg(0, d_fp.beg);
		Person p;

		do {
			p = readPerson();

			if(!d_fp.eof()) p.print();
			else break;

		} while(true);
	}
};

int main(int argc, char *argv[]){
	MyDatabase db = MyDatabase();

	db.printAll();

	return 0;
}
