#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>

using namespace std;

int main(void) {
	ofstream out;
	ifstream in;
	in.open("dictCMD_plain.txt");
	out.open("dictCMD_convert.txt");
	string word;
	unsigned int cmd;
	while (1) {
		in>>cmd;
		if (cmd == 99) break;
		in>>word;
		int size = word.length();
		out<<(char)(size+1)<<(char)cmd<<word<<"\n";
	}
	out.close();
	return 0;
}
