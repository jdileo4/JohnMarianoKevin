#ifndef COLUMN_H
#define COLUMN_H

#include "Datum.h"

class Column{
private:
	string type;
	string name;
	vector<Datum> data;

public:
	Column(string colName, string colType)
		: name(colName), type(colType) {}

	string getName() { return name; }
	string getType() { return type; }
	vector<Datum> getData() { return data; }

	void updateName(string newName) { name = newName; }

	void changeData(Datum newData, int id){ data[id] = newData; }
	void addData(Datum newData){ data.push_back(newData); }
};

#endif
