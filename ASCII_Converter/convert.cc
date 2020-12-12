#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>

using namespace std;

int main(void) {
	ofstream out;
	out.open("dictionaryCMDs.txt");
	string word;
	unsigned int cmd;
	while (1) {
		cin>>cmd;
		if (cmd == 99) break;
		cin>>word;
		int size = word.length();
		out<<size+1<<" ";
		for (string::iterator myIter = word.begin(); myIter < word.end(); myIter++) {
			cout<<(int)*myIter<<" ";
			out<<(int)*myIter<<" ";
		}
		cout<<endl;
		out<<"\n";
		cout<<size<<endl;
	}
	out.close();
	return 0;
}
