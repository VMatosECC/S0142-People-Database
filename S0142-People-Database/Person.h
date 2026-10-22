
#pragma once
#include <iostream>
#include <sstream>
using namespace std;

//Represents a person record in the People Database.
//Stores four string-based fields:
//- id:	unique identifier (default "-1")
//- name:	person's name (default "na")
//- age:	person's age as text (default "0")
//- info:	additional details (default "none")

//Includes a convenience constructor with defaults and an overloaded
//operator<< to print the record in a readable format.

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

