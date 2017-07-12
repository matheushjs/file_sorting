#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <fstream>
#include <string>
#include <queue>
#include <mutex>
#include <atomic>

class Person {
	std::string d_name;
	long int d_id;

public:
	Person();
	Person(std::string name, long int id);

	std::string name();
	long int id();
	void print();

	void read(std::istream &fp);
	void write(std::ostream &fp);

	bool operator< (const Person &p2);
	bool operator== (const Person &p2);
	bool operator> (const Person &p2);
};

class MyDatabase {
	std::fstream d_fp;
	std::string s_filename;

	// Structures for parallel heapsort
	// Both will be asynchronously, but safely, accessed.
	std::vector<Person> d_heap;
	int d_heapSize;
	volatile int v_heapSize;

	// Number of elements in the database
	int d_size;

	// Reads next person in the database file.
	Person readPerson();

	// Writes a person to the database file.
	void writePerson(Person &p);

	void heapify_up(std::vector<Person> &vec, int index);
	void heapify_down(std::vector<Person> &vec, int index, int size);

	// Procedures for parallel heapsorting
	static void p_readPeople(MyDatabase *db);
	static void p_writeSorted(MyDatabase *db, std::ostream &fp);
	void p_popSorted();
	void p_buildHeap();

public:
	MyDatabase();

	int size();

	// The database file is a sequence of ID (long int) / name (string).
	// The input file 'fp' should be a sequence of ID (a string representing a number) and
	//   a name, separated by blank spaces.
	void readFromInputFile(std::istream &fp);

	// Prints all records in the database file.
	void printAll();

	// Conventional heapsort. First read, then sort.
	void normal_heapsort(std::ostream &fp);

	// Parallel heapsort. First (read & build_heap) then (pop_sorted & write)
	void parallel_heapsort(std::ostream &fp);
};

#endif // MYDATABASE_H
