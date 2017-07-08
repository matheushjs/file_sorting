#include <QCoreApplication>

#include <mydatabase.h>

int main(int argc, char *argv[]){
	MyDatabase db = MyDatabase();

	db.printAll();

	return 0;
}
