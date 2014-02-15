#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <iostream>
#include <vector>

const int UNINITIALIZED = -1;

using namespace std;

class Datum{
private:
	string name;
	int value;

public:
	Datum(int v) : value(v){
		name = UNINITIALIZED;
	}
	Datum(string s) : name(s) {
		value = UNINITIALIZED;
	}

	string getName() { return name; }
	int getValue() { return value; }

	void setName(string s) { name = s; }
	void setValue(int v) { value = v; }

	string toString();
};

#endif
