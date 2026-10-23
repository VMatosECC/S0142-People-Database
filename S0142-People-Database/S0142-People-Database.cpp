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

// Shorthand Notation (Good programming practice) -----------------
// Short types below (DBmap, DBmmap) will replace long type names 
typedef map<string, list<Person>::iterator> DBmap;
typedef multimap<string, list<Person>::iterator> DBmmap;

// Utility Functions ----------------------------------------------
template <class T>
void showList(const list<T>& lst, string caption = "") {
	cout << endl << caption << " Size: " << lst.size() << endl;
	for (auto node : lst) {
		cout << node << endl;
	}
}

void showIndices(const DBmap& idMap, 
	             const DBmap& nameMap, 
	             const DBmmap& ageMultimap) {
	cout << "\nidMap (showing ID key and record)\n";
	for (auto row : idMap) {
		cout << row.first << "\t" << *row.second << endl;
	}

	cout << "\nnameMap (showing NAME key and record)\n";
	for (auto row : nameMap) {
		cout << row.first << "\t" << *row.second << endl;
	}

	cout << "\nageMultimap (showing AGE key and record(s))\n";
	for (auto row : ageMultimap) {
		cout << row.first << "\t" << *row.second << endl;
	}

}
//------------------------------------------------------------------
// Create the memory-resident database and its supporting indices
void loadDatabase(list<Person>& db, 
	              DBmap& idMap, DBmap& nameMap, DBmmap& ageMultimap) {

	//Source dataset available at 
	//https://github.com/VMatosECC/Datasets/tree/main/Simpsons
	//Download it and install locally
	ifstream infile("c:/temp/springfieldpeople.txt");
	if (!infile) {
		cout << "Fatal error - file not found\n";
		exit(101);
	}

	Person p;

	//Read disk data, assemble a Person record, save each into a list<>
	while (getline(infile, p.id, ',')) {
		getline(infile, p.name, ',');
		getline(infile, p.age, ',');
		getline(infile, p.info);

		//Add Person record to the database db
		db.push_back(p);

		//Update idMap, nameMap, and ageMultimap
		idMap[p.id] = prev(db.end());				
		nameMap[p.name] = prev(db.end());
		ageMultimap.emplace(p.age, prev(db.end()));
	}
	infile.close();
}

// --------------------------------------------------------------------------
void exactSearch(const list<Person>& db, const DBmap& m, string keyName ="") {
	// Ask the operator for a search key, use the supplied index 
	// to quickly retrieve the corresponding record. 
	// The map is based on a primary (unique) key
	
	string key;
	cout << "Testing retrieval based on: " << keyName << endl;
	do
	{
		cout << "\n(" << keyName << ") Enter search key [q to end]: ";
		getline(cin, key);

		if (key == "q") break;

		auto itm = m.find(key);
		if (itm == m.end())
		{
			cout << "Sorry - key not found " << key << endl;
		}
		else {
			cout << "Found ==> " << *itm->second << endl;
		}
	} while (true);
}

// --------------------------------------------------------------------------
void multiSearch(const list<Person>& db, const DBmmap& mm, string keyName = "") {
	// Searching through a multimap.
	// Potentially, multiple records hold the same key

	string key;
	do
	{
		cout << "\n(" << keyName << ") Enter search key [q to end]: ";
		getline(cin, key);
		if (key == "q") break;

		auto it = mm.find(key);
		if (it == mm.end()) {
			cout << "No one with the age of " << key << endl;
		}
		else {
			while (it != mm.end() && it->first == key)
			{
				cout << it->first << " " << *it->second << endl;
				it++;
			}
		}
	} while (true);
}

// -----------------------------------------------------------------------
void partialMatch(const list<Person>& db, const DBmap& m, string keyName = "") {
	//Partial match (only a prefix of the key is supplied) 
// Caution: the methods find, at, and [] perform an exact match
// For a partial match use equal_range (or lower_bound)
//---------------------------------------------------------------
	string key;
	cout << "\nTesting PARTIAL MATCH retrieval\n";
	do
	{
		cout << "\n(" << keyName << ") Enter key's prefix [q to end]: ";
		getline(cin, key);
		if (key == "q") break;

		auto it = m.lower_bound(key);
		if (it == m.end())
		{
			cout << "Sorry - key not found " << key << endl;
		}
		else {
			while (it != m.end() &&
				it->first.substr(0, key.size()) == key) {
				cout << "Found ==> " << *it->second << endl;
				it++;
			}

		}
	} while (true);
}

// --------------------------------------------------------------------------
void experiment01() {
	//Load-create database components (core + indices)
	list<Person> db;
	DBmap  idMap;
	DBmap  nameMap;
	DBmmap ageMultimap;

	//Create the database and its three indices
	loadDatabase(db, idMap, nameMap, ageMultimap);
	showList(db, "Person Database ");
	showIndices(idMap, nameMap, ageMultimap);

	//Test maps based on a primary key
	exactSearch(db, idMap, "ID");
	exactSearch(db, nameMap, "NAME");

	//Test multimap based on a non-primary key
	multiSearch(db, ageMultimap, "AGE");

	//Test a partial match query (only a prefix of the key)
	partialMatch(db, nameMap, "NAME PREFIX");


	
}

//------------------------------------------------------------
void experiment02() {
	//Load-create database components (core data + indices)
	list<Person> db;
	DBmap  idMap;
	DBmap  nameMap;
	DBmmap ageMultimap;

	//Create the database and its three indices
	loadDatabase(db, idMap, nameMap, ageMultimap);
	showList(db, "Person Database (Before insert)");

	//Insert a new Database record
	Person newPerson("200", "Otto Mann", "45", "data200");
	cout << "\n\nAdding new record ==>  " << newPerson << endl;

	if(idMap.count("200") == 1)
		throw exception("Insert ignored - similar key already exists");

	//Proceed with the insertion (update database list)
	db.push_back(newPerson);

	//Update all the indices
	idMap[newPerson.id] = prev(db.end());
	nameMap[newPerson.name] = prev(db.end());
	ageMultimap.emplace(newPerson.age, prev(db.end()));

	//Visualize database state
	showList(db, "Person Database (After insert)");
	showIndices(idMap, nameMap, ageMultimap);



	//Try to remove the target person from the database
	Person targetPerson("200", "Otto Mann", "45", "data200");
	cout << "\n\nTry to remove record ==> " << targetPerson << endl;

	auto dbit = find_if(db.begin(), db.end(),
		[&targetPerson](const Person& p) {
			return p.id == targetPerson.id;
		});

	if (dbit == db.end()) {
		throw exception("Delete ignored - Record not found");
	}


	// Step 1: Remove entry from all indexes BEFORE erasing from the list.
	// Once the list node is erased, the stored iterators become dangling.
	idMap.erase(dbit->id);
	nameMap.erase(dbit->name);

	// multimap may have several entries for this age — erase only the one
	// whose iterator points to this exact list node (dbit).
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

	cout << " Deleted: " << targetPerson << endl;


	showList(db, "Person Database after delete");
	showIndices(idMap, nameMap, ageMultimap);
}

//---------------------------------------------------------------------------
int main()
{
	//experiment01();		//create the database, test indices
	
	experiment02();		//add a new record to the database
						//then remove it
	

	cout << "\nAll done!\n";
}

