#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <fstream>
#include <string>

class Person {
	std::string d_name;
	long int d_id;
public:
	Person();
	Person(std::string name, long int id);

	std::string name();
	long int id();
	void print();
};

class MyDatabase {
	std::fstream d_fp;
	std::string s_filename;

	// Reads next person in the database file.
	Person readPerson();

public:
	MyDatabase();

	// The database file is a sequence of ID (long int) / name (string).
	// The input file 'fp' should be a sequence of ID (a string representing a number) and
	//   a name, separated by blank spaces.
	void storeNewPeople(std::istream &fp);

	// Prints all records in the database file.
	void printAll();

	// Conventional heapsort. First read, then sort.
	void normal_heapsort(std::ostream &fp);
};

#endif // MYDATABASE_H
