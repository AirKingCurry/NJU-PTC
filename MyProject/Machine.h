#pragma once
#include<iostream>
#include<utility>
#include<string>
#include<map>
#include<algorithm>
#include<vector>
#include<fstream>
using namespace std;
#define INT_MIN -2147483648
#define INT_MAX 2147483647
constexpr auto error = "ERRRRRRRRRROR";
struct TransResults {
	string rewrite;
	string l_or_r;
	string nextstate;
};
struct print {
	int num;//��дͷ������
	int step;
	int** index;
	char** tape;
	int* head;//head��tape_num��Ԫ��
	string state;
};
class Turing_Machine {
private:
	vector<string> states;//״̬��
	vector<char> alphabets;//������ĸ��
	vector<char> tape;//ֽ�����ż�
	string start_state;//��ʼ״̬
	string blank;//�ո����
	vector<string> finalstates;//�ս���ż�
	int tape_num;//ֽ����
	map<pair<string, string>, TransResults> trans;//ת�ƺ���
public:
	void add_states(string a);
	void add_alphabets(char a);
	void add_tape(char a);
	void add_start_states(string a);
	void add_blank(string a);
	void add_finalstates(string a);
	void add_tape_num(int a);
	void add_trans(string line);
	void run(string line);
	pair<string, string> find_trans(string state, char* current);
};
void init(char* argv[]);
void create(string line);
void init_id(print &id, string input, int num, string start);
void print_id(print &id,ofstream &out);
void modify_id(print &id,TransResults &res);