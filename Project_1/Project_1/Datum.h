#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Datum{
private:
	string name;
	double value;

public:
	Datum(double v) : value(v){}
	Datum(string s) : name(s) {}

	string getName() { return name; }
	double getValue() { return value; }

	void setName(string s) { name = s; }
	void setValue(double v) { value = v; }
};

#endif
