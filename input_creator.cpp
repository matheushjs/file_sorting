#include <random>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

string random_string(){
	static random_device r;
	static default_random_engine eng(r());
	static uniform_int_distribution<int> runif_size(1, 50);
	static uniform_int_distribution<char> runif_char(65, 90);

	int i, size;
	char ch;
	string str;

	size = runif_size(eng);

	for(i = 0; i < size; i++){
		str.push_back(runif_char(eng));
	}

	return str;
}

long int random_long(){
	static random_device r;
	static default_random_engine eng(r());
	static uniform_int_distribution<long int> runif;

	return runif(eng);
}

/*
int main(int argc, char *argv[]){
	fstream file;

	file.open("input.txt", file.out | file.trunc);

	int i;
	for(i = 0; i < 10000000; i++){
		file << random_long() << '\n';
		file << random_string() << '\n';
	}

	return 0;
}
*/
