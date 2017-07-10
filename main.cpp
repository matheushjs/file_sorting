#include <QCoreApplication>
#include <fstream>
#include <iostream>

#include <mydatabase.h>
#include <ctime>

using std::clock_t;
using std::clock;
using std::cout;
using std::ofstream;
using std::chrono::system_clock;
using std::chrono::time_point;
using std::chrono::duration;

int main(int argc, char *argv[]){
	MyDatabase db;
	ofstream file;

	cout << "Elements counted: " << db.size() << '\n';

	/*
	std::ifstream infile;
	infile.open("input.txt", std::ios::in);
	db.readFromInputFile(infile);
	*/

	/* Database file has 344MB
	 * 10 million records.
	 */

	clock_t beg, end;
	time_point<system_clock> begt, endt;

	file.open("output.db", file.in | file.trunc);

	begt = system_clock::now();
	beg = clock();
	db.normal_heapsort(file);
	file.flush();
	end = clock();
	endt = system_clock::now();
	cout << "Normal heapsort (clocks): " << (end-beg) / (double) CLOCKS_PER_SEC << '\n';
	cout << "Normal heapsort (time): " << duration<double>(endt-begt).count() << '\n';

	file.close();
	file.open("output.db", file.in | file.trunc);

	begt = system_clock::now();
	beg = clock();
	db.parallel_heapsort(file);
	file.flush();
	end = clock();
	endt = system_clock::now();
	cout << "Parallel heapsort (clocks): " << (end-beg) / (double) CLOCKS_PER_SEC << '\n';
	cout << "Parallel heapsort (time): " << duration<double>(endt-begt).count() << '\n';

	return 0;
}
