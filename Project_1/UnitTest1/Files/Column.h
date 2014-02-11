#ifndef COLUMN_H
#define COLUMN_H

#include "Datum.h"

class Column{

public:

	Column(string cName,string cType)
		:columnName(cName),columnType(cType){}

	Column& operator=( Column column);

	//Work on a test for this one
	int insertInto(Datum data) { Data.push_back(data); return 0;}

	const string getColumnName() { return columnName; }
	const string getColumnType() { return columnType; }
	const vector<Datum> getColumnDatum() { return Data; }

	void setColumnName(string cName) { columnName = cName; }
	void setColumnType(string cType) { columnType = cType; }
	void setColumnData(vector<Datum> data) { Data = data; }

	

private:

	string columnName;
	string columnType;
	
	vector<Datum> Data;

};


#endif
