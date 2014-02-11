#ifndef TABLE_H
#define TABLE_H

#include "Column.h"

using namespace std;

class Table{
private:
	int id;
	string name;

	vector<string> keys;
	vector<Column> columns;

public:
	Table(string tableName, vector<string> tableKeys)
		: name(tableName), keys(tableKeys){}

	string getName() { return name; }
	int getID() { return id; }
	vector<Column> getColumns() { return columns; }
	vector<string> getKeys() { return keys; }

	void addRow(vector<Datum> rowData);
	//type should be either "string" or "number"
	void addColumn(string name, string type);
};

#endif
