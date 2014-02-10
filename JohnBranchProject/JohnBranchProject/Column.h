#ifndef COLUMN_H
#define COLUMN_H

#include "Datum.h"

class Column{

public:
	Column(string name, string type);

/*
private:
*/
	string columnType;
	string columnName;

	vector<Datum> Data;

};


#endif