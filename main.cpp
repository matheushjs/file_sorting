#include <QCoreApplication>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class MyDatabase {
    fstream d_fp;
    string s_filename;

public:
    MyDatabase() : d_fp(), s_filename("database.db") {
        d_fp.open(s_filename, d_fp.out | d_fp.in);

        if(!d_fp.is_open()){
            cout << "Creating file.\n";
            d_fp.open(s_filename, d_fp.in | d_fp.out | d_fp.trunc);
        }
    }

    // The file is a sequence of name (string) / ID (long int)
    void readPeople(istream &fp){
        string str;
        long int num;

        d_fp.seekp(0, d_fp.end);
        do {
            if(fp >> str && fp >> num){
                d_fp.write(str.c_str(), str.size()+1);
                d_fp.write((char *) &num, sizeof(long int));
            }
        } while(!fp.eof());
        d_fp.flush();
    }
};

int main(int argc, char *argv[]){
    MyDatabase db = MyDatabase();
    db.readPeople(cin);
    return 0;
}

