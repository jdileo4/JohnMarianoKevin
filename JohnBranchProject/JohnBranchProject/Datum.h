#pragma once

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Datum{

public:

	Datum(double v)
		:value(v){}
	Datum(string s)
		:name(s) {}

	string getName() { return name; }
	double getValue() { return value; }

	void setName(string s) { name = s; }
	void setValue(double v) { value = v; }

private:

	string name;
	double value;

};

