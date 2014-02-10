#pragma once

#include <string>
#include <iostream>
#include <vector>

int UNINITIALIZED = -1;

using namespace std;

class Datum{

public:

	Datum(double v)
		:value(v){
		name = "";
	}
	Datum(string s)
		:name(s) {
		value = UNINITIALIZED;
	}

	string toString();
//
//	string getName() { return name; }
//	double getValue() { return value; }
//
//	void setName(string s) { name = s; }
//	void setValue(double v) { value = v; }
//
//private:
//
	string name;
	double value;

};

