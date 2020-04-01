#include "shannonfano.h"
#include "List.h"
#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

int main() {
	shannonfano map;
	string quote;
	cout << "Enter message you want to encode: ";
	getline(cin, quote);
	int i = 0;
	while (i < quote.length())
	{
		map.insert(quote[i]);
		i++;
	}

	map.Print();

	map.encoding_tree();

	map.show_codes();

	cout << "Now the encoded line looks like this: ";

	map.show_encoded(quote);


	cout << endl << "Amount of memory before: " << 8*quote.length() << " bits";
	cout << endl << "Amount of memory after: " << 2 * quote.length() << " bits";
	cout << endl << "Compression ratio is "<< (8 * quote.length())/(2 * quote.length());
	cout << endl << "If you want to decode a message, press 1 ";
	bool num;
	cin >> num;
	if (num == true)
	{
		cout << endl << "Enter your message: ";
		string todecode;
		cin >> todecode;
		string decoded;
		map.decoding(todecode, decoded);
		cout << endl << "Decoded message: " << decoded;
	}
	
	return 0;

}