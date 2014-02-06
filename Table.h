#ifndef TABLE_H
#define TABLE_H

#include "Row.h"

class Table{

public:

	Table(string name, vector<string> keys);
	
	void addRow(vector<Datum> rowData);
	void addColumn(string name, string type);
	


private:

	int tableID;
	string tableName;

	vector<string> keyColumns;
	vector<Column> tableColumn;
};

#endif
