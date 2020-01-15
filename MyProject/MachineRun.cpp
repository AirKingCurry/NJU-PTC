#include"Machine.h"
extern Turing_Machine machine;
extern string dir;
void Turing_Machine::run(string line) {
	
	ofstream out(dir+"/console.txt",ios::app);
	ofstream output(dir+"/result.txt",ios::app);
	if(!out){
		cout<<"Can't open console.txt!"<<endl;
		abort();
	}
	if(!output){
		cout<<"Can't open result.txt!"<<endl;
		abort();
	}
	string input = line;
	int len = input.length();
	int i = 0;
	//check whether input is illegal.
	for (; i < len; i++) {
		vector<char>::iterator it = find(machine.alphabets.begin(),machine.alphabets.end(),input[i]);
		if (it == machine.alphabets.end()) {
			out << "==================== ERR ====================" << endl;
			out << "The input " << input << " is illegal." << endl;
			out << "==================== END ====================" << endl;
			out.close();
			output << "Error" << endl;
			output.close();
			return;
		}
	}
	out << "Input:" << line << endl;
	out << "==================== RUN ====================" << endl;
	print id;
	init_id(id, input,machine.tape_num,machine.start_state);
	print_id(id,out);
	char* current = new char[machine.tape_num];
	for (int i = 0; i < machine.tape_num; i++) {
		current[i] = id.tape[i][id.head[i]];
	}
	pair<string, string> a = machine.find_trans(id.state, current);
	while(a.first != error && a.second != error) {
		map<pair<string, string>, TransResults>::iterator it = machine.trans.find(a);
		TransResults res = it->second;
		modify_id(id,res);
		print_id(id,out);
		for (int i = 0; i < machine.tape_num; i++) {
			current[i] = id.tape[i][id.head[i]];
		}
		a= machine.find_trans(id.state, current);
	}
	int start = 0;
	while (id.index[0][start] == INT_MIN)
		start++;
	int end = start;
	while (id.index[0][end] != INT_MIN)
		end++;
	out << "Result:";
	for(int i=start;i<end;i++)
		 out<< id.tape[0][i];
	out<<endl<< "==================== END ====================" << endl;
	out.close();
	for (int i = start; i < end; i++)
		output << id.tape[0][i];
	output << endl;
	output.close();
}
void init_id(print &id,string input,int num,string start){
	int len = input.length();
	int temp=len;
	int flag=0;
	if(len==0){
		input="_";
		len=10;
		flag=1;
	}
	else if(len<5){
		flag=2;
		len=10;
	}
	id.num = num;
	id.step = 0;
	id.index = new int*[num];
	id.tape = new char*[num];
	for (int i = 0; i < num; i++) {
		id.index[i] = new int[2 * len];
		id.tape[i] = new char[2 * len];
	}
	id.head = new int[num];
	id.state =start;
	//��index��ֽ�����г�ʼ��
	for (int i = 0; i < num; i++) {
		for (int j = 0; j < 2 * len; j++) {
			id.index[i][j] = INT_MIN;
			id.tape[i][j] = ';';
		}
	}
	if(flag==1){
		len=1;
	}
	else if(flag==2){
		len=temp;
	}
	//��index��ֽ���������
	for (int i = 0; i < len; i++) {
		id.index[0][i] = i;
		id.tape[0][i] = input[i];
	}
	for (int i = 1; i < num; i++) {
		id.index[i][0] = 0;
		id.tape[i][0] = '_';
	}
	for (int i = 0; i < num; i++) {
		id.head[i] = 0;
	}
}
void print_id(print &id, ofstream &out) {
	out << "Step	:" <<id.step<< endl;
	for (int i = 0; i < id.num; i++) {
		int start = 0;
		while (id.index[i][start] == INT_MIN)
			start++;
		int end = start;
		while (id.index[i][end] != INT_MIN)
			end++;
		out << "Index" << i << "	:";
		for (int j = start; j < end; j++) {
			if (id.index[i][j] < 0) {
				out << (-1)*id.index[i][j] << "	";
			}
			else
				out << id.index[i][j] << "	";
		}
		out << endl;
		out << "Tape" << i << "	:" ;
		for (int j = start; j < end; j++) {
			out << id.tape[i][j] << "	";
		}
		out << endl;
		out << "Head" << i << "	:" ;
		for (int j = start; j < end; j++) {
			if (j == id.head[i]) {
				out << "^	";
			}
			else {
				out << "	";
			}
		}
		out << endl;
	}
	out << "State	:" << id.state << endl;
	out << "------------------------------------------------------" << endl;
}
void modify_id(print &id,TransResults &res) {
	id.step++;
	id.state = res.nextstate;
	//�Զ�дͷ��λ���ϵ��ַ������޸�
	for (int i = 0; i < id.num; i++) {
		if (res.rewrite[i] != '*') {
			id.tape[i][id.head[i]] = res.rewrite[i];
		}
		if (res.l_or_r[i] != '*') {
			if (res.l_or_r[i] == 'l') {
				id.head[i]--;
			}
			else {
				id.head[i]++;
			}
		}
	}
	//����head"���"������
	for (int i = 0; i < id.num; i++) {
		int start = 0;
		while (id.index[i][start] == INT_MIN)
			start++;
		int end = start;
		while (id.index[i][end] != INT_MIN)
			end++;
		if (id.head[i] == end) {
			id.index[i][id.head[i]] = id.index[i][end-1]+1;
			id.tape[i][id.head[i]] = '_';
		}
		else if (id.head[i] < start) {
			if (start > 0) {
				id.index[i][id.head[i]] = id.index[i][start] - 1;
				id.tape[i][id.head[i]] = '_';
			}
			else {
				while (end > start) {
					id.index[i][end] = id.index[i][end - 1];
					id.tape[i][end] = id.tape[i][end - 1];
					end--;
				}
				id.index[i][0] = id.index[i][1] - 1;
				id.head[i] = 0;
				id.tape[i][id.head[i]] = '_';
			}
		}
	}
	//������С���������
	for (int i = 0; i < id.num; i++) {
		int start = 0;
		while (id.index[i][start] == INT_MIN)
			start++;
		int end = start;
		while (id.index[i][end] != INT_MIN)
			end++;
		while (start < id.head[i]) {
			if (id.tape[i][start] == '_') {
				id.tape[i][start] = ';';
				id.index[i][start] = INT_MIN;
			}
			else
				break;
			start++;
		}
		end--;
		while (end > id.head[i]) {
			if (id.tape[i][end] == '_') {
				id.tape[i][end] = ';';
				id.index[i][end] = INT_MIN;
			}
			end--;
		}
	}
}
pair<string, string> Turing_Machine::find_trans(string state, char* current) {
	string a = current;
	pair<string, string> cur(state, a);
	map<pair<string, string>, TransResults>::iterator it = machine.trans.find(cur);
	if (it != machine.trans.end()) {
		return cur;
	}
	else {
		it = machine.trans.begin();
		map<pair<string, string>, TransResults>::iterator res;
		int min = INT_MAX;
		while (it != machine.trans.end()) {
			int count = 0;
			if (it->first.first == state) {
				string trans = it->first.second;
				int i = 0;
				for (; i < machine.tape_num; i++) {
					if (current[i] == trans[i]) {
						continue;
					}
					else {
						if (trans[i] == '*') {
							count++;
						}
						else{
							break;
						}
					}
				}
				if (i == machine.tape_num) {
					if (count < min) {
						min = count;
						res = it;
					}
				}
			}
			it++;
		}
		if (min == INT_MAX) {
			pair<string, string> err(error, error);
			return err;
		}
		return res->first;
	}
}