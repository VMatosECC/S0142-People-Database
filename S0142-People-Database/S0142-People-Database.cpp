// S0142-People-Database.cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <map>
#include "Person.h"
#include <vector>
using namespace std;

// Utility Functions ----------------------------------------------
template <class T>
void showList(const list<T>& lst, string caption = "") {
	cout << endl << caption << " Size: " << lst.size() << endl;
	for (auto node : lst) {
		cout << node << endl;
	}
}

// --------------------------------------------------------------------------

void experiment01() {
	ifstream infile("c:/temp/springfieldpeople.txt");
	if (!infile) {
		cout << "Fatal error - file not found\n";
		exit(101);
	}

	Person p;
	list<Person> db;
	// Hint replace long type names with a shorter nickname 
	// such as DBmap - see examples below
	typedef map<string, list<Person>::iterator> DBmap;
	typedef multimap<string, list<Person>::iterator> DBmmap;

	map<string, list<Person>::iterator> idMap;		//or DBmap idMap
	map<string, list<Person>::iterator> nameMap;	//or DBmap nameMap
	multimap<string, list<Person>::iterator> ageMultimap; //or DBmmap ageMultimap

	//Hint: place this code segment into a loadDatabase function
	while (getline(infile, p.id, ',')) {
		getline(infile, p.name, ',');
		getline(infile, p.age, ',');
		getline(infile, p.info);
		//cout << p << endl;

		db.push_back(p);
		idMap[p.id] = prev(db.end());
		nameMap[p.name] = prev(db.end());
		ageMultimap.emplace(p.age, prev(db.end()));

	}

	infile.close();
	showList(db, "Person Database ");

	string key;

	//Test the ID map - Exact search
	cout << "Testing ID based retrieval\n";
	do
	{
		cout << "\nEnter an ID key [q to end]: ";
		getline(cin, key);

		if (key == "q") break;

		auto itm = idMap.find(key);
		if (itm == idMap.end())
		{
			cout << "Sorry - key not found " << key << endl;
		}
		else {
			cout << "Found ==> " << *itm->second << endl;
			cout << "\tid:   " << itm->second->id << endl;
			cout << "\tname: " << itm->second->name << endl;
		}
	} while (true);

	//---------------------------------------------------------------
	// Test the nameMap exact search
	// Hint: this code segment is identical to the one above
	//       refactor this segment as a function, such as exactSearch
	do
	{
		cout << "\nEnter a NAME key [q to end]: ";
		getline(cin, key);
		if (key == "q") break;

		auto itm = nameMap.find(key);
		if (itm == nameMap.end())
		{
			cout << "Sorry - key not found " << key << endl;
		}
		else {
			cout << "Found ==> " << *itm->second << endl;
			cout << "\tid:   " << itm->second->id << endl;
			cout << "\tname: " << itm->second->name << endl;
		}
	} while (true);

	// ______________________________________________________
	// Testing ageMultimap retieval
	
	
	do
	{
		cout << "\nEnter an AGE key [q to end]: ";
		getline(cin, key);
		if (key == "q") break;

		auto it = ageMultimap.find(key);
		if (it == ageMultimap.end()) {
			cout << "No one with the age of " << key << endl;
		}
		else {
			while (it != ageMultimap.end() && it->first == key)
			{
				cout << it->first << " " << *it->second << endl;
				it++;
			}
		}
	} while (true);

	//Partial match -------------------------------
	// find, at, and [] perform an exact match
	// For a partial match use lower_bound
	// Hint: explore lower_bound retrieval
	//---------------------------------------------------------------
	cout << "\nTesting NAME PARTIAL MATCH retrieval\n";
	do
	{
		cout << "\nEnter a NAME's prefix [q to end]: ";
		getline(cin, key);
		if (key == "q") break;

		auto itm = nameMap.lower_bound(key);
		if (itm == nameMap.end())
		{
			cout << "Sorry - key not found " << key << endl;
		}
		else {
			while (itm != nameMap.end() && 
				   itm->first.substr(0, key.size()) == key) {
					cout << "Found ==> " << *itm->second << endl;
					itm++;
			}
			
		}
	} while (true);
	
	//Insert a new Database record
	Person newPerson ("200", "Otto Mann", "45", "data200");
	db.push_back(newPerson);
	//Update all the indices
	idMap[newPerson.id] = prev(db.end());
	nameMap[newPerson.name] = prev(db.end());
	ageMultimap.emplace(newPerson.age, prev(db.end()));
	showList(db, "Person Database");

	//Delete a person from the database (remove newPerson)
	auto dbit = find_if(db.begin(), db.end(), 
		[&newPerson](const Person& p) {
		return p.id == newPerson.id;
		});
		
	if (dbit == db.end()) {
		cout << "Record not found: " << newPerson.id << "\n";
	}
	else {
		// Step 1: Remove from all indexes BEFORE erasing from the list.
		// Once the list node is erased, the stored iterators become dangling.
		idMap.erase(dbit->id);
		nameMap.erase(dbit->name);

		// multimap may have several entries for this age — erase only the one
		// whose iterator points to this exact list node.
		auto range = ageMultimap.equal_range(dbit->age);
		auto first = range.first;
		auto last = range.second;

		for (auto ait = first; ait != last; ++ait) {
			if (ait->second == dbit) {          // same list node?
				ageMultimap.erase(ait);
				break;
			}
		}

		// Step 2: Erase from the primary list.
		db.erase(dbit);

		cout << " Deleted: " << newPerson.id << " ==> " << newPerson.name << "\n";
	}

	showList(db, "Person Database after delete");
}
//------------------------------------------------------------


int main()
{

	experiment01();


	cout << "\nAll done!\n";
}

