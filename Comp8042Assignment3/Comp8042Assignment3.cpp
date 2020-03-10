// Comp8042Assignment3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int main()
{
	/*ofstream myfile;
	myfile.open("VA_Bath.txt");
	myfile << "Writing this to a file.\n";
	myfile.close();*/
	string line;
	ifstream myfile2("VA_Bath.txt");
	if (myfile2.is_open())
	{
		while (getline(myfile2, line))
		{
			cout << line << '\n';
		}
		myfile2.close();
	}
}

