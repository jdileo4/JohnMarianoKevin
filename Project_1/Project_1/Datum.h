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
	string type;
	
public:
	Datum(){
		name = "";
		value = UNINITIALIZED;
	}
	//type is either "string" or "number"
	Datum(void* value, string type){
		if (type == "string"){
			this->value = UNINITIALIZED;
			this->type = type;
			string* temp = (string*)value;
			name = *temp;
		}
		else if (type == "number"){
			this->name = "";
			this->type = type;
			int* temp = (int*)value;
			this->value = *temp;
		}

	}
	Datum(int v) : value(v){
		name = "";
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
