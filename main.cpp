#include <QCoreApplication>
#include <fstream>
#include <iostream>

#include <mydatabase.h>
#include <ctime>

using std::clock_t;
using std::clock;
using std::cout;

int main(int argc, char *argv[]){
	MyDatabase db = MyDatabase();
	std::ofstream file;

	clock_t beg, end;

	// db.printAll();

	file.open("output.db", file.in | file.trunc);

	beg = clock();
	db.normal_heapsort(file);
	end = clock();

	cout << "Normal heapsort (clocks): " << (end-beg) / (double) CLOCKS_PER_SEC << '\n';

	return 0;
}
