#include"Machine.h"
extern Turing_Machine machine;
string dir;
void init(char* argv[]) {
	dir = argv[1];
	ifstream in(dir+"/test.tm");
	if (!in) {
		cout << "Fail to open the file!" << endl;
		abort();
	}
	while (!in.eof()) {
		string line;
		getline(in, line);
		if (line == "") {
			continue;
		}
		create(line);
	}
}
void create(string line) {
	if (line[0] != '#'&&line[0] != ';') {
		machine.add_trans(line);
		return;
	}
	int len = line.find(";", 0);
	if (len != string::npos) {
		line = line.substr(0, len);
	}
	len = line.length();
	for (int i = 0; i < len; i++) {
		if (line[i] == '#') {
			if (i != 0) {
				cout << "Syntax error!" << endl;
				abort();
			}
			continue;
		}
		else if (line[i] == 'Q') {
			int start = i + 5;
			int comma = line.find(",", start);
			string state;
			while (comma!=string::npos){
				state = line.substr(start, comma - start);
				machine.add_states(state);
				start = comma + 1;
				comma = line.find(",", start);
			}
			comma = line.find("}", start);
			state = line.substr(start, comma - start);
			machine.add_states(state);
			return;
		}
		else if (line[i] == 'S') {
			int start = i + 5;
			char symbol;
			while (start<len) {
				symbol = line[start];
				machine.add_alphabets(symbol);
				if (line[start + 1] == '}')
					break;
				start = start + 2;
			}
			return;
		}
		else if (line[i] == 'G') {
			int start = i + 5;
			char tape;
			while (start<len) {
				tape = line[start];
				machine.add_tape(tape);
				if (line[start + 1] == '}')
					break;
				start = start + 2;
			}
			return;
		}
		else if (line[i] == 'q') {
			int start = i + 5;
			int end = line.find(" ", start);
			string s_state;
			if (end == string::npos) {
				s_state = line.substr(start, len - start);
			}
			else {
				s_state = line.substr(start, end - start);
			}
			machine.add_start_states(s_state);
			return;
		}
		else if (line[i] == 'B') {
			i = i + 4;
			string bla = line.substr(i, 1);
			machine.add_blank(bla);
			return;
		}
		else if (line[i] == 'F') {
			int start = i + 5;
			int comma = line.find(",", start);
			string final;
			while (comma != string::npos) {
				final = line.substr(start, comma - start);
				machine.add_finalstates(final);
				start = comma + 1;
				comma = line.find(",", start);
			}
			comma = line.find("}", start);
			final = line.substr(start, comma - start);
			machine.add_finalstates(final);
			return;
		}
		else if (line[i] == 'N') {
			int N = line[i + 4] - '0';
			machine.add_tape_num(N);
			return;
		}
	}
}
void Turing_Machine::add_states(string a) {
	machine.states.push_back(a);
}
void Turing_Machine::add_alphabets(char a) {
	machine.alphabets.push_back(a);
}
void Turing_Machine::add_tape(char a) {
	machine.tape.push_back(a);
}
void Turing_Machine::add_start_states(string a) {
	machine.start_state = a;
}
void Turing_Machine::add_blank(string a) {
	machine.blank = a;
}
void Turing_Machine::add_finalstates(string a) {
	machine.finalstates.push_back(a);
}
void Turing_Machine::add_tape_num(int a) {
	machine.tape_num = a;
}
void Turing_Machine::add_trans(string line) {
	int len = line.find(";", 0);
	if (len != string::npos) {
		line = line.substr(0, len);
	}
	int start = 0;
	int space = line.find(" ", start);
	string current_state = line.substr(start, space - start);
	start = space + 1;
	space = line.find(" ", start);
	string current_tape = line.substr(start, space - start);
	TransResults res;
	start = space + 1;
	space = line.find(" ", start);
	res.rewrite=line.substr(start, space - start);
	start = space + 1;
	space = line.find(" ", start);
	res.l_or_r= line.substr(start, space - start);
	start = space + 1;
	space = line.find(" ", start);
	if (space == string::npos) {
		res.nextstate = line.substr(start, line.length() - start);
	}
	else {
		res.nextstate = line.substr(start, space - start);
	}
	pair<string, string> a = make_pair(current_state, current_tape);
	machine.trans[a] = res;
	return;
}