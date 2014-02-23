#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>

using namespace std;

class Datum{

public:

	Datum(double v)
		:value(v),name("NULL"){}
	Datum(string s)
		:name(s),value(0){}



	string getName() { return name; }
	double getValue() { return value; }

	void setName(string s) { name = s; }
	void setValue(double v) { value = v; }

private:

	string name;
	double value;

};

#endif
