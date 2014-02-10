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

	void changeData(Datum newData, int id){ data[id] = newData; }
	void updateData(Datum newData){ data.push_back(newData); }
};

#endif
