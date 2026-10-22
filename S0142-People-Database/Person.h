#pragma once
#include <iostream>
#include <sstream>
using namespace std;

struct Person
{
	//data members
	string id;
	string name;
	string age;
	string info;

	//constructor
	Person(string i = "-1", string n = "na",
		string a = "0", string infoVal = "none")
		: id(i), name(n), age(a), info(infoVal)
	{
	}

	//operator<<
	friend ostream& operator<<(ostream& sout, const Person& p) {
		sout << " Person [Id: " << p.id
			<< ", Name: " << p.name
			<< ", Age: " << p.age
			<< ", Info: " << p.info << "]";
		return sout;
	}
};

