#ifndef COLUMN_H
#define COLUMN_H

#include "Datum.h"

class Column{

public:

	Column(string cName,string cType)
		:columnName(cName),columnType(cType){}

	//Work on a test for this one
	int insertInto(Datum data) { Data.push_back(data); return 0;}

	string getColumnName() { return columnName; }
	string getColumnType() { return columnType; }

	void setColumnName(string cName) { columnName = cName; }
	void setColumnType(string cType) { columnType = cType; }
	

private:

	string columnName;
	string columnType;
	
	vector<Datum> Data;

};


#endif
