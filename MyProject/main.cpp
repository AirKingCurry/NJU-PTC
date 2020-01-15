#include"Machine.h"
using namespace std;
Turing_Machine machine;
int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout<<"bash command!"<<endl;
		return -1;
	}
	init(argv);
	string path=argv[1];
	ifstream in(path+"/input.txt");
	if (!in) {
		cout << "Can't open the input file!" << endl;
		abort();
	}
	while (!in.eof()) {
		string line;
		getline(in, line);
		//cout<<line.length()<<endl;
		machine.run(line);
	}
	return 0;
}