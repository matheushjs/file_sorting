#include <QCoreApplication>
#include <fstream>
#include <iostream>

#include <mydatabase.h>
#include <ctime>

using std::clock_t;
using std::clock;
using std::cout;

int main(int argc, char *argv[]){
	MyDatabase db;
	std::ofstream file;

	/*
	std::ifstream infile;
	infile.open("input.txt", std::ios::in);
	db.readFromInputFile(infile);
	*/

	/* Database file has 344MB
	 * 10 million records.
	 */

	clock_t beg, end;

	// db.printAll();

	file.open("output.db", file.in | file.trunc);

	beg = clock();
	db.normal_heapsort(file);
	file.flush()
	end = clock();
	cout << "Normal heapsort (clocks): " << (end-beg) / (double) CLOCKS_PER_SEC << '\n';

	file.close();
	file.open("output.db", file.in | file.trunc);

	beg = clock();
	db.parallel_heapsort(file);
	file.flush();
	end = clock();
	cout << "Parallel heapsort (clocks): " << (end-beg) / (double) CLOCKS_PER_SEC << '\n';

	return 0;
}
